#include "viterbi.h"

static uint8_t incomingSubsets[16 * 4];
static uint8_t outgoingSubsets[16 * 4];

static float _pathMetrics1[PATH_METRICS_SIZE];
static float _pathMetrics2[PATH_METRICS_SIZE];


static float distance(Complex x, Complex y)
{
    return sqrt(pow(x.real - y.real, 2) + pow(x.imag - y.imag, 2));
}

void VITERBI_Init(void)
{
    incomingSubsets[0] = 0;
    incomingSubsets[1] = 2;
    incomingSubsets[2] = 4;
    incomingSubsets[3] = 6;
    incomingSubsets[4] = 2;
    incomingSubsets[5] = 0;
    incomingSubsets[6] = 6;
    incomingSubsets[7] = 4;
    incomingSubsets[8] = 6;
    incomingSubsets[9] = 4;
    incomingSubsets[10] = 2;
    incomingSubsets[11] = 0;
    incomingSubsets[12] = 4;
    incomingSubsets[13] = 6;
    incomingSubsets[14] = 0;
    incomingSubsets[15] = 2;
    incomingSubsets[16] = 6;
    incomingSubsets[17] = 4;
    incomingSubsets[18] = 2;
    incomingSubsets[19] = 0;
    incomingSubsets[20] = 4;
    incomingSubsets[21] = 6;
    incomingSubsets[22] = 0;
    incomingSubsets[23] = 2;
    incomingSubsets[24] = 0;
    incomingSubsets[25] = 2;
    incomingSubsets[26] = 4;
    incomingSubsets[27] = 6;
    incomingSubsets[28] = 2;
    incomingSubsets[29] = 0;
    incomingSubsets[30] = 6;
    incomingSubsets[31] = 4;
    incomingSubsets[32] = 1;
    incomingSubsets[33] = 3;
    incomingSubsets[34] = 5;
    incomingSubsets[35] = 7;
    incomingSubsets[36] = 3;
    incomingSubsets[37] = 1;
    incomingSubsets[38] = 7;
    incomingSubsets[39] = 5;
    incomingSubsets[40] = 7;
    incomingSubsets[41] = 5;
    incomingSubsets[42] = 3;
    incomingSubsets[43] = 1;
    incomingSubsets[44] = 5;
    incomingSubsets[45] = 7;
    incomingSubsets[46] = 1;
    incomingSubsets[47] = 3;
    incomingSubsets[48] = 7;
    incomingSubsets[49] = 5;
    incomingSubsets[50] = 3;
    incomingSubsets[51] = 1;
    incomingSubsets[52] = 5;
    incomingSubsets[53] = 7;
    incomingSubsets[54] = 1;
    incomingSubsets[55] = 3;
    incomingSubsets[56] = 1;
    incomingSubsets[57] = 3;
    incomingSubsets[58] = 5;
    incomingSubsets[59] = 7;
    incomingSubsets[60] = 3;
    incomingSubsets[61] = 1;
    incomingSubsets[62] = 7;
    incomingSubsets[63] = 5;
    outgoingSubsets[0] = 0;
    outgoingSubsets[1] = 2;
    outgoingSubsets[2] = 6;
    outgoingSubsets[3] = 4;
    outgoingSubsets[4] = 6;
    outgoingSubsets[5] = 4;
    outgoingSubsets[6] = 0;
    outgoingSubsets[7] = 2;
    outgoingSubsets[8] = 1;
    outgoingSubsets[9] = 3;
    outgoingSubsets[10] = 7;
    outgoingSubsets[11] = 5;
    outgoingSubsets[12] = 7;
    outgoingSubsets[13] = 5;
    outgoingSubsets[14] = 1;
    outgoingSubsets[15] = 3;
    outgoingSubsets[16] = 2;
    outgoingSubsets[17] = 0;
    outgoingSubsets[18] = 4;
    outgoingSubsets[19] = 6;
    outgoingSubsets[20] = 4;
    outgoingSubsets[21] = 6;
    outgoingSubsets[22] = 2;
    outgoingSubsets[23] = 0;
    outgoingSubsets[24] = 3;
    outgoingSubsets[25] = 1;
    outgoingSubsets[26] = 5;
    outgoingSubsets[27] = 7;
    outgoingSubsets[28] = 5;
    outgoingSubsets[29] = 7;
    outgoingSubsets[30] = 3;
    outgoingSubsets[31] = 1;
    outgoingSubsets[32] = 4;
    outgoingSubsets[33] = 6;
    outgoingSubsets[34] = 2;
    outgoingSubsets[35] = 0;
    outgoingSubsets[36] = 2;
    outgoingSubsets[37] = 0;
    outgoingSubsets[38] = 4;
    outgoingSubsets[39] = 6;
    outgoingSubsets[40] = 5;
    outgoingSubsets[41] = 7;
    outgoingSubsets[42] = 3;
    outgoingSubsets[43] = 1;
    outgoingSubsets[44] = 3;
    outgoingSubsets[45] = 1;
    outgoingSubsets[46] = 5;
    outgoingSubsets[47] = 7;
    outgoingSubsets[48] = 6;
    outgoingSubsets[49] = 4;
    outgoingSubsets[50] = 0;
    outgoingSubsets[51] = 2;
    outgoingSubsets[52] = 0;
    outgoingSubsets[53] = 2;
    outgoingSubsets[54] = 6;
    outgoingSubsets[55] = 4;
    outgoingSubsets[56] = 7;
    outgoingSubsets[57] = 5;
    outgoingSubsets[58] = 1;
    outgoingSubsets[59] = 3;
    outgoingSubsets[60] = 1;
    outgoingSubsets[61] = 3;
    outgoingSubsets[62] = 7;
    outgoingSubsets[63] = 5;
}

void VITERBI_Decode(Complex* source, uint8_t* dest)
{
    float branchMetrics[BRANCH_METRICS_SIZE];
    uint8_t incomingBits[INCOMING_BITS_SIZE];
    uint8_t prevStates[PREV_STATES_SIZE];
    
    float distances[16];
    
    float *currPathMetrics = _pathMetrics1;
    float *prevPathMetrics = _pathMetrics2;
    float *temp;
    
    // Initialise memories
    for (int i = 0; i < BRANCH_METRICS_SIZE; i++)
        branchMetrics[i] = 0.0f;
    
    prevPathMetrics[0] = 0;
    for (int i = 1; i < PATH_METRICS_SIZE; i++)
        prevPathMetrics[i] = INFINITY;
    
    // These arrays are the same size even though there are two macros. 
    for (int i = 0; i < INCOMING_BITS_SIZE; i++) {
        incomingBits[i] = 0;
        prevStates[i] = 0;
    }
    
    
    for (int n = 0; n < BLOCK_LENGTH_SYMBOLS; n++) {
        for (int signal = 0; signal < 16; signal++) {
            distances[signal] = distance(source[n], SYMBOLS_Get16QAM(signal));
        }
        for (uint8_t currentState = 0; currentState < 16; currentState++) {
            for (uint8_t incomingStateIter = 0; incomingStateIter < 4; incomingStateIter++) {
                for (uint8_t incomingSignalIter = 0; incomingSignalIter < 2; incomingSignalIter++) {
                    uint8_t incomingSubset = incomingSubsets[currentState*4 + incomingStateIter];
                    uint8_t incomingSignal = (incomingSignalIter << 3) | incomingSubset;
//                    float dist = distance(source[n], SYMBOLS_Get16QAM(incomingSignal));
//                    branchMetrics[currentState*4*2 + incomingStateIter*2 + incomingSignalIter] = dist;
//                    branchMetrics[incomingStateIter*2 + incomingSignalIter] = dist;
                    branchMetrics[incomingStateIter*2 + incomingSignalIter] = distances[incomingSignal];
                }
            }
                
            // Add compare select path metrics
            float min_pm = INFINITY;
            for (int incomingStateIter = 0; incomingStateIter < 4; incomingStateIter++) {
                // Find state number of the "incomingStateIter'th" state
                uint8_t incomingStateInt = (currentState >> 2) | (incomingStateIter << 2);

                for (int incomingSignalIter = 0; incomingSignalIter < 2; incomingSignalIter++) {
                    float pm = (prevPathMetrics[incomingStateInt]
//                          + branchMetrics[currentState*4*2 + incomingStateIter*2 + incomingSignalIter]);
                          + branchMetrics[incomingStateIter*2 + incomingSignalIter]);
                    if (pm < min_pm) {
                        min_pm = pm;
                        prevStates[n*16 + currentState] = incomingStateInt;
                        // Incoming message bits is the signal selector bit concatenated with the lsbs of current state
                        incomingBits[n*16 + currentState] = (incomingSignalIter << 2) | (currentState & 0b11);
                    }
                }
            }
            currPathMetrics[currentState] = min_pm;
        }
        
        // Swap path metric arrays 
        temp = currPathMetrics;
        currPathMetrics = prevPathMetrics;
        prevPathMetrics = temp;
    }
    
    // Back propagation time
    // First find min end node (from previousPathMetrics because they've been swapped)
    uint8_t node = 0;
    for (int i = 1; i < 16; i++) {
        if (prevPathMetrics[i] < prevPathMetrics[node]) {
            node = i;
        }
    }
    
    // Must loop in groups of three. See lab book page 53 for more details. 
    uint8_t currBits;
    currBits = 10; // Just for breakpoint
    for (int destIter = BLOCK_LENGTH_BYTES - 1, 
            columnIter = BLOCK_LENGTH_SYMBOLS - 1; 
            destIter >= 2 /*, columnIter >= 7*/; // columnIter check is unnecessary, comment is for clarity
            destIter -= 3, columnIter -= 8) {
        
        // 1st symbol (columnIter)
        currBits = incomingBits[columnIter*16 + node];
        dest[destIter] = currBits;
        node = prevStates[columnIter*16 + node];
        
        // 2nd symbol (columnIter - 1)
        currBits = incomingBits[(columnIter - 1)*16 + node];
        dest[destIter] |= currBits << 3;
        node = prevStates[(columnIter - 1)*16 + node];
        
        // 3rd symbol (columnIter - 2)
        currBits = incomingBits[(columnIter - 2)*16 + node];
        dest[destIter] |= (currBits << 6) & 0xFF;
        dest[destIter - 1] = currBits >> 2;
        node = prevStates[(columnIter - 2)*16 + node];
        
        // 4th symbol (columnIter - 3)
        currBits = incomingBits[(columnIter - 3)*16 + node];
        dest[destIter - 1] |= currBits << 1;
        node = prevStates[(columnIter - 3)*16 + node];
        
        // 5th symbol (columnIter - 4)
        currBits = incomingBits[(columnIter - 4)*16 + node];
        dest[destIter - 1] |= currBits << 4;
        node = prevStates[(columnIter - 4)*16 + node];
        
        // 6th symbol (columnIter - 5)
        currBits = incomingBits[(columnIter - 5)*16 + node];
        dest[destIter - 1] |= (currBits << 7) & 0x80;
        dest[destIter - 2] = currBits >> 1;
        node = prevStates[(columnIter - 5)*16 + node];
        
        // 7th symbol (columnIter - 6)
        currBits = incomingBits[(columnIter - 6)*16 + node];
        dest[destIter - 2] |= currBits << 2;
        node = prevStates[(columnIter - 6)*16 + node];
        
        // 8th symbol (columnIter - 7)
        currBits = incomingBits[(columnIter - 7)*16 + node];
        dest[destIter - 2] |= currBits << 5;
        node = prevStates[(columnIter - 7)*16 + node];
    }
}
