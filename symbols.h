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
#ifndef SYMBOLS_H
#define	SYMBOLS_H

#include <xc.h> // include processor files - each processor file is guarded.  

#include "complex.h"


/**
 * Initialise the symbols. Should be called before attempting to retrieve any 
 * symbols with SYMBOLS_Getxxx()
 */
void SYMBOLS_Init();

/**
 * Get the complex symbol associated with the given bits 
 * in the 8PSK constellation
 * 
 * @param bits: integer representation of the bits to be mapped to the symbol
 * @return Complex normalised symbol 
 */
Complex SYMBOLS_Get8PSK(int bits);

/**
 * Get the complex symbol associated with the given bits 
 * in the 16QAM constellation
 * 
 * @param bits: integer representation of the bits to be mapped to the symbol
 * @return Complex normalised symbol 
 */
Complex SYMBOLS_Get16QAM(int bits);

/**
 * Get the complex symbol associated with the given bits 
 * in the 16QAM constellation for uncoded transmission (grey code)
 * 
 * @param bits: integer representation of the bits to be mapped to the symbol
 * @return Complex normalised symbol 
 */
Complex SYMBOLS_Get16QAMUncoded(int bits);


/**
 * Map given symbol to bits
 * 
 * @param symbol input symbol
 * @return bits as int
 */
int SYMBOLS_GetBits16QAM(Complex symbol);

/**
 * Map given symbol to bits (uncoded 16QAM)
 * 
 * @param symbol input symbol
 * @return bits as int
 */
int SYMBOLS_GetBits16QAMUncoded(Complex symbol);

/**
 * Map given symbol to bits
 * 
 * @param symbol input symbol
 * @return bits as int
 */
int SYMBOLS_GetBits8PSK(Complex symbol);

#endif

