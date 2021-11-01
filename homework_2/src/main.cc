#include <iostream>
#include "ComputeArithmetic.hh"
#include "ComputePi.hh"
#include "PrintSeries.hh"
#include "WriteSeries.hh"
#include <fstream>
#include <iomanip>
#include <stdexcept>

int main(int argc, char ** argv){

    std::string seriesType = argv[1];
    unsigned int N = std::stoi(argv[2]);
    std::string outputType = argv[3];
    std::cout << "Input series type: " << seriesType << std::endl;
    std::cout << "Input N: " << N << std::endl;
    std::cout << "Input outputType: " << outputType << std::endl;

    double ret = 0.0;
    Series *ptr = NULL;

    if (seriesType == "arithmetic"){
        ptr = new ComputeArithmetic();
    }else if (seriesType == "pi"){
        ptr = new ComputePi();
    }
    else{
        std::cerr << "Unkown series type" << std::endl;
    }

    DumperSeries *dumperPtr = NULL;
    if (outputType == "printToScreen"){
        dumperPtr = new PrintSeries(*ptr, N, 10);
        dumperPtr->dump(std::cout);
    }else if (outputType == "saveToFile"){
        dumperPtr = new WriteSeries(*ptr, N, 10);
        std::ofstream resultFile;
        resultFile.open("result.txt");
        dumperPtr->dump(resultFile);
    }
    else{
        std::cerr << "Unkown output type" << std::endl;
        // throw std::invalid_argument("Unkown output type");
    };

    delete dumperPtr;
    delete ptr;

}