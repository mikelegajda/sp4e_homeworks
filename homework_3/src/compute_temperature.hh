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
  Real delta;       /* integration time step */
  Real rho;      /* mass density */
  Real heat_cap;        /* specific heat capacity */
  Real kappa;    /* heat conductivity */
};

/* -------------------------------------------------------------------------- */
#endif  //__COMPUTE_TEMPERATURE__HH__
