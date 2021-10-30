#include "Series.hh"
#include <cmath>

Series::Series(/* args */)
{
};

Series::~Series()
{
};

void Series::addTerm(){
    this->currentTerm += 1;
    this->currentValue += this->computeTerm(this->currentTerm);
}

double Series::compute(unsigned int N){
    if (this->currentTerm <= N){
        N -= this->currentTerm;
    }else{
        this->currentTerm = 0;
        this->currentValue = 0;
    };

    for (int i = 1; i <= N; i++){
        this->addTerm();
    }

    return this->currentValue;
}

double Series::getAnalyticPrediction(){
    return nan("");
};