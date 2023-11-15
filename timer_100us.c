#include "timer_100us.h"
#include "core.h"

extern uint8_t DEMODULATOR_TimerPeriodOver;
extern uint8_t MAIN_TimerPeriodOver;


void TIMER_Init(void)
{
    // Turn off the timer and disable interrupts
    T1CONbits.TON = 0;
    _T1IE = 0;
    
    // Set the configuration
    _T1IF = 0;
    _T1IP = 7;
    TMR1 = 0;
    PR1 = 9999;
    T1CONbits.TECS = 0b01;
    T1CONbits.TCS = 1;
    T1CONbits.TCKPS = 0b00;
    T1CONbits.TSYNC = 0;
    
    // Re-enable interrupts
    _T1IE = 1;
}

void TIMER_On(void)
{
    TMR1 = 0;
    _T1IF = 0;
    T1CONbits.TON = 1;
}

void TIMER_Off(void)
{
    T1CONbits.TON = 0;
    TMR1 = 0;
    _T1IF = 0;
}


void TIMER_SetHalfPeriod(void)
{
    PR1 = 4900;
}

void TIMER_SetPeriodPerc(float percentage)
{
    PR1 = (int)(9999 * percentage);
}

void TIMER_SetFullPeriod(void)
{
    PR1 = 9999;
}

/**
 * Timer1 interrupt callback
 */
void __attribute__((__interrupt__, auto_psv)) _T1Interrupt(void)
{
    MAIN_TimerPeriodOver = 1;
    DEMODULATOR_TimerPeriodOver = 1;
    _T1IF = 0;
}
