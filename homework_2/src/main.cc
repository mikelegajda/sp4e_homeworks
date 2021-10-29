#include <iostream>
#include "ComputeArithmetic.hh"

int main(int argc, char ** argv){
    ComputeArithmetic *ptr = new ComputeArithmetic();
    double ret = ptr->compute(10);
    std::cout << ret << std::endl;
}