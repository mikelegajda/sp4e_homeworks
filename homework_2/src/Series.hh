#ifndef SERIES_HPP
#define SERIES_HPP

class Series
{
private:
    /* data */
public:
    // Attributes
    unsigned int currentTerm = 0;
    double currentValue = 0;

    // Constructor
    Series(/* args */);
    // Destructor
    ~Series();

    // Methods
    virtual double compute(unsigned int N) = 0;
    virtual void addTerm();
    virtual double computeTerm(unsigned int k) = 0; 
    virtual double getAnalyticPrediction () = 0;
};
#endif


