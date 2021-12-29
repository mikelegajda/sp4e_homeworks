#ifndef COMPUTE_TEMPERATURE_FINITE_DIFFERENCES_HH
#define COMPUTE_TEMPERATURE_FINITE_DIFFERENCES_HH

#include "compute.hh"
#include <Eigen/SparseCore>
#include <Eigen/SparseLU>

class ComputeTemperatureFiniteDifferences : public Compute {
public:
  ComputeTemperatureFiniteDifferences(Real deltaT) 
  {this->deltaT = deltaT;}
  void compute(System& system) override;
  void assembleLinearOperator(System& system);
  void assembleRightHandSide(System& system);

  Real& getDensity() { return density; };
  Real& getL() { return L; };
  Real& getDeltaT() { return deltaT; };
  Real& getConductivity() { return conductivity; };
  Real& getCapacity() { return capacity; };

private:
  Real density;
  Real L;
  Real deltaT;
  Real conductivity;
  Real capacity;
  Eigen::SparseMatrix<double> A;
  Eigen::VectorXd rhs;
  Eigen::SparseLU<Eigen::SparseMatrix<double>> solver;
};




#endif  // COMPUTE_TEMPERATURE_FINITE_DIFFERENCES_HH
