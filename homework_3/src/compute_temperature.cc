#include "compute_temperature.hh"
#include "fft.hh"
#include "material_point.hh"
#include <cmath>

/* -------------------------------------------------------------------------- */

void ComputeTemperature::compute(System& system) {
    UInt size = sqrt(system.getNbParticles());

    Matrix<complex> Temp(size);
    Matrix<std::complex<int>> Freq(size);
    Freq = FFT::computeFrequencies(size); // TODO: normalize or not
    Matrix<complex> Heat(size); // matrix of volumetric heat source/ heat rate
    Matrix<complex> deltaTemp(size); // matirx of dT/dt

    // set Temp matrix
    for(auto&& entry: index(Temp)){
        int i = std::get<0>(entry);
        int j = std::get<1>(entry);
        auto& val = std::get<2>(entry);
        auto& particle = static_cast<MaterialPoint &>(system.getParticle(j*size + i));
        val = particle.getTemperature();
    } 
    // set HeatRate matrix
    for(auto&& entry: index(Heat)){
        int i = std::get<0>(entry);
        int j = std::get<1>(entry);
        auto& val = std::get<2>(entry);
        auto& particle = static_cast<MaterialPoint &>(system.getParticle(j*size + i));
        val = particle.getHeatRate();
    }

    // 1st step: FFT of temperature matrix and heat rate matrix
    Temp = FFT::transform(Temp);
    Heat = FFT::transform(Heat);

    // 2nd step: compute deltaTemp matrix
    for(auto&& entry: index(deltaTemp)){
        int i = std::get<0>(entry);
        int j = std::get<1>(entry);
        auto& val = std::get<2>(entry);
        double k = std::abs(Freq(i,j));
        auto& particle = static_cast<MaterialPoint &>(system.getParticle(j*size + i));
        val = 1.0/(particle.getMass()*this->heat_cap) * (Heat(i, j) - this->kappa*Temp(i,j)*k*k);
    }

    // 3rd step: inverse FFT
    deltaTemp = FFT::itransform(deltaTemp);

    // 4th step: update particle temperature
    for(auto&& entry: index(deltaTemp)){
        int i = std::get<0>(entry);
        int j = std::get<1>(entry);
        auto& val = std::get<2>(entry);
        auto& particleTemp = static_cast<MaterialPoint &>(system.getParticle(j*size + i)).getTemperature();
        Real newTemp = particleTemp + val.real() * this->delta;
        particleTemp = newTemp;
    }

}

/* -------------------------------------------------------------------------- */
