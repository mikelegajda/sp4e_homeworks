#ifndef COMPUTEARITHMETIC_HPP
#define COMPUTEARITHMETIC_HPP

#include "Series.hh"

class ComputeArithmetic: public Series{
    public:
    // Constructor
    ComputeArithmetic();
    // Constructor
    virtual ~ComputeArithmetic();

    // Methods
    double computeTerm(unsigned int k) override;
    double compute(unsigned int N) override;
    double getAnalyticPrediction() override;
};
#endif