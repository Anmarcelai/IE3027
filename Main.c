
  

// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
void __interrupt() ISR();
#include <xc.h>
#include "SEGHEX.h"

#define _XTAL_FREQ 4000000

int Btnmas=0;
int Btnmenos=0;
int ndisplay1=0;
int ndisplay2=0;
int ndisplaycompleto=0;
int contbtn1=0;
int contbtn2=0;
int display=0;

void main(void) {    
    
    //Config TMR0 
    TMR0=0;//Limpia el timer
    INTCONbits.T0IE=1; // Habilita los bits del Timer 0
    INTCONbits.T0IF=0;// Limpiar la bandera 
    OPTION_REGbits.T0CS=0; //Se Trabaja con el reloj interno
    OPTION_REGbits.PSA=0; // Establecer el prescaler al TMR0
    OPTION_REGbits.PS2=0;
    OPTION_REGbits.PS1=1;
    OPTION_REGbits.PS0=1; //1:16
    
    INTCONbits.INTE=1; // Habilitar ints externos (Botones puerto b)
    INTCONbits.RBIE=1; // Habilitar botones (ints)
    INTCONbits.INTF=0; // Limpia bandera externa
    INTCONbits.RBIF=0; // Limpia bandera del puerto B 
    OPTION_REGbits.nRBPU=0; //Desabilita los pullups en el puerto B 
    OPTION_REGbits.INTEDG=1; // External interrupts will happen on rising age
    IOCB= 0b00000011; // RB0 y RB1 con interrupciones
    
    //ADC
    INTCONbits.PEIE=1; //
    PIR1bits.ADIF=0; // Clear Bandera ADC
    ADCON0bits.ADCS1=0;//
    ADCON0bits.ADCS0=1;// Fosc/8
    ADCON0bits.CHS3=0;
    ADCON0bits.CHS2=0;
    ADCON0bits.CHS0=0;// Canal analógico AN0
    ADCON0bits.GO_nDONE=0;// Interrupciones 
    ADCON0bits.ADON=1; // Habilita ADC
    ADCON1bits.ADFM=1; // Pines significativos en ADRESH
    ADCON1bits.VCFG1=0; // Referencia a Tierra
    ANSELbits.ANS0=1; // RA0 como pin analógico
    
    //PUERTOS
    PORTA=0; // Limpia el Puerto A
    PORTB=0;// Limpia el Puerto B
    PORTC=0; // Limpia el Puerto C
    PORTD=0; // Limpia el Puerto D
    TRISA=1; // RA0 INPUTS
    TRISB= 0b00000011; // RB0 y RB1 inputs, los demás outputs
    TRISC=0; // Output
    TRISD=0; //Output
    TRISE=0;
    INTCONbits.GIE=1; //Habilita las interrupciones
    
    while (1)
    {
      ADCON0bits.ADON=1;
      __delay_ms(1);
      ADCON0bits.GO=1;
      
        
    if (Btnmas==1){
        if (contbtn1<=100){
            contbtn1++;
        }
        else {
            if (contbtn1>100 && Btnmas==1){
                PORTD++;
                
            }
        }
    }
    
    
    if (Btnmenos==1){
        if (contbtn2<=100){
            contbtn2++;
        }
        else {
            if (contbtn2>100 && Btnmenos==1){
                //codigo de decremento
                PORTD--;
            }
        }
    }
    
    if (display==1){
        PORTBbits.RB3=0;
        PORTBbits.RB4=1;
        PORTC=tabla7seg(ndisplay1);
    }
    if (display==0){
        PORTBbits.RB3=1;
        PORTBbits.RB4=0;
        PORTC=tabla7seg(ndisplay2);
    }
    
      if (ndisplaycompleto==PORTD){
          PORTBbits.RB2=1;//Activa LED
          PORTEbits.RE0=1; //Activa Buzzer
                                   }
                                   }
      
    }
     

void __interrupt() ISR(){
    if (INTCONbits.RBIF==1)
    {
        if (PORTBbits.RB0==1)
        { Btnmas=1;
        PORTBbits.RB0=1;
        PORTBbits.RB2 = 1;
        }
        if (PORTBbits.RB1==1)
        { Btnmenos=1;
        PORTBbits.RB1=1;
        }
    }
    if (INTCONbits.T0IF==1){
        if (display==1)
        { display=0;
        }
        if (display==0)
        { display=1;
        }
    }

    if ( PIR1bits.ADIF==1){
        ADCON0bits.ADON=0; //Desactivamos el ADC
        ndisplay1=ADRESH*0b00001111; //Primeros 4 bits
        ndisplay2=(ADRESH*0b11110000)>> 4; // Bits restantes y los corre para usar la misma tabla
        ndisplaycompleto=ADRESH; 
        PIR1bits.ADIF=0;
    }
}
             
 
      
       