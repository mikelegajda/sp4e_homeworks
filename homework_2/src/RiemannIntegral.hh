#ifndef REIMANNINTEGRAL_HH
#define REIMANNINTEGRAL_HH
#include "Series.hh"
#include <iomanip>


class RiemannIntegral : public Series
{
    
private:
    //Attributes
    double initial, final, deltaX;
    unsigned int maxiter;
    std::string function_type;
public:
    // Constructor
    RiemannIntegral (double initial, double final, std::string function_type);
    // Destructor
    virtual ~RiemannIntegral ();

    // Methods
    double f_cubic (double x);
    double f_cos (double x);
    double f_sin (double x);
    double delta(double initial, double final, unsigned int maxiter);
    double computeCurrentX (unsigned int index, double initial, double deltaValue);
    double computeTerm(unsigned int k) override;
    double compute(unsigned int N) override;
    double getAnalyticPrediction() override;
};

#endif