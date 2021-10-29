#include "ComputeArithmetic.hh"

ComputeArithmetic::ComputeArithmetic(){
};

ComputeArithmetic::~ComputeArithmetic(){};

double ComputeArithmetic::compute(unsigned int N){
    // double sum = 0;
    // for (int i = 1; i <= N; i++){
    //     sum += i;
    // }
    // return sum;
    return (1.0 + N) * N / 2.0;
}