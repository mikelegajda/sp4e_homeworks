#include "RiemannIntegral.hh"
#include <cmath>

RiemannIntegral::RiemannIntegral(int init, int fin, int maxi, std::string f_type) : initial(init), final(fin), maxiter(maxi), function_type(f_type){}

RiemannIntegral::~RiemannIntegral() {};

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

double RiemannIntegral::computeTerm(unsigned int x){
    if (function_type == "cubic"){
        double first = f_cubic(x);
        double second = delta(initial, final, maxiter);
        return first*second;
    } else if (function_type == "cos"){
        double first = f_cos(x);
        double second = delta(initial, final, maxiter);
        return first*second;
    }else {
        double first = f_sin(x);
        double second = delta(initial, final, maxiter);
        return first*second;
    }
};





