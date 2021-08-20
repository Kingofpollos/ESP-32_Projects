#include "TF.h"

TF::TF() 
{
    
}

TF::~TF() 
{
    
}

void TF::setup(float a[], float b[], float x) 
{
    //Saving data
    _a = a;
    _b = b;
    _x = x;
    //Past values
    x_3 = 0.0;
    x_2 = 0.0;
    x_1 = 0.0;

    y_3 = 0.0;
    y_2 = 0.0;
    y_1 = 0.0;
    y = 0.0;
}

float TF::getValue(float x) 
{

    y = (_b[0]*x + _b[1]*x_1 + _b[2]*x_2 + _b[3]*x_3 - _a[1]*y_1 - _a[2]*y_2 - _a[3]*y_3)/_a[0];
    x_3 = x_2;
    x_2 = x_1;
    x_1 = x;
    y_3 = y_2;
    y_2 = y_1;
    y_1 = y;
    return y;
}

