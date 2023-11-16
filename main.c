/*
 * File:   main.c
 * Author: User
 *
 * Created on 21 August 2023, 2:29 PM
 */


#include <xc.h>
#include <string.h>
#include <stdio.h>
#include <p33CK256MP508.h>

#include "core.h"
#include "viterbi.h"
#include "complex.h"
#include "system.h"
#include "adc.h"
#include "uart1.h"
#include "timer_100us.h"
#include "serial.h"
#include "pll.h"
#include "demodulator.h"



volatile uint8_t MAIN_TimerPeriodOver;

static void initialiseSystem(void);
static void viterbiDebug(uint8_t* destBuffer);


int main(void) {
    
    initialiseSystem();
    
    DataMode dataMode = CODED_16QAM;
    uint8_t serialByte;
    bool start = false;
    
    Complex symbolReadings[BLOCK_LENGTH_SYMBOLS];
    uint8_t destBuffer[BLOCK_LENGTH_BYTES];
    
    for (int i = 0; i < BLOCK_LENGTH_BYTES; i++)
        destBuffer[i] = 0;
    
//    viterbiDebug(destBuffer);
//    while (1);
    
//    while (1) {
//        float reading = COMPLEX_Magnitude(ADC_ReadComplex());
//        if (reading > 0.1f)
//            printf("%f\n\r", reading);
//    }
    while (1)
    {
        // Check for serial message
        if (UART1_TRANSFER_STATUS_RX_DATA_PRESENT & UART1_TransferStatusGet()) {
            serialByte = UART1_Read();
            
            switch (serialByte) {
                case SERIAL_MODE_16QAM_CODED:
                    dataMode = CODED_16QAM;
//                    PLL_SetMode(dataMode);
                    break;
                case SERIAL_MODE_16QAM_UNCODED:
                    dataMode = UNCODED_16QAM;
//                    PLL_SetMode(dataMode);
                    break;
                case SERIAL_MODE_8PSK_UNCODED:
                    dataMode = UNCODED_8PSK;
//                    PLL_SetMode(dataMode);
                    break;
                case SERIAL_NAME_REQUEST:
                    while (UART1_TRANSFER_STATUS_TX_FULL & UART1_TransferStatusGet());
                    UART1_Write(0x0b);
                    break;
                case SERIAL_ADC_CALIBRATE:
                    ADC_Calibrate();
                    break;
            }
        }
        
        // Check for signal onset
        if (COMPLEX_Magnitude(ADC_ReadComplex()) > 0.14f) {
//            for (int i = 0; i < BLOCK_LENGTH_BYTES; i++)
//                destBuffer[i] = 0;
//            
            switch (dataMode) {
                case CODED_16QAM:
                    DEMODULATOR_16QAM_Coded(symbolReadings, destBuffer);
                    break;
                case UNCODED_16QAM:
                    DEMODULATOR_16QAM_Uncoded(symbolReadings, destBuffer);
                    break;
                case UNCODED_8PSK:
                    DEMODULATOR_8PSK_Uncoded(symbolReadings, destBuffer);
                    break;
            }
            
            int numBytes = 0;
            UART1_TRANSFER_STATUS status;
            while (numBytes < BLOCK_LENGTH_BYTES) {
                status = UART1_TransferStatusGet();
                if (status & UART1_TRANSFER_STATUS_TX_EMPTY) {
                    numBytes += UART1_WriteBuffer(destBuffer + numBytes, 
                            BLOCK_LENGTH_BYTES - numBytes);
                }
            }
            
        }
    }
//    printf("Hello world!\n\r");
    
    uint16_t counter = 0;
    Complex reading;
    double mag;
    
    for (int i = 0; i < BLOCK_LENGTH_BYTES; i++)
        destBuffer[i] = 0;
    
//    viterbiDebug(destBuffer);
//    while (1);
    
   
//    while (1) {
//        printf("%f\n\r", COMPLEX_Magnitude(ADC_ReadComplex()));
//    }
    while (COMPLEX_Magnitude(ADC_ReadComplex()) < 0.03f);

    TIMER_SetHalfPeriod();
    MAIN_TimerPeriodOver = 0;
    TIMER_On();
    while (!MAIN_TimerPeriodOver);
    TIMER_Off();
    TIMER_SetFullPeriod();
    MAIN_TimerPeriodOver = 0;
    TIMER_On();

    for (int i = 0; i < BLOCK_LENGTH_SYMBOLS; i++)
    {
        symbolReadings[i] = ADC_ReadComplex();
        while(!MAIN_TimerPeriodOver);
        MAIN_TimerPeriodOver = 0;
    }
    TIMER_Off();
    
    for (int i = 0; i < BLOCK_LENGTH_SYMBOLS; i++) {
        printf("%f+%fj\n\r", symbolReadings[i].real, symbolReadings[i].imag);
    }
    
    VITERBI_Decode(symbolReadings, destBuffer);

    while (1);
        
   
//    printf("Float size is %d\n\r", sizeof(float));
//    while (1);
    uint32_t result;
    uint8_t* sendBuffer = (uint8_t*)&result;
    
    
    while (1)
    {
        while (!ADCON5Lbits.C0RDY);
        while (!ADCON5Lbits.C1RDY);
        
        ADCON3Lbits.SWCTRG = 1;
//        printf("Waiting for conversion...\n\r");
        
        while (!ADSTATLbits.AN0RDY);
        while (!ADSTATLbits.AN1RDY);
        
        result = ((uint32_t)ADCBUF0 << 16) | ADCBUF1;
        
        

//        unsigned int numBytes;
//        UART1_TRANSFER_STATUS status;
//        if (1 || !counter++) {
//            while (numBytes < 4)
//            {
//                status = UART1_TransferStatusGet();
//                if (status & UART1_TRANSFER_STATUS_TX_EMPTY) {
//                    numBytes += UART1_WriteBuffer(sendBuffer + numBytes,
//                            4 - numBytes);
//
//                }
//                // Do something else...
//            }
//            UART1_Write('\n');
//        }
//        if (!counter++)
//            printf("Result: %d\n\r", ADCBUF0);
    }
    
    
    while (1);
    
    return 0;
    
}


static void initialiseSystem(void)
{
    SYSTEM_Initialize();
    VITERBI_Init();
    SYMBOLS_Init();
    ADC_Init();
    TIMER_Init();
    DEMODULATOR_Init();
    PLL_Init();
    
    
    MAIN_TimerPeriodOver = 0;
}


static void viterbiDebug(uint8_t* destBuffer)
{
    Complex symbols[BLOCK_LENGTH_SYMBOLS] = {
        {-0.316406,-0.947754},
        {-0.947266,-0.948242},
        {-0.947754,-0.318848},
        {0.314941,0.315918},
        {0.946289,-0.947266},
        {0.315918,0.316406},
        {0.945312,-0.947754},
        {-0.946777,0.944336},
        {0.315918,-0.315918},
        {-0.316895,-0.317871},
        {-0.945801,-0.316406},
        {0.314941,0.315918},
        {0.946289,-0.946777},
        {0.314453,0.314941},
        {0.945312,-0.947754},
        {-0.946289,0.944824},
        {0.314941,-0.316895},
        {-0.315918,-0.316895},
        {-0.946289,-0.316895},
        {0.315430,0.315918},
        {0.945801,-0.947754},
        {0.314453,0.314941},
        {0.945801,-0.948242},
        {-0.946289,0.945312},
        {0.314941,-0.317383},
        {-0.316406,-0.317383},
        {-0.946777,-0.317871},
        {0.314941,0.315430},
        {0.946289,-0.947754},
        {0.314941,0.315430},
        {0.945801,-0.947754},
        {-0.946777,0.944336},
        {0.314453,-0.317383},
        {-0.316895,-0.317871},
        {-0.946777,-0.316895},
        {0.314941,0.316406},
        {0.946289,-0.947754},
        {0.316895,0.317383},
        {0.945801,-0.948242},
        {-0.945312,0.946777},
        {0.315430,-0.316406},
        {-0.315918,-0.317383},
        {-0.944824,-0.315430},
        {0.314453,0.314941},
        {0.947266,-0.946777},
        {0.314941,0.315430},
        {0.945312,-0.947754},
        {-0.945801,0.944824},
        {0.314453,-0.317871},
        {-0.316895,-0.318359},
        {-0.946777,-0.317383},
        {0.314941,0.315918},
        {0.946289,-0.947266},
        {0.314941,0.315918},
        {0.944824,-0.948730},
        {-0.946289,0.944824},
        {0.313965,-0.317871},
        {-0.316406,-0.317383},
        {-0.945801,-0.316406},
        {0.315430,0.316406},
        {0.945801,-0.947754},
        {0.315430,0.315918},
        {0.945801,-0.948730},
        {-0.946289,0.946777},
        {0.313965,-0.317871},
        {-0.315430,-0.316895},
        {-0.947266,-0.317871},
        {0.314941,0.315430},
        {0.945312,-0.947754},
        {0.316406,0.316895},
        {0.945801,-0.947754},
        {-0.946289,0.946289},
        {0.314941,-0.316895},
        {-0.315918,-0.317383},
        {-0.946777,-0.317871},
        {0.315430,0.315918},
        {0.947266,-0.947754},
        {0.313965,0.314453},
        {0.946289,-0.947266},
        {-0.946777,0.945312},
        {0.315918,-0.316406},
        {-0.316406,-0.317871},
        {-0.945312,-0.315918},
        {0.314941,0.315430},
        {0.945801,-0.947754},
        {0.314453,0.314941},
        {0.946289,-0.947754},
        {-0.945312,0.946289},
        {0.314453,-0.317383},
        {-0.315430,-0.316895},
        {-0.946289,-0.316406},
        {0.315430,0.316406},
        {0.947754,-0.946289},
        {0.315918,0.316406},
        {0.945801,-0.948242},
        {-0.945801,0.945801},
        {0.313965,-0.317871},
        {-0.315918,-0.317383},
        {-0.946777,-0.316895},
        {0.314941,0.315430},
        {0.945312,-0.947266},
        {0.314941,0.315918},
        {0.945312,-0.948730},
        {-0.946777,0.944824},
        {0.316406,-0.315430},
        {-0.316406,-0.317871},
        {-0.947266,-0.317871},
        {0.314941,0.315918},
        {0.946777,-0.947754},
        {0.314453,0.314941},
        {0.945801,-0.946777},
        {-0.945801,0.945801},
        {0.314941,-0.316895},
        {-0.315918,-0.316895},
        {-0.946289,-0.316895},
        {0.314941,0.315918},
        {0.944824,-0.947754},
        {0.314453,0.315430},
        {0.945312,-0.948730},
        {-0.945801,0.946289},
        {0.315918,-0.315918},
        {-0.316406,-0.317871},
        {-0.947266,-0.318359},
        {0.315430,0.316406},
        {0.946289,-0.946777},
        {0.315430,0.316406},
        {0.944824,-0.949219},
        {-0.946777,0.945312},
        {0.312988,-0.318359},
        {-0.316406,-0.317383},
        {-0.947754,-0.318359},
        {0.313965,0.314941},
        {0.946289,-0.948242},
        {0.316406,0.316895},
        {0.946777,-0.946777},
        {-0.946289,0.945312},
        {0.314453,-0.317383},
        {-0.316406,-0.317871},
        {-0.947754,-0.318359},
        {0.314941,0.315918},
        {0.944824,-0.947754},
        {0.314453,0.314941},
        {0.945801,-0.947754},
        {-0.946777,0.944336},
        {0.315430,-0.316406},
        {-0.316406,-0.317871},
        {-0.945801,-0.316406},
        {0.314941,0.315430},
        {0.947266,-0.947266},
        {0.315430,0.316406},
        {0.945801,-0.947266},
        {-0.946777,0.945312},
        {0.315430,-0.316406},
        {-0.316895,-0.317871},
        {-0.946777,-0.317871},
        {0.314941,0.315430},
        {0.945801,-0.948242},
        {0.314453,0.314941},
        {0.946777,-0.946777},
        {-0.945801,0.946289},
        {0.315430,-0.316406},
        {-0.316895,-0.317871},
        {-0.946777,-0.317383},
        {0.314941,0.315430},
        {0.947266,-0.948242},
        {0.314453,0.314453},
        {0.944824,-0.948730},
        {-0.945801,0.945312},
        {0.314453,-0.317383},
        {-0.315430,-0.316406},
        {-0.946289,-0.317383},
        {0.314941,0.315918},
        {0.945801,-0.947754},
        {0.314941,0.315918},
        {0.947266,-0.947754},
        {-0.946289,0.944336},
        {0.314453,-0.317871},
        {-0.316406,-0.317871},
        {-0.946777,-0.317871},
        {0.313965,0.314941},
        {0.945801,-0.947754},
        {0.314941,0.315430},
        {0.945801,-0.947754},
        {-0.946289,0.946289},
        {0.314941,-0.317383},
        {-0.316895,-0.317871},
        {-0.946777,-0.317383},
        {0.313965,0.314453},
        {0.946777,-0.946289},
        {0.315430,0.315918},
        {0.944336,-0.948730},
        {-0.945312,0.946289},
        {0.314941,-0.316895},
        {-0.316406,-0.317383},
        {-0.946777,-0.317383},
        {0.313477,0.313965},
        {0.945801,-0.947754},
        {0.315430,0.316406},
        {0.945312,-0.947754},
        {-0.945801,0.945801},
        {0.314941,-0.317383},
        {-0.314941,-0.316406},
        {-0.946289,-0.317383},
        {0.314941,0.315918},
        {0.945312,-0.947754},
        {0.315430,0.315918},
        {0.946289,-0.947266},
        {-0.945801,0.945801},
        {0.315430,-0.316895},
        {-0.316406,-0.317871},
        {-0.946289,-0.317383},
        {0.314453,0.315430},
        {0.945801,-0.946777},
        {0.315918,0.316406},
        {0.944824,-0.948730},
        {-0.945801,0.945801},
        {0.315430,-0.316406},
        {-0.315918,-0.316895},
        {-0.946777,-0.317871},
        {0.315430,0.315918},
        {0.946289,-0.947266},
        {0.314453,0.315430},
        {0.945801,-0.948242},
        {-0.945312,0.946289},
        {0.316406,-0.315430},
        {-0.316895,-0.317871},
        {-0.945801,-0.316895},
        {0.314453,0.315430},
        {0.946777,-0.946777},
        {0.315430,0.315430},
        {0.945312,-0.948242},
        {-0.945801,0.945801},
        {0.313477,-0.318359},
        {-0.316895,-0.317871},
        {-0.946289,-0.316406},
        {0.314941,0.316406},
        {0.946777,-0.947266},
        {0.315430,0.315918},
        {0.946777,-0.947266},
        {-0.945801,0.946289},
        {0.314941,-0.316895},
        {-0.316895,-0.317871},
        {-0.947754,-0.318848},
        {0.314453,0.315430},
        {0.945801,-0.947754},
        {0.315430,0.315918},
        {0.945312,-0.948242},
        {-0.945312,0.946289},
        {0.313965,-0.317871},
        {-0.315918,-0.316895},
        {-0.946777,-0.317383},
        {0.313965,0.314453},
        {0.945801,-0.948242},
        {0.315430,0.315918},
        {0.946289,-0.947754},
        {-0.949219+0.937500}
    };
    VITERBI_Decode(symbols, destBuffer);
}