/*
 * File:   symbols.c
 * Author: User
 *
 * Created on 31 July 2023, 12:37 PM
 */
#include <math.h>

#include "xc.h"
#include "complex.h"
#include "symbols.h"
#include "core.h"

Complex _8PSK[8];
Complex _16QAM[16];
Complex _16QAM_UNCODED[16];

void SYMBOLS_Init(void)
{
    _8PSK[0] = (Complex){-ONE_OVER_ROOT2, -ONE_OVER_ROOT2};       // 0 000
    _8PSK[1] = (Complex){          -1.0f,            0.0f};       // 1 001
    _8PSK[2] = (Complex){           0.0f,            1.0f};       // 2 010
    _8PSK[3] = (Complex){-ONE_OVER_ROOT2,  ONE_OVER_ROOT2};       // 3 011
    _8PSK[4] = (Complex){           0.0f,           -1.0f};       // 4 100
    _8PSK[5] = (Complex){ ONE_OVER_ROOT2, -ONE_OVER_ROOT2};       // 5 101
    _8PSK[6] = (Complex){ ONE_OVER_ROOT2,  ONE_OVER_ROOT2};       // 6 110
    _8PSK[7] = (Complex){           1.0f,            0.0f};       // 7 111

    
    _16QAM[0]  = (Complex){   ONE_OVER_ROOT10,   -ONE_OVER_ROOT10};  // 0 0000
    _16QAM[1]  = (Complex){  -ONE_OVER_ROOT10,   -ONE_OVER_ROOT10};  // 1 0001
    _16QAM[2]  = (Complex){ THREE_OVER_ROOT10, -THREE_OVER_ROOT10};  // 2 0010
    _16QAM[3]  = (Complex){-THREE_OVER_ROOT10, -THREE_OVER_ROOT10};  // 3 0011
    _16QAM[4]  = (Complex){-THREE_OVER_ROOT10,   -ONE_OVER_ROOT10};  // 4 0100
    _16QAM[5]  = (Complex){ THREE_OVER_ROOT10,   -ONE_OVER_ROOT10};  // 5 0101
    _16QAM[6]  = (Complex){  -ONE_OVER_ROOT10, -THREE_OVER_ROOT10};  // 6 0110
    _16QAM[7]  = (Complex){   ONE_OVER_ROOT10, -THREE_OVER_ROOT10};  // 7 0111
    _16QAM[8]  = (Complex){-THREE_OVER_ROOT10,  THREE_OVER_ROOT10};  // 8 1000
    _16QAM[9]  = (Complex){ THREE_OVER_ROOT10,  THREE_OVER_ROOT10};  // 9 1001
    _16QAM[10] = (Complex){  -ONE_OVER_ROOT10,    ONE_OVER_ROOT10};  // A 1010
    _16QAM[11] = (Complex){   ONE_OVER_ROOT10,    ONE_OVER_ROOT10};  // B 1011
    _16QAM[12] = (Complex){   ONE_OVER_ROOT10,  THREE_OVER_ROOT10};  // C 1100
    _16QAM[13] = (Complex){  -ONE_OVER_ROOT10,  THREE_OVER_ROOT10};  // D 1101
    _16QAM[14] = (Complex){ THREE_OVER_ROOT10,    ONE_OVER_ROOT10};  // E 1110
    _16QAM[15] = (Complex){-THREE_OVER_ROOT10,    ONE_OVER_ROOT10};  // F 1111
    
    _16QAM_UNCODED[0]  = (Complex){-THREE_OVER_ROOT10, -THREE_OVER_ROOT10};  // 0 0000
    _16QAM_UNCODED[1]  = (Complex){-THREE_OVER_ROOT10,   -ONE_OVER_ROOT10};  // 1 0001
    _16QAM_UNCODED[2]  = (Complex){-THREE_OVER_ROOT10,  THREE_OVER_ROOT10};  // 2 0010
    _16QAM_UNCODED[3]  = (Complex){-THREE_OVER_ROOT10,    ONE_OVER_ROOT10};  // 3 0011
    _16QAM_UNCODED[4]  = (Complex){  -ONE_OVER_ROOT10, -THREE_OVER_ROOT10};  // 4 0100
    _16QAM_UNCODED[5]  = (Complex){  -ONE_OVER_ROOT10,   -ONE_OVER_ROOT10};  // 5 0101
    _16QAM_UNCODED[6]  = (Complex){  -ONE_OVER_ROOT10,  THREE_OVER_ROOT10};  // 6 0110
    _16QAM_UNCODED[7]  = (Complex){  -ONE_OVER_ROOT10,    ONE_OVER_ROOT10};  // 7 0111
    _16QAM_UNCODED[8]  = (Complex){ THREE_OVER_ROOT10, -THREE_OVER_ROOT10};  // 8 1000
    _16QAM_UNCODED[9]  = (Complex){ THREE_OVER_ROOT10,   -ONE_OVER_ROOT10};  // 9 1001
    _16QAM_UNCODED[10] = (Complex){ THREE_OVER_ROOT10,  THREE_OVER_ROOT10};  // A 1010
    _16QAM_UNCODED[11] = (Complex){ THREE_OVER_ROOT10,    ONE_OVER_ROOT10};  // B 1011
    _16QAM_UNCODED[12] = (Complex){   ONE_OVER_ROOT10, -THREE_OVER_ROOT10};  // C 1100
    _16QAM_UNCODED[13] = (Complex){   ONE_OVER_ROOT10,   -ONE_OVER_ROOT10};  // D 1101
    _16QAM_UNCODED[14] = (Complex){   ONE_OVER_ROOT10,  THREE_OVER_ROOT10};  // E 1110
    _16QAM_UNCODED[15] = (Complex){   ONE_OVER_ROOT10,    ONE_OVER_ROOT10};  // F 1111
}

Complex SYMBOLS_Get8PSK(int bits)
{
    return _8PSK[bits];
}

Complex SYMBOLS_Get16QAM(int bits)
{
    return _16QAM[bits];
}

Complex SYMBOLS_Get16QAMUncoded(int bits)
{
    return _16QAM_UNCODED[bits];
}

int SYMBOLS_GetBits16QAM(Complex symbol)
{
    for (int i = 0; i < 16; i++)
        if (COMPLEX_Equals(_16QAM[i], symbol)) return i;
    
    return -1;
}

int SYMBOLS_GetBits16QAMUncoded(Complex symbol)
{
    for (int i = 0; i < 16; i++)
        if (COMPLEX_Equals(_16QAM_UNCODED[i], symbol)) return i;
    
    return -1;
}

int SYMBOLS_GetBits8PSK(Complex symbol)
{
    for (int i = 0; i < 8; i++)
        if (COMPLEX_Equals(_8PSK[i], symbol)) return i;
    
    return -1;
}

