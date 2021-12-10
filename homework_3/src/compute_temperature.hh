#ifndef __COMPUTE_TEMPERATURE__HH__
#define __COMPUTE_TEMPERATURE__HH__

/* -------------------------------------------------------------------------- */
#include "compute.hh"

//! Compute contact interaction between ping-pong balls
class ComputeTemperature : public Compute {

  // Virtual implementation
public:
  //! Penalty contact implementation
  void compute(System& system) override;
  void setDelta(Real delta) { this->delta = delta;};
  void setK(Real k) {this->kappa = k;};
  void setHeadCapacity(Real heat_cap) {this->heat_cap = heat_cap;};

private:
  Real delta;
  Real kappa = 1.0;
  Real heat_cap = 1.0; // heat capacity

};

/* -------------------------------------------------------------------------- */
#endif  //__COMPUTE_TEMPERATURE__HH__
