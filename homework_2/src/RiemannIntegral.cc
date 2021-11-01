#include "RiemannIntegral.hh"
#include <cmath>

RiemannIntegral::RiemannIntegral(){
};


double f_cubic (double x){
    return x*x*x;
}

double f_cos (double x){
    return cos(x);
}

double f_sin (double x){
    return sin(x);
}

double delta(int initial, int final, int maxiter){
    return (final-initial)/maxiter;
}

double computeCurrentX (int index, int initial, double deltaValue){
    return initial + index*deltaValue;
}

double RiemannIntegral::computeTerm(unsigned int k){

};





