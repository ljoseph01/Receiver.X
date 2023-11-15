#include "dma.h"

void DMA_Init(void)
{
    DMACONbits.DMAEN = 1;
    
    DMACH0bits.SAMODE = 0b00;
    DMACH0bits.DAMODE = 0b01;
    DMACH0bits.TRMODE = 0b01;
    DMACH0bits.SIZE = 0;
    DMACH0bits.CHEN = 1;
    
    DMAINT0bits.CHSEL = 0x28;
    
}
