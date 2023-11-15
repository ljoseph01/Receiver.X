
#ifndef VITERBI_H
#define	VITERBI_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <math.h>
#include "complex.h"
#include "core.h"
#include "symbols.h"

//#define BRANCH_METRICS_SIZE 128
#define BRANCH_METRICS_SIZE 8
#define PATH_METRICS_SIZE 16
// NB!! Must be changed when block length changes.
// Should be 16 * BLOCK_LENGTH_SYMBOLS
#define INCOMING_BITS_SIZE 4096
#define PREV_STATES_SIZE 4096

/**
 * Initialises the appropriate hard-coded transition arrays. Must be called 
 * before VITERBI_Decode(), but only needs to be called once in the program's
 * life cycle.
 */
void VITERBI_Init(void);


/**
 * Perform the Viterbi algorithm on the given set of symbols. Assumes 
 * the encoder generated with the VITERBI_Init function. Transitions are changed
 * manually only. SYBMOLS_Init() must be called before this works.
 * 
 * @param source Array of symbols at complex baseband for the algorithm to be 
 * performed on. Assumed to be of length BLOCK_LENGTH_SYMBOLS.
 * @param dest Destination buffer. The resultant data are stored in this buffer.
 * Note that this does not form a sequence of bits, but rather of bytes. Hence 
 * the length is assumed to be BLOCK_LENGTH_BYTES which must be divisible by 3.
 * Must be initialised to zeroes.
 * 
 * @note BLOCK_LENGTH_xxx macros are defined in core.h
 */
void VITERBI_Decode(Complex* source, uint8_t* dest);


#endif	/* XC_HEADER_TEMPLATE_H */

