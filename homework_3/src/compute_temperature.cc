#include "compute_temperature.hh"
#include "fft.hh"
#include "material_point.hh"
#include <cmath>


/* -------------------------------------------------------------------------- */

ComputeTemperature::ComputeTemperature(Real delta, Real rho, Real heat_cap, Real kappa) 
                              : delta(delta), rho(rho), heat_cap(heat_cap), kappa(kappa) {}

void ComputeTemperature::setDelta(Real delta) { this->delta = delta; }

/* -------------------------------------------------------------------------- */
//! Step one of the transient heat equation
void ComputeTemperature::compute(System& system) {
    int N = sqrt(system.getNbParticles());

    Matrix<complex> theta_n(N);
    Matrix<complex> hv(N);

    // Our system is a rectangulat grid composed by a square number of MaterialPoints.
    Real max_x = std::numeric_limits<Real>::min();
    Real min_x = std::numeric_limits<Real>::max();
    Real max_y = std::numeric_limits<Real>::min();
    Real min_y = std::numeric_limits<Real>::max();
    //Building matricial system in order to use Fourier transpormations and matrix operations
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
    //Find the lenght of the system in the same units
    Real Lx = max_x - min_x;
    Real Ly = max_y - min_y;

    Matrix<complex> thetah = FFT::transform(theta_n);
    Matrix<std::complex<int>> q_freq = FFT::computeFrequencies(N);
    Matrix<complex> hvh = FFT::transform(hv);

    // Calculating delta_t for each particle
    std::complex<double> qfreq_ij;
    Matrix<complex> dthetah_over_dt(N);
    for (auto&& entry : index(dthetah_over_dt)) {
        int i = std::get<0>(entry);
        int j = std::get<1>(entry);
        auto& val = std::get<2>(entry);
        
        Real q_x = (2*M_PI/Lx)*q_freq(i,j).real() / N;
        Real q_y = (2*M_PI/Ly)*q_freq(i,j).imag() / N;

        val = (hvh(i,j) - kappa * thetah(i,j)  * (q_x * q_x + q_y * q_y)) / (rho * heat_cap);
        
    }

    // Inverse transformation
    Matrix<complex> dtheta_over_dt = FFT::itransform(dthetah_over_dt);

    // Forward Euler scheme
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