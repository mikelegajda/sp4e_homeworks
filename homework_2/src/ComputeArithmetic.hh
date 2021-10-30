#ifndef COMPUTEARITHMETIC_HPP
#define COMPUTEARITHMETIC_HPP

#include "Series.hh"

class ComputeArithmetic: public Series{
    public:
    ComputeArithmetic();
    virtual ~ComputeArithmetic();
    double computeTerm(unsigned int k) override;
    double compute(unsigned int N) override;
};
#endif