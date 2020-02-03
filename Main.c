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
#include "7SEGHEX.h"

int Btnmas=0;
int Btnmenos=0;

void main(void) {
    
    int contbtn1=0;
    int contbtn2=0;
    int display=0;
    int ndisplay1=0;
    int ndisplay2=0;
    //Config TMR0 
    TMR0=0;//Limpia el timer
    INTCON.T0IE=1; // Habilita los bits del Timer 0
    INTCON.T0IF=0;// Limpiar la bandera 
    OPTION_REG.T0CS=0; //Se Trabaja con el reloj interno
    OPTION_REG.PSA=0; // Establecer el prescaler al TMR0
    OPTION_REG.PS2=0;
    OPTION_REG.PS1=1;
    OPTION_REG.PS0=1; //1:16
    
    INTCON.INTE=1; // Habilitar ints externos (Botones puerto b)
    INTCON.RBIE=1; // Habilitar botones (ints)
    INTCON.INTF=0; // Limpia bandera externa
    INTCON.RBIF=0; // Limpia bandera del puerto B 
    OPTION_REG.7=0; //Desabilita los pullups en el puerto B 
    OPTION_REG.INTEDG=1; // External interrupts will happen on rising age
    IOCB= 0b00000011; // RB0 y RB1 con interrupciones
    
    //ADC
    INTCON.PEIE=1 //
    PIR1.ADIF=0; // Clear Bandera ADC
    ADCON0.ADC1=0;//
    ADCON0.ADCS0=1;// Fosc/8
    ADCON0.CHS3=0;
    ADCON0.CHS2=0;
    ADCON0.CHS0=0;// Canal analógico AN0
    ADCON0.1=0;// Interrupciones 
    ADCON0.ADON=1; // Habilita ADC
    ADCON1.ADFM=1; // Pines significativos en ADRESH
    ADCON1.VCFG1=0; // Referencia a Tierra
    ANSEL.0=1; // RA0 como pin analógico
    
    //PUERTOS
    PORTA=0; // Limpia el Puerto A
    PORTB=0;// Limpia el Puerto B
    PORTC=0; // Limpia el Puerto C
    PORTD=0; // Limpia el Puerto D
    TRISA=1; // RA0 INPUTS
    TRISB= 0b00000011; // RB0 y RB1 inputs, los demás outputs
    TRISC=0; // Output
    TRISD=0; //Output
    
    INTCON.GIE=1; //Habilita las interrupciones
    
    

   
    
/* 
int val1=0b00000000;
int val2=0b00000000;

PORTA = val1;
PORTB.RB0 = 1; // (Activar el transistor del primer 7seg
__delay_ms(5);; //(Delay recomendado de 5ms entre cambios)
PORTB.RB0 = 0;// Desactivo el transistor 
PORTA = val2; //(Valor del otro 7 seg)
PORTB.RB1 = 1; //(segundo transistor del segundo 7 seg)
PORTB.RB1 = 0; //(Desactivar para poder colocar el siguiente valor)
 
  */   
    
    while (1)
    {
    if (Btnmas==1){
        if (contbtn1<=100){
            contbtn1++;
        }
        else {
            if (contbtn1>100 && Btnmas==1){
                PORTA++;
                
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
                PORTA--;
            }
        }
    }

    if (T0IF.INTCON==1){
        if (display==1)
        { display=0;
        }
        if (display==0)
        { display=1;
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
    
    
    }
    
   return;
} 

  void __interrupt() ISR(){
      
          if (INTCON.RBIF==1)
          {
              if (PORTBbits.RB0==1)
              { Btnmas=1;
              PORTEbits.RE0=1;
              }
              if (PORTBbits.RB1==1)
              { Btnmenos=1;
              PORTEbits.RE1=1;
              }
              
              }
          }
      
       
  