#include "ComputePi.hh"
#include "math.h"

ComputePi::ComputePi(/* args */)
{
}

ComputePi::~ComputePi()
{
}

double ComputePi::computeTerm(unsigned int k){
    return 1.0 / (1.0 * k * k);
}

double ComputePi::compute(unsigned int N){
    Series::compute(N);
    return sqrt(6.0 * this->currentValue);
}

double ComputePi::getAnalyticPrediction(){
    return M_PI;
}
