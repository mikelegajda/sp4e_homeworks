#include <iostream>
#include "ComputeArithmetic.hh"
#include "ComputePi.hh"
#include "PrintSeries.hh"
#include "WriteSeries.hh"
#include "RiemannIntegral.hh"
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include <sstream>
#include "dumper__series.hh"
#include <math.h>

// Usage example:
// 1. ./src/main arithmetic 100 saveAllToFile csv
// 2. ./src/main pi 100 printAllToScreen
// 3. ./src/main integral 10000 printOnlyResult nan cos 0 3.1415926
// 4. ./src/main integral 10000 printOnlyResult nan cubic 0 1

int main(int argc, char ** argv){
    //Riemann Test
    // Series *ptr = NULL;
    // double ret = 0.0;
    // std::string f_type = "sin";
    // ptr = new RiemannIntegral(0, M_PI/2.0, f_type);
    // ret = ptr->compute(1000000);
    // std::cout<< ret << std::endl;

    // using the stringstream to parse arguments
    std::string seriesType;
    unsigned int N;
    std::string outputType;
    std::string fileType;
    std::string functionType;
    double a;
    double b;

    std::stringstream sstr;
    for (int i=1; i<argc; i++){
        sstr << argv[i] << " ";
    }
    sstr >> seriesType >> N >> outputType >> fileType >> functionType >> a >> b;

    // print parsed arguments
    std::cout << "Input series type: " << seriesType << std::endl;
    std::cout << "Input N: " << N << std::endl;
    std::cout << "Input outputType: " << outputType << std::endl;
    std::cout << "Input fileType: " << fileType << std::endl;
    std::cout << "Input functionType: " << functionType << std::endl;
    std::cout << "Input integral lower limit a: " << a << std::endl;
    std::cout << "Input integral upper limit b: " << b << std::endl;

    double ret = 0.0;
    Series *ptr = NULL;

    if (seriesType == "arithmetic"){
        ptr = new ComputeArithmetic();
    }else if (seriesType == "pi"){
        ptr = new ComputePi();
    }else if (seriesType == "integral"){
        ptr = new RiemannIntegral(a, b, functionType);
    }
    else{
        std::cerr << "Unkown series type" << std::endl;
        return -1;
    }


    DumperSeries *dumperPtr = NULL;
    if (outputType == "printAllToScreen" && seriesType != "integral"){
        dumperPtr = new PrintSeries(*ptr, N, 1);
        PrintSeries *printerPtr = static_cast<PrintSeries *>(dumperPtr);
        printerPtr->dump();
    }else if (outputType == "saveAllToFile" && seriesType != "integral"){
        std::string seperator;
        if (fileType.empty()){
            seperator = "\t";
            fileType = "txt";
        }else if (fileType == "csv"){
            seperator = ",";
        }else if (fileType == "txt"){
            seperator = "\t";
        }else if (fileType == "psv"){
            seperator = "|";
        }else{
            std::cerr << "Unkown file type" << std::endl;
            return -1;
        }
        dumperPtr = new WriteSeries(*ptr, N, 1);
        WriteSeries *writerPtr = static_cast<WriteSeries *>(dumperPtr);
        std::ofstream resultFile;
        resultFile.open("result." + fileType);
        writerPtr->setSeparator(seperator);
        // writerPtr->dump(resultFile);
        resultFile << *writerPtr; // using defined operator
    }else if(outputType == "printOnlyResult" && seriesType == "integral"){
        std::cout<< "Integral Result: " << std::endl;
        std::cout<< ptr->compute(N) << std::endl;
    }
    else{
        std::cerr << "Unkown output type" << std::endl;
        return -1;
    };

    delete dumperPtr;
    delete ptr;



}