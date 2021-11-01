#include <iostream>
#include "ComputeArithmetic.hh"
#include "ComputePi.hh"
#include "PrintSeries.hh"
#include "WriteSeries.hh"
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include <sstream>

int main(int argc, char ** argv){

    // using the stringstream to parse arguments
    std::string seriesType;
    unsigned int N;
    std::string outputType;
    std::string fileType;

    std::stringstream sstr;
    for (int i=1; i<argc; i++){
        sstr << argv[i] << " ";
    }
    sstr >> seriesType >> N >> outputType >> fileType;

    // print parsed arguments
    std::cout << "Input series type: " << seriesType << std::endl;
    std::cout << "Input N: " << N << std::endl;
    std::cout << "Input outputType: " << outputType << std::endl;
    std::cout << "Input fileType: " << fileType << std::endl;

    double ret = 0.0;
    Series *ptr = NULL;

    if (seriesType == "arithmetic"){
        ptr = new ComputeArithmetic();
    }else if (seriesType == "pi"){
        ptr = new ComputePi();
    }
    else{
        std::cerr << "Unkown series type" << std::endl;
        return -1;
    }


    DumperSeries *dumperPtr = NULL;
    if (outputType == "printToScreen"){
        dumperPtr = new PrintSeries(*ptr, N, 1);
        dumperPtr->dump(std::cout);
    }else if (outputType == "saveToFile"){
        std::string seperator;
        if (fileType.empty() || fileType == "tsv"){
            seperator = "\t";
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
        writerPtr->dump(resultFile);
    }
    else{
        std::cerr << "Unkown output type" << std::endl;
        return -1;
    };

    delete dumperPtr;
    delete ptr;

}