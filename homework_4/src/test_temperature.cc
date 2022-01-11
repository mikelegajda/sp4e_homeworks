#include "compute_temperature.hh"
#include "compute_temperature_finite_differences.hh"
#include "csv_reader.hh"
#include "csv_writer.hh"
#include "material_point.hh"
#include "material_points_factory.hh"
#include "system.hh"
#include <gtest/gtest.h>
#include <iostream>

#define TEST_TOLERANCE 1e-8

/*****************************************************************/
// Fixture class
class TemperatureTest : public ::testing::Test {
public:
  void SetUp() override {
    MaterialPointsFactory::getInstance();
    // Store particles in a vector
    std::vector<MaterialPoint> vecParticles;

    // Populating vector with particles
    for (UInt i = 0; i < size; ++i) {
      for (UInt j = 0; j < size; ++j) {
        MaterialPoint p;
        // Evenly spaced particles
        p.getPosition()[0] = -L / 2 + i * L / (size - 1);
        p.getPosition()[1] = -L / 2 + j * L / (size - 1);
        // Fix z-position to 0
        p.getPosition()[2] = 0;
        p.getTemperature() = T;
        p.getHeatSource() = 0;
        vecParticles.push_back(p);
        T++;
      }
    }

    // Add created particles to vecParticles
    for (auto& p : vecParticles) {
      system.addParticle(std::make_shared<MaterialPoint>(p));
    }
  }

  System system;

  // Set variables
  Real dt = 0.001;
  Real rho = 1.;
  Real C = 1.;
  Real k = 1.;
  UInt size = 3;
  Real L = 2.;
  // Set initial temperature field
  Real T = 1;
};
/*****************************************************************/

TEST_F(TemperatureTest, temperature_equilibrium) {

  // Number of allowed iterations
  UInt steps = 20000;
  dt = 0.001;

  ComputeTemperatureFiniteDifferences temperature =
      ComputeTemperatureFiniteDifferences(dt);

  // Parameters
  temperature.getCapacity() = C;
  temperature.getConductivity() = k;
  temperature.getDensity() = rho;
  temperature.getDeltat() = dt;
  temperature.getL() = L;

  for (UInt i = 0; i < steps; ++i) {
    temperature.compute(system);
  }

  for (auto& part : system) {
    auto& p = static_cast<MaterialPoint&>(part);
    ASSERT_NEAR(p.getTemperature(), 0, TEST_TOLERANCE);
  }
}

/*****************************************************************/
TEST_F(TemperatureTest, sinHeat) {
  // Number of allowed iterations
  UInt steps = 1000;

  // assume L = 2
  for (auto& p : system) {
    MaterialPoint& pt = dynamic_cast<MaterialPoint&>(p);
    // initial temperature at equilibrium state
    pt.getTemperature() = sin(M_PI * pt.getPosition()[0]);
    pt.getHeatSource() = (M_PI) * (M_PI)*sin(M_PI * pt.getPosition()[0]);
  }

  

  auto temperature = std::make_shared<ComputeTemperatureFiniteDifferences>(dt);
  // Parameters
  temperature->getCapacity() = C;
  temperature->getConductivity() = k;
  temperature->getDensity() = rho;
  temperature->getL() = L;

  for (UInt s = 0; s < steps; ++s) {
    temperature->compute(system);
  }

  for (UInt j = 0; j < size; j++) {
    for (UInt i = 0; i < size; i++) {
      MaterialPoint& pt =
          dynamic_cast<MaterialPoint&>(system.getParticle(j * size + i));
      Real x = pt.getPosition()[0];
      ASSERT_NEAR(pt.getTemperature(), sin(x * M_PI), TEST_TOLERANCE);
    }
  }
}
/*****************************************************************/