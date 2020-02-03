unsigned int tabla7seg(unsigned int num) {
    // b a f g e d c .
    switch(num){
        case 0:
            return 0b11101110;
            break;
        case 1:
            return 0b10000010;
            break;
        case 2:
            return 0b11011100;
            break;
        case 3:
            return 0b11010110;
            break;
        case 4:
            return 0b10110010;
            break;
        case 5:
            return 0b01110110;
            break;
        case 6:
            return 0b01111110;
            break;
        case 7:
            return 0b11000010;
            break;
        case 8:
            return 0b11111110;
            break;
        case 9:
            return 0b11110110;
            break;
        case 10:
            return 0b11111011;
            break;
        case 11:
            return 0b00111111;
            break;
        case 12:
            return 0b01101101;
            break;
        case 13:
            return 0b10011111;
            break;
        case 14:
            return 0b01111101;
            break;
        case 15:
            return 0b01111001;
    }
}