#ifndef CORE_H
#define	CORE_H

#include <xc.h> // include processor files - each processor file is guarded.  

// NB!! If these change then viterbi.h needs changes too
// Nominally working off the idea of a 1 fps GIF at 200x150 resolution monochromatic
// 256 symbols per block
#define BLOCK_LENGTH_BITS 768
#define BLOCK_LENGTH_BYTES 96
#define BLOCK_LENGTH_SYMBOLS 256

#define ONE_OVER_ROOT2 (1.0f / sqrt(2))
#define ONE_OVER_ROOT10 (1.0f / sqrt(10))
#define TWO_OVER_ROOT10 (2.0f / sqrt(10))
#define THREE_OVER_ROOT10 (3.0f / sqrt(10))

#define PILOT_LENGTH 10

#define PILOT_SYMBOL_16QAM (Complex) {THREE_OVER_ROOT10, THREE_OVER_ROOT10}
#define PILOT_SYMBOL_8PSK (Complex) {1.0f, 0.0f}

typedef enum {
    CODED_16QAM,
    UNCODED_16QAM,
    UNCODED_8PSK
} DataMode;


#endif	/* XC_HEADER_TEMPLATE_H */

