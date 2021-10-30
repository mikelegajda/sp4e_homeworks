#include <iostream>
#include "ComputeArithmetic.hh"
#include "ComputePi.hh"
#include <iomanip>

int main(int argc, char ** argv){

    std::string seriesType = argv[1];
    unsigned int N = std::stoi(argv[2]);
    std::cout << "Input series type: " << seriesType << std::endl;
    std::cout << "Input N: " << N << std::endl;

    double ret = 0.0;
    Series *ptr = NULL;

    if (seriesType == "arithmetic"){
        ptr = new ComputeArithmetic();
        ret = ptr->compute(N);
        std::cout << std::setprecision(10) << ret << std::endl;
        delete ptr;
    }else if (seriesType == "pi"){
        ptr = new ComputePi();
        ret = ptr->compute(N);
        std::cout << std::setprecision(10) << ret << std::endl;
        delete ptr;
    }
    else{
        std::cerr << "Unkown series type" << std::endl;
    }

}