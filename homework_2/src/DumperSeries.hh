#include "Series.hh"
#include <ostream>

#ifndef DUMPERSERIES_HH
#define DUMPERSERIES_HH

class DumperSeries {
    public:
        // Constructor
        DumperSeries(Series &series) : series(series) {};

        // Destructor
        virtual ~DumperSeries() {};

    public:
        // Methods
        virtual void dump(std::ostream &os) = 0;
        virtual void setPrecision(unsigned int precision) = 0;

    protected:
        Series &series;
};

#endif
