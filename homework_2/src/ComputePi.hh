#ifndef COMPUTEPI_HPP
#define COMPUTEPI_HPP

#include "Series.hh"

class ComputePi:public Series
{
public:
    ComputePi();
    virtual ~ComputePi();
    double computeTerm(unsigned int k) override;
    double compute(unsigned int N) override;
    double getAnalyticPrediction();

};

#endif

