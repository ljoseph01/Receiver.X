#include "pll.h"

static float phaseError;
static float amplitudeError;
static float cummulativePhaseError;
static float frequency;
static float coarsePhaseError;


static Complex tempFirst;

static const float alphaPhase = 0.132;
static const float alphaAmplitude = 0.132;

static float PILOT_16QAM_MAG;

static float getError8PSK(Complex input, Complex* estimatedSymbol);
static Complex estimate16QAM(Complex input);

static double _8PSK_AngleBounds[8];

void PLL_Init()
{
    _8PSK_AngleBounds[0] = -7 * M_PI / 8;
    _8PSK_AngleBounds[1] = -5 * M_PI / 8;
    _8PSK_AngleBounds[2] = -3 * M_PI / 8;
    _8PSK_AngleBounds[3] = -1 * M_PI / 8;
    _8PSK_AngleBounds[4] =  1 * M_PI / 8;
    _8PSK_AngleBounds[5] =  3 * M_PI / 8;
    _8PSK_AngleBounds[6] =  5 * M_PI / 8;
    _8PSK_AngleBounds[7] =  7 * M_PI / 8;
    
    PILOT_16QAM_MAG = COMPLEX_Magnitude(PILOT_SYMBOL_16QAM);
}


void PLL_Reset()
{
    phaseError = 0.0f;
    cummulativePhaseError = 0.0f;
    frequency = 0.0f;
    coarsePhaseError = 0.0f;
    amplitudeError = 1.0f;
}

void PLL_First8PSK(Complex input) 
{
    coarsePhaseError = atan2(input.imag, input.real);
    
    phaseError = 0.0f;
}

bool PLL_Pilot8PSK(Complex input)
{
    float dPhaseError = atan2(input.imag, input.real) - coarsePhaseError - phaseError;
    while (dPhaseError > M_PI) dPhaseError -= 2 * M_PI;
    while (dPhaseError < -M_PI) dPhaseError += 2 * M_PI;
    
//    float dAmplitudeError = 1.0f / COMPLEX_Magnitude(input) - amplitudeError;
    
    if ((dPhaseError > M_PI / 2.0f) || (dPhaseError < -M_PI / 2.0f)) {
        int i = 0; // Breakpoint
        return true;
    }
    phaseError += alphaPhase * dPhaseError;
//    amplitudeError += alphaAmplitude * dAmplitudeError;
    return false;
}

Complex PLL_8PSK(Complex input, Complex* outputPtr)
{
    *outputPtr = COMPLEX_Rotate(input, -coarsePhaseError - phaseError);
    Complex estimatedSymbol;
    float dPhaseError = getError8PSK(*outputPtr, &estimatedSymbol);
//    float dAmplitudeError = 1.0f / COMPLEX_Magnitude(*outputPtr);

    phaseError += alphaPhase * dPhaseError;
//    amplitudeError += alphaAmplitude * dAmplitudeError;
    
//    *outputPtr = COMPLEX_Rotate(input, -phase);
    int i = 0; // breakpoint
    return estimatedSymbol;
}

void PLL_First16QAM(Complex input)
{
    tempFirst = input;
    coarsePhaseError = atan2(input.imag, input.real) - M_PI / 4;
    phaseError = 0.0f;
    amplitudeError = PILOT_16QAM_MAG / COMPLEX_Magnitude(input);
}

bool PLL_Pilot16QAM(Complex input)
{
    int i; // Breakpoint
    float dPhaseError = atan2(input.imag, input.real) - coarsePhaseError - M_PI / 4 - phaseError;
    while (dPhaseError > M_PI) dPhaseError -= 2 * M_PI;
    while (dPhaseError < -M_PI) dPhaseError += 2 * M_PI;
    
    if ((dPhaseError > M_PI / 2.0f) || (dPhaseError < -M_PI / 2.0f)) {
        i = 0; // Breakpoint
        return true;
    }
    
//    float dAmplitudeError = PILOT_16AM_MAG / COMPLEX_Magnitude(input) - amplitudeError;
    phaseError += alphaPhase * dPhaseError;
//    amplitudeError += alphaAmplitude * dAmplitudeError;
    return false;
}

Complex PLL_16QAM(Complex input, Complex* outputPtr)
{
    *outputPtr = COMPLEX_RotateScale(input, -coarsePhaseError - phaseError, amplitudeError);
    Complex estimatedSymbol;

    estimatedSymbol = estimate16QAM(*outputPtr);
    float dPhaseError = atan2(outputPtr->imag, outputPtr->real)
            - atan2(estimatedSymbol.imag, estimatedSymbol.real);
    while (dPhaseError > M_PI) dPhaseError -= 2 * M_PI;
    while (dPhaseError < -M_PI) dPhaseError += 2 * M_PI;
    
    float dAmplitudeError = COMPLEX_Magnitude(estimatedSymbol) / COMPLEX_Magnitude(*outputPtr) - 1.0f;
    
    phaseError += alphaPhase * dPhaseError;
    amplitudeError += alphaAmplitude * dAmplitudeError;
    
    int i = 0; // for breakpoint, i is not used
    return estimatedSymbol;
}

static float getError8PSK(Complex input, Complex* estimatedSymbol)
{
    float inputAngle = atan2(input.imag, input.real);
    
    if (inputAngle >= _8PSK_AngleBounds[0] && inputAngle < _8PSK_AngleBounds[1]) {
        *estimatedSymbol = (Complex){-ONE_OVER_ROOT2, -ONE_OVER_ROOT2};
        return inputAngle + 3 * M_PI / 4;
    }
    if (inputAngle >= _8PSK_AngleBounds[1] && inputAngle < _8PSK_AngleBounds[2]) {
        *estimatedSymbol = (Complex){0.0f, -1.0f};
        return inputAngle + M_PI / 2;
    }
    if (inputAngle >= _8PSK_AngleBounds[2] && inputAngle < _8PSK_AngleBounds[3]) {
        *estimatedSymbol = (Complex){ONE_OVER_ROOT2, -ONE_OVER_ROOT2};
        return inputAngle + M_PI / 4;
    }
    if (inputAngle >= _8PSK_AngleBounds[3] && inputAngle < _8PSK_AngleBounds[4]) {
        *estimatedSymbol = (Complex){1.0f, 0.0f};
        return inputAngle;
    }
    if (inputAngle >= _8PSK_AngleBounds[4] && inputAngle < _8PSK_AngleBounds[5]) {
        *estimatedSymbol = (Complex){ONE_OVER_ROOT2, ONE_OVER_ROOT2};
        return inputAngle - M_PI / 4;
    }
    if (inputAngle >= _8PSK_AngleBounds[5] && inputAngle < _8PSK_AngleBounds[6]) {
        *estimatedSymbol = (Complex){0.0f, 1.0f};
        return inputAngle - M_PI / 2;
    }
    if (inputAngle >= _8PSK_AngleBounds[6] && inputAngle < _8PSK_AngleBounds[7]) {
        *estimatedSymbol = (Complex){-ONE_OVER_ROOT2, ONE_OVER_ROOT2};
        return inputAngle - 3 * M_PI / 4;
    }
        
    *estimatedSymbol = (Complex){-1.0f, 0.0f};
    return inputAngle > 0 ? M_PI - inputAngle : M_PI + inputAngle;
}

static Complex estimate16QAM(Complex input)
{
    float real, imag;
    
    if (input.real < -TWO_OVER_ROOT10) {
        real = -THREE_OVER_ROOT10;
    }
    else if (input.real < 0.0f) {
        real = -ONE_OVER_ROOT10;
    }
    else if (input.real < TWO_OVER_ROOT10) { 
        real = ONE_OVER_ROOT10;
    }
    else {
        real = THREE_OVER_ROOT10;
    }
    
    if (input.imag < -TWO_OVER_ROOT10) {
        imag = -THREE_OVER_ROOT10;
    }
    else if (input.imag < 0.0f) {
        imag = -ONE_OVER_ROOT10;
    }
    else if (input.imag < TWO_OVER_ROOT10) { 
        imag = ONE_OVER_ROOT10;
    }
    else {
        imag = THREE_OVER_ROOT10;
    }
    
    return (Complex){real, imag};
}
