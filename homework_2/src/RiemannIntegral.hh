#ifndef REIMANNINTEGRAL_HH
#define REIMANNINTEGRAL_HH
#include "Series.hh"
#include <iomanip>


class RiemannIntegral : public Series
{
    
private:
    //Attributes
    int initial, final, maxiter;
    std::string function_type;
public:
    // Constructor
    RiemannIntegral (int initial, int final, int maxiter, std::string function_type);
    // Destructor
    virtual ~RiemannIntegral ();

    // Methods
    double f_cubic (double x);
    double f_cos (double x);
    double f_sin (double x);
    double computeTerm(unsigned int k) override;
    double compute(unsigned int N) override;
    double getAnalyticPrediction() override;
    double computeIntegral (int, int, int, std::string);
};

#endif