#ifndef __TF_H__
#define __TF_H__

class TF
{
    /*
    Solo acepta hasta 3er orden
    */
public:
    TF( );
    ~TF();
    void setup(float a[], float b[], float x);//Función pa meterle valores
    float getValue(float x);//Funcion para obtener valores

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

/*
TF::TF()
{
}

TF::~TF()
{
}
*/


#endif // __TF_H__