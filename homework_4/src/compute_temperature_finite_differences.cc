#include "compute_temperature_finite_differences.hh"
#include "material_point.hh"
#include "matrix.hh"
#include "fft.hh"

void ComputeTemperatureFiniteDifferences::compute(System& system) {
  assembleLinearOperator(system);
  assembleRightHandSide(system);

  Eigen::SparseMatrix<double> &A = sparse_matrix;
  Eigen::VectorXd &b = sparse_vector;

  Eigen::SparseLU<Eigen::SparseMatrix<double> > solver;
  A.makeCompressed(); 
  solver.analyzePattern(A); 
  solver.factorize(A);
  auto next_theta = solver.solve(b); 
    // LU factorization
    auto i = 0;
    for (auto& part : system) {
        static_cast<MaterialPoint&>(part).getTemperature() = next_theta(i);
        i++;
    }
}

void ComputeTemperatureFiniteDifferences::assembleLinearOperator(
    System& system) {

    // number of particles
    UInt dimension = system.getNbParticles();
    // See equation (3) in problem definition -> rhs
    Real linear_factor = dt * conductivity / (capacity * density);

    // Resizing sparse_matrix
    sparse_matrix.resize(dimension, dimension);

    // Defining n and calcualting a beforehand -> will be used as division factor as seen in eq. (1)
    UInt n = std::sqrt(dimension);
    Real a = L/(n-1);

    // i is the index of a particle
    for(UInt i = 0; i < dimension; ++i){


        // Checking boundaries
        if((i % n) != 0)
            sparse_matrix.insert(i, i-1) = 1;
        if(i > n-1)
            sparse_matrix.insert(i, i-n) = 1;

        // (i,i) is the index for the particle at each loop iteration
        sparse_matrix.insert(i, i) = -4;

    }

    sparse_matrix = (sparse_matrix * linear_factor) / (a*a);

    Eigen::SparseMatrix<Real> sparse_ones_matrix(dimension, dimension);

}

void ComputeTemperatureFiniteDifferences::assembleRightHandSide(
    System& system) {

  UInt dimension = system.getNbParticles();
  UInt n = std::sqrt(dimension);

  Matrix<double> temperature(n);
  Matrix<double> heat_source(n);

  std::transform(system.begin(), system.end(), temperature.begin(),
                 [](auto& part) {
                   return static_cast<MaterialPoint&>(part).getTemperature();
                 });

  std::transform(system.begin(), system.end(), heat_source.begin(),
                 [](auto& part) {
                   return static_cast<MaterialPoint&>(part).getHeatSource();
                 });

  const auto factor_right = dt / (density * capacity);

  Eigen::Map<Eigen::VectorXd> temp_vector(
      reinterpret_cast<double*>(temperature.data()), dimension);

  Eigen::Map<Eigen::VectorXd> heat_source_vector(
      reinterpret_cast<double*>(heat_source.data()), dimension);

  auto assembled_vector = heat_source_vector + temp_vector;

  sparse_vector = assembled_vector;
}
