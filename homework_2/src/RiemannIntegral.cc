#include "RiemannIntegral.hh"
#include <cmath>

RiemannIntegral::RiemannIntegral(double init, double fin, std::string f_type) : initial(init), final(fin), function_type(f_type){}

RiemannIntegral::~RiemannIntegral() {};

double RiemannIntegral::f_cubic (double x){
    return x*x*x;
}

double RiemannIntegral::f_cos (double x){
    return cos(x);
}

double RiemannIntegral::f_sin (double x){
    return sin(x);
}

double RiemannIntegral::delta(double initial, double final, unsigned int maxiter){
    return (final-initial) * 1.0 /maxiter;
}

double RiemannIntegral::computeCurrentX (unsigned int index, double initial, double deltaValue){
    return initial + index*deltaValue;
}

double RiemannIntegral::computeTerm(unsigned int k){
    if (function_type == "cubic"){
        return f_cubic(computeCurrentX(k, this->initial, this->deltaX)) * deltaX;
    } 
    else if (function_type == "cos"){
        return f_cos(computeCurrentX(k, this->initial, this->deltaX)) * deltaX;
    }else {
        return f_sin(computeCurrentX(k, this->initial, this->deltaX)) * deltaX;
    }
};

double RiemannIntegral::getAnalyticPrediction(){
    return nan("");
}

double RiemannIntegral::compute(unsigned int N){
    this->maxiter = N;
    this->deltaX = this->delta(initial, final, maxiter);
    return Series::compute(N);
};





