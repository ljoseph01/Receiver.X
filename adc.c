#include "adc.h"

void ADC_Init(void)
{
    ANSELAbits.ANSELA0 = 1;
    ADCON1L = 0x8000;
    ADCON1H = 0x0060;
    ADCON2L = 0x0000;
    ADCON2H = 0x0000;
    ADCON3L = 0x0000;
    ADCON3Hbits.CLKSEL = 0b11;
    ADCON3Hbits.CLKDIV = 0b000001;
    ADCON3Hbits.C0EN = 1;
    ADCON3Hbits.C1EN = 1;
    ADCON5Lbits.C0PWR = 1;
    ADCON5Lbits.C1PWR = 1;
    ADCON5H = 0x0200;
    
    ADCORE0Hbits.RES = 0b11;
    ADCORE1Hbits.RES = 0b11;
    
    ADTRIG0Lbits.TRGSRC0 = 1;
    ADTRIG0Lbits.TRGSRC1 = 1;
    
//    ADMOD0L = 0xF;
}

Complex ADC_ReadComplex(void)
{
    while (!ADCON5Lbits.C0RDY);
    while (!ADCON5Lbits.C1RDY);

    ADCON3Lbits.SWCTRG = 1;

    while (!ADSTATLbits.AN0RDY);
    while (!ADSTATLbits.AN1RDY);
    
    int16_t real = (int16_t) ADCBUF0;
    int16_t imag = (int16_t) ADCBUF1;
    
    // 1.5 is a hand calculated scaling value
    return (Complex) {
        1.55 * (real - 2047) / 2048.0f,
        1.55 * (imag - 2047) / 2048.0f
    };
}