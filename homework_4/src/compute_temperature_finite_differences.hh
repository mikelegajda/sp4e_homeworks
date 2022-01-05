#ifndef COMPUTE_TEMPERATURE_FINITE_DIFFERENCES_HH
#define COMPUTE_TEMPERATURE_FINITE_DIFFERENCES_HH

#include "compute.hh"
#include <Eigen/SparseCore>
#include <Eigen/SparseLU>

class ComputeTemperatureFiniteDifferences : public Compute {
public:
  //TODO: checj renaming deltaT to dt
  ComputeTemperatureFiniteDifferences(Real dt) {this->dt = dt;}

  void compute(System& system) override;
  void assembleLinearOperator(System& system);
  void assembleRightHandSide(System& system);

  //! return the heat conductivity
  Real& getConductivity() { return conductivity; };
  //! return the heat capacity
  Real& getCapacity() { return capacity; };
  //! return the mass density
  Real& getDensity() { return density; };
  //! return the characteristic length of the square
  Real& getL() { return L; };
  //! return the time interval
  Real& getDeltat() { return dt; };

private:
  Real conductivity;
  Real capacity;
  Real density;
  Real L;
  Real dt;
  Eigen::SparseMatrix<double> sparse_matrix;
  Eigen::VectorXd sparse_vector;

};

#endif  // COMPUTE_TEMPERATURE_FINITE_DIFFERENCES_HH
