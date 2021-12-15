#ifndef __COMPUTE_TEMPERATURE__HH__
#define __COMPUTE_TEMPERATURE__HH__

/* -------------------------------------------------------------------------- */
#include "compute.hh"

//! Compute contact interaction between ping-pong balls
class ComputeTemperature : public Compute {
public:
  // constructor
  ComputeTemperature(Real timestep, Real rho, Real heat_cap, Real kappa);
  //! Penalty contact implementation
  void compute(System& system) override;

  void setDelta(Real delta);
private:
  Real delta = 0.001;       /* integration time step */
  Real rho = 1.0;      /* mass density */
  Real heat_cap = 1.0;        /* specific heat capacity */
  Real kappa = 1.0;    /* heat conductivity */
};

/* -------------------------------------------------------------------------- */
#endif  //__COMPUTE_TEMPERATURE__HH__
