#ifndef __TF_H__
#define __TF_H__

class TF
{
public:
    TF( );
    ~TF();
    void setup(float a[], float b[], float x);/
    float getValue(float x);//Function to obtain values

private:
    /* data */
    int _order;
    float *_a;
    float *_b;
    float _x;
protected:
    float x_3;
    float x_2;
    float x_1;
    float y_3;
    float y_2;
    float y_1;
    float y;
    float _y;
};
#endif // __TF_H__
