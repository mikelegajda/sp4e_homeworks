#include "compute_temperature.hh"
#include "fft.hh"
#include "material_point.hh"
#include <cmath>

/* ----------------------------------FIRST_VERSION---------------------------------------- 
void ComputeTemperature::compute(System& system) {
    UInt size = sqrt(system.getNbParticles());

    Matrix<complex> Temp(size);
    Matrix<std::complex<int>> Freq(size);
    //Freq = FFT::computeFrequencies(size); // TODO: normalize or not
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
    Freq = FFT::computeFrequencies(size);
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
 -------------------------------------------------------------------------- */



/* ----------------------------------SECOND_VERSION---------------------------------------- */

ComputeTemperature::ComputeTemperature(Real delta, Real rho, Real heat_cap, Real kappa) 
                              : delta(delta), rho(rho), heat_cap(heat_cap), kappa(kappa) {}

void ComputeTemperature::setDelta(Real delta) { this->delta = delta; }

/* -------------------------------------------------------------------------- */
//! Compute one step of the transient heat equation
void ComputeTemperature::compute(System& system) {
    int N = sqrt(system.getNbParticles());

    Matrix<complex> theta_n(N);
    Matrix<complex> hv(N);
     
    /* We assume the system to be a rectangular grid with a square number of MaterialPoint. We assume
    every point to be given in order. However we still need to compute the domain size */
    Real max_x = std::numeric_limits<Real>::min();
    Real min_x = std::numeric_limits<Real>::max();
    Real max_y = std::numeric_limits<Real>::min();
    Real min_y = std::numeric_limits<Real>::max();

    /* Build the system into matrices to use the fourier transform & matrix operations */
    int i = 0;
    int j = 0;
    for (auto& par : system) {

        MaterialPoint & matpt = dynamic_cast<MaterialPoint&>(par);

        double ptx = matpt.getPosition()[0];
        double pty =  matpt.getPosition()[1];
        min_x = std::min(min_x, ptx);
        min_y = std::min(min_x, pty);
        max_x = std::max(max_x, ptx);
        max_y = std::max(max_x, pty);

        theta_n(i,j) = matpt.getTemperature();
        hv(i,j)      = matpt.getHeatRate();
        
        i++;
        if (i >= N){
            i = 0;
            j++;
        }
    }

    /* Find the lenght of the system in the same units */
    Real Lx = max_x - min_x;
    Real Ly = max_y - min_y;


    /* Space fourier transform and unscalled frequencies */
    Matrix<complex> thetah = FFT::transform(theta_n);
    Matrix<std::complex<int>> q_freq = FFT::computeFrequencies(N);
    Matrix<complex> hvh = FFT::transform(hv);

    /* Compute, for each particle, it's Delta t in time in fourier space */
    std::complex<double> qfreq_ij;
    Matrix<complex> dthetah_over_dt(N);
    for (auto&& entry : index(dthetah_over_dt)) {
        int i = std::get<0>(entry);
        int j = std::get<1>(entry);
        auto& val = std::get<2>(entry);
        
        Real q_x = (2*M_PI/Lx)*q_freq(i,j).real();
        Real q_y = (2*M_PI/Ly)*q_freq(i,j).imag();

        val = (hvh(i,j) - kappa * thetah(i,j)  * (q_x * q_x + q_y * q_y)) / (rho * heat_cap);
        
    }

    /* inverse transform of the delta of temperature over time */
    Matrix<complex> dtheta_over_dt = FFT::itransform(dthetah_over_dt);

    /* perform the forward euler scheme and write the new system state */
    i = 0; j = 0;
    for (auto& par : system) {
        MaterialPoint & matpt = dynamic_cast<MaterialPoint&>(par);
        // enfore boundary condition
        if (i == 0 || j == 0 || i == N-1 || j == N-1){
            matpt.getTemperature() = 0;
        }else{
            matpt.getTemperature() = std::real(theta_n(i,j) + delta*dtheta_over_dt(i,j));
        }
        i++;
        if (i >= N){
            i = 0;
            j++;
        }
    }
}

/* -------------------------------------------------------------------------- */