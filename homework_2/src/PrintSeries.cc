#include <iostream>
#include <iomanip>
#include <cmath>
#include "PrintSeries.hh"

PrintSeries::PrintSeries(Series &s, int m, int f) : DumperSeries(s), maxiter(m), freq(f), precision(6) {}

// Destructor definition
PrintSeries::~PrintSeries() = default;

// Set precision
void PrintSeries::setPrecision(unsigned int prec) {
    this->precision = prec;
}

void PrintSeries::dump(std::ostream &os) {
    
    double init;
    double pred;

    for (int i = 1; i <= this->maxiter; i += this->freq) {
        init = this->series.compute(i);
        pred = this->series.getAnalyticPrediction();
        os  << i << "\t"
            << std::setprecision(this->precision)
            << init << "\t";

	// Print analytical prediction exists
        if (!std::isnan(pred)) {
            os << "\t" << pred-init;
        }
        os << std::endl; 
    }
}