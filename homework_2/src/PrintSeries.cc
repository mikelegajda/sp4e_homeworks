#include <iostream>
#include <iomanip>
#include <cmath>
#include "PrintSeries.hh"

PrintSeries::PrintSeries(Series &s, int m, int f) : DumperSeries(s), maxiter(m), freq(f), precision(6) {}

// Destructor definition
PrintSeries::~PrintSeries() = default;

// Set precision if valid value specified by user
void PrintSeries::setPrecision(int p) {
    this->precision = abs(p);
}

void PrintSeries::dump(std::ostream &os) {
    
    double init;
    double pred = this->series.getAnalyticPrediction();

    for (int i = 1; i <= this->maxiter; i += this->freq) {
        init = this->series.compute(i);
        os  << i << "\t"
            << std::setprecision(this->precision)
            << init << "\t";

	// If analytical prediction exists, 
	// print error to the ostream
        if (!std::isnan(pred)) {
            os << "\t" << pred-init;
        }
        os << std::endl; 
    }
}