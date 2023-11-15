#include "complex.h"

double COMPLEX_Magnitude(Complex x)
{
    return sqrt(x.real*x.real + x.imag*x.imag);
}

Complex COMPLEX_Rotate(Complex x, float theta)
{
    return (Complex){x.real * cos(theta) - x.imag * sin(theta), 
            x.imag * cos(theta)+ x.real * sin(theta)};
}

Complex COMPLEX_RotateScale(Complex x, float theta, float scale)
{
    return (Complex){scale * (x.real * cos(theta) - x.imag * sin(theta)), 
            scale * (x.imag * cos(theta)+ x.real * sin(theta))};
}

bool COMPLEX_Equals(Complex x, Complex y)
{
    return x.real == y.real && x.imag == y.imag;
}