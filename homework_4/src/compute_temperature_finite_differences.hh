#ifndef COMPUTE_TEMPERATURE_FINITE_DIFFERENCES_HH
#define COMPUTE_TEMPERATURE_FINITE_DIFFERENCES_HH

#include "compute.hh"
#include <Eigen/SparseCore>
#include <Eigen/SparseLU>

class ComputeTemperatureFiniteDifferences : public Compute {
public:
  ComputeTemperatureFiniteDifferences(Real dt) {}

  void compute(System& system) override;
  void assembleLinearOperator(System& system);
  void assembleRightHandSide(System& system);
};

#endif  // COMPUTE_TEMPERATURE_FINITE_DIFFERENCES_HH
