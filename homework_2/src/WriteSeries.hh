#include "DumperSeries.hh"
#include <map>

#ifndef WRITESERIES_HH
#define WRITESERIES_HH

class WriteSeries : public DumperSeries {
    public:
        // Constructor
        WriteSeries(Series &series, int maxiter, int frequency);
        // Destructor
        virtual ~WriteSeries();

        // Methods
        void setSeparator(std::string separator);
        void setExtension();
        void dump(std::ostream &os) override;
        void setPrecision(unsigned int precision) override;
    
    private:
        // Attributes
        int maxiter, frequency, precision;
        std::string separator, extension;
};

#endif