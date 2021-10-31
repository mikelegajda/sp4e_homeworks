#include "Series.hh"
#include <ostream>

#ifndef DUMPERSERIES_HH
#define DUMPERSERIES_HH

class DumperSeries {
    public:
        DumperSeries(Series &series) : series(series) {};
        virtual ~DumperSeries() {};

    public:

        virtual void dump(std::ostream &os) = 0;
        virtual void setPrecision(unsigned int precision) = 0;

    protected:
        Series &series;
};

#endif
