#include "csv_reader.hh"
#include "csv_writer.hh"
#include "material_point.hh"
#include "material_points_factory.hh"
#include "system.hh"
#include "compute_temperature.hh"
#include <gtest/gtest.h>
#include <iostream>
#include "compute_temperature_finite_differences.hh"


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
        for (UInt i = 0; i < size; ++i){
            for (UInt j = 0; j < size; ++j){
                MaterialPoint p;
                // Evenly spaced particles 
                p.getPosition()[0] = -L/2 + i*L/(size-1);
                p.getPosition()[1] = -L/2 + j*L/(size-1); 
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
    Real  rho = 1.;
    Real  C =  1.;
    Real  k = 1.;
    UInt size = 3;
    Real L = 2.;
    // Set initial temperature field
    Real  T = 1;

};
/*****************************************************************/

TEST_F(TemperatureTest, temperature_equilibrium) {

  //Number of allowed iterations
  UInt steps = 20000;
  dt = 0.001;

  ComputeTemperatureFiniteDifferences temperature = ComputeTemperatureFiniteDifferences(dt);

  // Parameters
  temperature.getCapacity()= C;
  temperature.getConductivity() = k;
  temperature.getDensity() = rho;
  temperature.getDeltat() = dt;
  temperature.getL() = L;

  for(UInt i = 0; i < steps; ++i){
    temperature.assembleRightHandSide(system);
    temperature.assembleLinearOperator(system);
    temperature.compute(system);
  }

  for (auto& part : system) {
    auto& p = static_cast<MaterialPoint&>(part);
    ASSERT_NEAR(p.getTemperature(), 0, TEST_TOLERANCE);
  }
}