/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef TIMER_10US_H
#define	TIMER_10US_H

#include <xc.h> // include processor files - each processor file is guarded.  


/**
 * Initialise Timer1 with a 1us period and enable the appropriate interrupts.
 * 
 * Note: this explicitly stops the timer. Call TIMER_on() if you want it to start.
 */
void TIMER_Init(void);


/**
 * Clear and start the timer.
 */
void TIMER_On(void);


/**
 * Stop and clear the timer.
 */
void TIMER_Off(void);


/**
 * Set the timer period to 50 us. Should be called after TIMER_Off().
 */
void TIMER_SetHalfPeriod(void);

/**
 * Set the timer period to a specified percentage of 100 us
 * @param percentage [0, 1] percentage to set the period
 */
void TIMER_SetPeriodPerc(float percentage);

/**
 * Set the timer period to 100 us. Should be called after TIMER_Off().
 */
void TIMER_SetFullPeriod(void);



#endif	/* XC_HEADER_TEMPLATE_H */

