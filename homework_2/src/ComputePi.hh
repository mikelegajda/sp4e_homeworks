#ifndef COMPUTEPI_HPP
#define COMPUTEPI_HPP

#include "Series.hh"

class ComputePi:public Series
{
public:
    // Constructor
    ComputePi();
    // Destructor
    virtual ~ComputePi();

    // Methods
    double computeTerm(unsigned int k) override;
    double compute(unsigned int N) override;
    double getAnalyticPrediction() override;
};

#endif

