#include "DumperSeries.hh"
#include <map>

#ifndef WRITESERIES_HH
#define WRITESERIES_HH

class WriteSeries : public DumperSeries {
    public:
        WriteSeries(Series &series, int maxiter, int frequency);
        virtual ~WriteSeries();

        //Funtions
        void setSeparator(std::string separator);
        void setExtension();
        void dump(std::ostream &os) override;
        void setPrecision(int precision) override;
    
    private:
        int maxiter, frequency, precision;
        std::string separator, extension;
};

#endif