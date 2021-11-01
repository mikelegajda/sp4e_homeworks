#ifndef REIMANNINTEGRAL_HH
#define REIMANNINTEGRAL_HH
#include "Series.hh"
#include <iomanip>


class RiemannIntegral : public Series
{
    int initial, final; 
    std::string function_type;
private:
    /* data */
public:
    RiemannIntegral ();
    ~RiemannIntegral ();

    /*Functions*/
    double f_cubic (double x);
    double f_cos (double x);
    double f_sin (double x);
    double computeTerm(unsigned int k) override;

    double computeIntegral (int, int, std::string);
};

RiemannIntegral ::RiemannIntegral()
{
}

RiemannIntegral ::~RiemannIntegral()
{
}


#endif