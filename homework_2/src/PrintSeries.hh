#include "DumperSeries.hh"

#ifndef PRINTSERIES_HH
#define PRINTSERIES_HH

class PrintSeries : public DumperSeries {
    public:
        // Constructor
        PrintSeries(Series &series, int maxiter, int frequency);
        // Destructor
        virtual ~PrintSeries();

        // Methods
        void dump(std::ostream &os = std::cout) override;
        void setPrecision(unsigned int precision) override;

    private:
        // Attributes
        int freq, maxiter;
        unsigned int precision;
};

#endif