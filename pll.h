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
#ifndef PLL_H
#define	PLL_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <math.h>
#include <stdbool.h>
#include "core.h"
#include "complex.h"
#include "symbols.h"


/**
 * Initialise the PLL
 */
void PLL_Init(void);

/**
 * Reset the PLL's internal phase and frequency trackers.
 */
void PLL_Reset(void);

/**
 * PLL iteration for first received symbol (coarse phase estimate)
 * @param input measured input
 */
void PLL_First8PSK(Complex input);

/**
 * PLL iteration during pilot sequence
 * @param input measured input
 * @return false normally, true if last pilot symbol (negative of usual) is detected.
 */
bool PLL_Pilot8PSK(Complex input);

/**
 * Perform one iteration of the PLL. 
 * 
 * @param input Complex symbol reading
 * @param outputPtr pointer to the output 
 * @return the estimated symbol
 */
Complex PLL_8PSK(Complex input, Complex* outputPtr);

/**
 * PLL iteration for first received symbol (coarse phase estimate)
 * @param input measured input
 */
void PLL_First16QAM(Complex input);

/**
 * PLL iteration during pilot sequence
 * @param input measured input
 * @return false normally, true if last pilot symbol (negative of usual) is detected.
 */
bool PLL_Pilot16QAM(Complex input);

/**
 * Perform one iteration of the PLL. 
 * 
 * @param input Complex symbol reading
 * @param outputPtr pointer to the output 
 * @return the estimated symbol
 */
Complex PLL_16QAM(Complex input, Complex* outputPtr);

#endif	/* XC_HEADER_TEMPLATE_H */

