#include "adc.h"

static float _realOffset;
static float _imagOffset;

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
    
    _realOffset = 2047.0f;
    _imagOffset = 2047.0f;
    
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
    
    // 1.5 is a hand calculated scaling value (now 1.62)
    return (Complex) {
        1.62 * ((float)real - _realOffset) / 2048.0f,
        1.62 * ((float)imag - _imagOffset) / 2048.0f
    };
}

void ADC_Calibrate(void)
{
    float realTotal = 0.0f, imagTotal = 0.0f;
    int N = 100;
    for (int i = 0; i < N; i++) {
        while (!ADCON5Lbits.C0RDY);
        while (!ADCON5Lbits.C1RDY);

        ADCON3Lbits.SWCTRG = 1;

        while (!ADSTATLbits.AN0RDY);
        while (!ADSTATLbits.AN1RDY);

        int16_t real = (int16_t) ADCBUF0;
        int16_t imag = (int16_t) ADCBUF1;
        realTotal += (float) real;
        imagTotal += (float) imag;
    }
    _realOffset = realTotal / N;
    _imagOffset = imagTotal / N;
}