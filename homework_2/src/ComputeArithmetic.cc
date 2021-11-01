#include "ComputeArithmetic.hh"

ComputeArithmetic::ComputeArithmetic(){
};

ComputeArithmetic::~ComputeArithmetic(){};

// double ComputeArithmetic::compute(unsigned int N){
//     // double sum = 0;
//     // for (int i = 1; i <= N; i++){
//     //     sum += i;
//     // }
//     // return sum;
//     return (1.0 + N) * N / 2.0;
// }

double ComputeArithmetic::computeTerm(unsigned int k){
    return 1.0 * k;
}

double ComputeArithmetic::compute(unsigned int N){
    return Series::compute(N);
}

double ComputeArithmetic::getAnalyticPrediction(){
    return (1.0 + this->currentTerm) * this->currentTerm / 2.0;
}