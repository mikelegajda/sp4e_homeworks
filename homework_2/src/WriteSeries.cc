#include "WriteSeries.hh"
#include <cmath>
#include <iomanip>

WriteSeries::WriteSeries(Series &series, int miter, int freq):
    DumperSeries(series), maxiter(miter), frequency(freq), separator("\t"), precision(4), extension(".txt") {}

WriteSeries::~WriteSeries() = default;

void WriteSeries::setPrecision(unsigned int prec) {
    this->precision = prec; 
}

void WriteSeries::setSeparator(std::string sep) {
    this->separator = sep; 
}

void WriteSeries::setExtension() {
    if (this->separator == ","){
        this->extension = ".csv";
    } else if (this->separator == "|"){
        this->extension = ".psv";
    }else {
        this->extension = ".txt";
    }
}

// Write to the ostream
void WriteSeries::dump(std::ostream &os) {
    
    double ap = this->series.getAnalyticPrediction();
    double Sn;

    // Compute series and print
    for (int i = 1; i <= this->maxiter; i += this->frequency) {
        Sn = this->series.compute(i);
        os << i << this->separator << std::setprecision(this->precision)
            << Sn;

	// Print analytical prediction
        if (!std::isnan(ap)) {
            os << this->separator << ap;
        }
        os << std::endl;
    }
}


