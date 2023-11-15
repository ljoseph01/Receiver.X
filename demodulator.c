#include "demodulator.h"
#include <string.h>
#include <stdbool.h>
//#define DEBUG_PRINT

volatile uint8_t DEMODULATOR_TimerPeriodOver;

static void waitHalfPeriod(void);

void DEMODULATOR_Init()
{
    PLL_Reset();
}

void DEMODULATOR_16QAM_Uncoded(Complex* symbolReadings, uint8_t* destBuffer)
{
    waitHalfPeriod();
    int numberOfSymbols = BLOCK_LENGTH_SYMBOLS * 3 / 4;
    Complex estimatedSymbol;
    int outputBits;
    bool pilotSequenceOver = false;
#ifdef DEBUG_PRINT
    Complex tempSymbolsPrePLL[BLOCK_LENGTH_SYMBOLS + PILOT_LENGTH];
#endif
    Complex measuredSymbol;
    
    measuredSymbol = ADC_ReadComplex();
    PLL_First16QAM(measuredSymbol);
#ifdef DEBUG_PRINT
    tempSymbolsPrePLL[0] = measuredSymbol;
#endif
    while(!DEMODULATOR_TimerPeriodOver);
    DEMODULATOR_TimerPeriodOver = 0;
    int numPilotSymbolsFound;
    for (numPilotSymbolsFound = 1; numPilotSymbolsFound < PILOT_LENGTH; numPilotSymbolsFound++) {
        if (pilotSequenceOver) break;
        measuredSymbol = ADC_ReadComplex();
        pilotSequenceOver = PLL_Pilot16QAM(measuredSymbol);
#ifdef DEBUG_PRINT
        tempSymbolsPrePLL[numPilotSymbolsFound] = measuredSymbol;
#endif
        while(!DEMODULATOR_TimerPeriodOver);
        DEMODULATOR_TimerPeriodOver = 0;
    }
    
    for (int i = 0; i < numberOfSymbols; i++) {
        measuredSymbol = ADC_ReadComplex();
        symbolReadings[i] = measuredSymbol;
#ifdef DEBUG_PRINT
        tempSymbolsPrePLL[i + PILOT_LENGTH] = measuredSymbol;
#endif
        while(!DEMODULATOR_TimerPeriodOver);
        DEMODULATOR_TimerPeriodOver = 0;
    }
    
    
    
    for (int i = 0; i < numberOfSymbols; i++) {
        Complex measuredSymbol = symbolReadings[i];
        estimatedSymbol = PLL_16QAM(measuredSymbol, symbolReadings + i);
//        while(!DEMODULATOR_TimerPeriodOver);
//        DEMODULATOR_TimerPeriodOver = 0;
        outputBits = SYMBOLS_GetBits16QAMUncoded(estimatedSymbol);
        
        if (i % 2 == 0) {
            destBuffer[i / 2] = outputBits << 4;
        }
        else {
            destBuffer[i / 2] |= outputBits;
        }
    }
    TIMER_Off();
#ifdef DEBUG_PRINT
    // TEMP FOR DEBUG///////////////////////////////////////////////////////
    /**/for (int i = 0; i < numPilotSymbolsFound; i++) {                  //
    /**/    printf("%f,%f\n",                                             //
    /**/            tempSymbolsPrePLL[i].real,                            //
    /**/            tempSymbolsPrePLL[i].imag);                           //
    /**/}                                                                 //
    /**/for (int i = numPilotSymbolsFound; i < PILOT_LENGTH; i++) {       //
    /**/    printf("%f,%f\n",                                             //
    /**/            0.0f,                                                 //
    /**/            0.0f);                                                //
    /**/}                                                                 //
    /**/for (int i = 0; i < numberOfSymbols; i++) {                       //
    /**/    printf("%f,%f\n",                                             //
//    /**/            tempSymbolsPrePLL[i + PILOT_LENGTH].real,             //
//    /**/            tempSymbolsPrePLL[i + PILOT_LENGTH].imag);            //    
    /**/            symbolReadings[i].real,                               //
    /**/            symbolReadings[i].imag);                              //
    /**/}                                                                 //
    /**/for (int i = 0; i < BLOCK_LENGTH_SYMBOLS - numberOfSymbols; i++) {//
    /**/    printf("%f,%f\n", 0.0f, 0.0f);                                //
    /**/}                                                                 //
    ////////////////////////////////////////////////////////////////////////
#endif
}

void DEMODULATOR_16QAM_Coded(Complex* symbolReadings, uint8_t* destBuffer)
{
    waitHalfPeriod();
    int numberOfSymbols = BLOCK_LENGTH_SYMBOLS;
#ifdef DEBUG_PRINT
    Complex tempSymbolsPrePLL[BLOCK_LENGTH_SYMBOLS + PILOT_LENGTH];
#endif
    Complex measuredSymbol;
    bool pilotSequenceOver = false;
//    Complex* symbolReadingsIter = symbolReadings;
    
    measuredSymbol = ADC_ReadComplex();
    PLL_First16QAM(measuredSymbol);
#ifdef DEBUG_PRINT
    tempSymbolsPrePLL[0] = measuredSymbol;
#endif
    Complex first = measuredSymbol;
    
//    memset(destBuffer, 0, BLOCK_LENGTH_BYTES);
    while(!DEMODULATOR_TimerPeriodOver);
    DEMODULATOR_TimerPeriodOver = 0;
    int numPilotSymbolsFound;
    for (numPilotSymbolsFound = 1; numPilotSymbolsFound < PILOT_LENGTH; numPilotSymbolsFound++) {
        if (pilotSequenceOver) break;
        measuredSymbol = ADC_ReadComplex();
        pilotSequenceOver = PLL_Pilot16QAM(measuredSymbol);
#ifdef DEBUG_PRINT
        tempSymbolsPrePLL[numPilotSymbolsFound] = measuredSymbol;
#endif
        while(!DEMODULATOR_TimerPeriodOver);
        DEMODULATOR_TimerPeriodOver = 0;
    }
    
    for (int i = 0; i < numberOfSymbols; i++) {
        measuredSymbol = ADC_ReadComplex();
        symbolReadings[i] = measuredSymbol;
#ifdef DEBUG_PRINT
        tempSymbolsPrePLL[i + PILOT_LENGTH] = measuredSymbol;
#endif
        while(!DEMODULATOR_TimerPeriodOver);
        DEMODULATOR_TimerPeriodOver = 0;
    }
    TIMER_Off();
    for (int i = 0; i < numberOfSymbols; i++) {
        Complex measuredSymbol = symbolReadings[i];
        (void) PLL_16QAM(measuredSymbol, symbolReadings + i);
    }
#ifdef DEBUG_PRINT
    // TEMP FOR DEBUG///////////////////////////////////////////////////////
    /**/for (int i = 0; i < numPilotSymbolsFound; i++) {                  //
    /**/    printf("%f,%f\n",                                             //
    /**/            tempSymbolsPrePLL[i].real,                            //
    /**/            tempSymbolsPrePLL[i].imag);                           //
    /**/}                                                                 //
    /**/for (int i = numPilotSymbolsFound; i < PILOT_LENGTH; i++) {       //
    /**/    printf("%f,%f\n",                                             //
    /**/            0.0f,                                                 //
    /**/            0.0f);                                                //
    /**/}                                                                 //
    /**/for (int i = 0; i < BLOCK_LENGTH_SYMBOLS; i++) {                  //
    /**/    printf("%f,%f\n",                                             //
//    /**/            tempSymbolsPrePLL[i + PILOT_LENGTH].real,             //
//    /**/            tempSymbolsPrePLL[i + PILOT_LENGTH].imag);            //    
    /**/            symbolReadings[i].real,                               //
    /**/            symbolReadings[i].imag);                              //
    /**/}                                                                 //
    ////////////////////////////////////////////////////////////////////////
#endif
    VITERBI_Decode(symbolReadings, destBuffer);
}

void DEMODULATOR_8PSK_Uncoded(Complex* symbolReadings, uint8_t* destBuffer)
{
    waitHalfPeriod();
    int numberOfSymbols = BLOCK_LENGTH_SYMBOLS;
    Complex estimatedSymbol;
    uint8_t outputBits;
    int destBufferIter = 0;
    bool pilotSequenceOver = false;
#ifdef DEBUG_PRINT
    Complex tempSymbolsPrePLL[BLOCK_LENGTH_SYMBOLS + PILOT_LENGTH];
#endif
    Complex measuredSymbol;
    
//    Complex temp[numberOfSymbols];
    measuredSymbol = ADC_ReadComplex();
    PLL_First8PSK(measuredSymbol);
#ifdef DEBUG_PRINT
    tempSymbolsPrePLL[0] = measuredSymbol;
#endif
    while(!DEMODULATOR_TimerPeriodOver);
    DEMODULATOR_TimerPeriodOver = 0;
    int numPilotSymbolsFound;
    for (numPilotSymbolsFound = 1; numPilotSymbolsFound < PILOT_LENGTH; numPilotSymbolsFound++) {
        if (pilotSequenceOver) break;
        measuredSymbol = ADC_ReadComplex();
        pilotSequenceOver = PLL_Pilot8PSK(measuredSymbol);
#ifdef DEBUG_PRINT
        tempSymbolsPrePLL[numPilotSymbolsFound] = measuredSymbol;
#endif
        while(!DEMODULATOR_TimerPeriodOver);
        DEMODULATOR_TimerPeriodOver = 0;
    }
    for (int i = 0; i < numberOfSymbols; i++) {
        measuredSymbol = ADC_ReadComplex();
        symbolReadings[i] = measuredSymbol;
#ifdef DEBUG_PRINT
        tempSymbolsPrePLL[i + PILOT_LENGTH] = measuredSymbol;
#endif
        while(!DEMODULATOR_TimerPeriodOver);
        DEMODULATOR_TimerPeriodOver = 0;
    }
    for (int i = 0; i < numberOfSymbols; i++) {
        Complex measuredSymbol = symbolReadings[i];
        estimatedSymbol = PLL_8PSK(measuredSymbol, symbolReadings + i);
//        while(!DEMODULATOR_TimerPeriodOver);
//        DEMODULATOR_TimerPeriodOver = 0;
        outputBits = SYMBOLS_GetBits8PSK(estimatedSymbol);
        
        // There are 8 possible places the output bits can go
        switch (i % 8) {
            case 0:
                destBuffer[destBufferIter] = outputBits << 5;
                break;
            case 1:
                destBuffer[destBufferIter] |= outputBits << 2;
                break;
            case 2:
                destBuffer[destBufferIter] |= outputBits >> 1;
                destBufferIter++;
                destBuffer[destBufferIter] = (outputBits << 7) & 0xFF;
                break;
            case 3:
                destBuffer[destBufferIter] |= outputBits << 4;
                break;
            case 4:
                destBuffer[destBufferIter] |= outputBits << 1;
                break;
            case 5:
                destBuffer[destBufferIter] |= outputBits >> 2;
                destBufferIter++;
                destBuffer[destBufferIter] = (outputBits << 6) & 0xFF;
                break;
            case 6:
                destBuffer[destBufferIter] |= outputBits << 3;
                break;
            case 7:
                destBuffer[destBufferIter] |= outputBits;
                destBufferIter++;
                break;
        }
    }
    TIMER_Off();
    
#ifdef DEBUG_PRINT
    // TEMP FOR DEBUG///////////////////////////////////////////////////////
    /**/for (int i = 0; i < numPilotSymbolsFound; i++) {                  //
    /**/    printf("%f,%f\n",                                             //
    /**/            tempSymbolsPrePLL[i].real,                            //
    /**/            tempSymbolsPrePLL[i].imag);                           //
    /**/}                                                                 //
    /**/for (int i = numPilotSymbolsFound; i < PILOT_LENGTH; i++) {       //
    /**/    printf("%f,%f,%f,%f\n",                                       //
    /**/            0.0f,                                                 //
    /**/            0.0f);                                                //
    /**/}                                                                 //
    /**/for (int i = 0; i < BLOCK_LENGTH_SYMBOLS; i++) {                  //
    /**/    printf("%f,%f\n",                                             //
//    /**/            tempSymbolsPrePLL[i + PILOT_LENGTH].real,             //
//    /**/            tempSymbolsPrePLL[i + PILOT_LENGTH].imag);            //    
    /**/            symbolReadings[i].real,                               //
    /**/            symbolReadings[i].imag);                              //
    /**/}                                                                 //
    ////////////////////////////////////////////////////////////////////////
#endif
}

void DEMODULATOR_Reset()
{
    PLL_Reset();
}

static void waitHalfPeriod()
{
    TIMER_SetPeriodPerc(0.01f);
    DEMODULATOR_TimerPeriodOver = 0;
    TIMER_On();
    while (!DEMODULATOR_TimerPeriodOver);
    TIMER_Off();
    TIMER_SetFullPeriod();
    DEMODULATOR_TimerPeriodOver = 0;
    TIMER_On();
}