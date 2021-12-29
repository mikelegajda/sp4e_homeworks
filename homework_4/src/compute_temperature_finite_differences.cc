#include "compute_temperature_finite_differences.hh"
#include "material_point.hh"
#include "matrix.hh"

void ComputeTemperatureFiniteDifferences::compute(System& system) {
    
    ComputeTemperatureFiniteDifferences(deltaT).assembleLinearOperator(system);
    ComputeTemperatureFiniteDifferences(deltaT).assembleRightHandSide(system);
    
    UInt dimension = system.getNbParticles();
    UInt n = std::sqrt(dimension);
    
    Eigen::VectorXd x(dimension);
    //Sparse matrix is A; Sparse vector is rhs (b)
    A.makeCompressed();
    solver.analyzePattern(A);
    solver.factorize(A);

    x = solver.solve(rhs);

    UInt i = 0;
    for (auto& part : system) {
        static_cast<MaterialPoint&>(part).getTemperature() = x(i);
        i++;
    }

}

void ComputeTemperatureFiniteDifferences::assembleLinearOperator(
    System& system) {
    UInt n = std::sqrt(system.getNbParticles());
    auto deltaL = L/n;

    Matrix<double> temperature(n);

    std::transform(system.begin(), system.end(), temperature.begin(),
                 [](auto& part) {
                   return static_cast<MaterialPoint&>(part).getTemperature();
                 });

    }

void ComputeTemperatureFiniteDifferences::assembleRightHandSide(
    System& system) {}
