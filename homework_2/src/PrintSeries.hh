#include "DumperSeries.hh"

#ifndef PRINTSERIES_HH
#define PRINTSERIES_HH

class PrintSeries : public DumperSeries {
    public:
        PrintSeries(Series &series, int maxiter, int frequency);
        virtual ~PrintSeries();

        void dump(std::ostream &os) override;
        void setPrecision(int precision) override;

    private:
        int freq, maxiter;
        unsigned int precision;
};

#endif