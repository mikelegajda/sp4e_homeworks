#ifndef DUMPER__SERIES_HPP
#define DUMPER__SERIES_HPP

#include<iostream>
#include "DumperSeries.hh"

inline std::ostream & operator <<(std::ostream & stream, DumperSeries & _this){
    _this.dump(stream);
    return stream;
}

#endif