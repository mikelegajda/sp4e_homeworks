#include "DumperSeries.hh"

#ifndef PRINTSERIES_HH
#define PRINTSERIES_HH

class PrintSeries : public DumperSeries {
    public:
        PrintSeries(Series &series, int maxiter, int frequency);
        virtual ~PrintSeries();

        void dump(std::ostream &os = std::cout) override;
        void setPrecision(unsigned int precision) override;

    private:
        int freq, maxiter;
        unsigned int precision;
};

#endif