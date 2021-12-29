#include "csv_reader.hh"
#include "csv_writer.hh"
#include "material_point.hh"
#include "material_points_factory.hh"
#include "system.hh"
#include "compute_temperature.hh"
#include <gtest/gtest.h>
#include <iostream>

#define TEST_TOLERANCE 1e-2

/*****************************************************************/
// Fixture class
class MaterialPointsSystem: public ::testing::Test {
  public:
    UInt size = 100;
    Real L = 2.;

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
              // Fix z-position to 0'
              p.getPosition()[2] = 0; 
              vecParticles.push_back(p); 
          }
    }

    // Add created particles to vecParticles
    for (auto& p : vecParticles) {
      system.addParticle(std::make_shared<MaterialPoint>(p)); 
    }
  }

  System system;
  UInt steps = 10;  
  Real dt = 0.0001;

  // Set variables
  Real  rho = 1.;
  Real  C =  1.;
  Real  kappa = 1.;

};

/*****************************************************************/
TEST_F(MaterialPointsSystem, homogeneousNoHeat){
    for(auto& p : system){
        MaterialPoint & pt = dynamic_cast<MaterialPoint&>(p);
        // Initial temperature
        pt.getTemperature() = 1.; 
    }

    auto temperature = std::make_shared<ComputeTemperature>(dt, rho, C, kappa);

    for (UInt s = 0; s < steps; ++s) {
      temperature->compute(system);
    
      // don't test on the boundary
      for (UInt j = 1; j < size - 1; j++)
      {
          for (UInt i = 1; i < size - 1; i++)
          {
              MaterialPoint &pt = dynamic_cast<MaterialPoint &>(system.getParticle(j * size + i));
              ASSERT_NEAR(pt.getTemperature(), 1, TEST_TOLERANCE);
          }
      }
    }
}
/*****************************************************************/

/*****************************************************************/
TEST_F(MaterialPointsSystem, sinHeat){
    // assume L = 2

    for(auto& p : system){
      MaterialPoint & pt = dynamic_cast<MaterialPoint&>(p);
      // initial temperature at equilibrium state
      pt.getTemperature() = sin(M_PI*pt.getPosition()[0]);  
      pt.getHeatRate() = (M_PI)*(M_PI)*sin(M_PI*pt.getPosition()[0]);
    }

    auto temperature = std::make_shared<ComputeTemperature>(dt, rho, C, kappa);
    
    for (UInt s = 0; s < steps; ++s) {
      temperature->compute(system);

      // don't test on the boundary
      for (UInt j = 1; j < size - 1; j++)
      {
          for (UInt i = 1; i < size - 1; i++)
          {   
              MaterialPoint &pt = dynamic_cast<MaterialPoint &>(system.getParticle(j * size + i));
              Real x = pt.getPosition()[0];
              ASSERT_NEAR(pt.getTemperature(), sin(x * M_PI), TEST_TOLERANCE);
          }
      }
    }
}
/*****************************************************************/

/*****************************************************************/
TEST_F(MaterialPointsSystem, stepHeat){   
    for(auto& p : system){
        MaterialPoint & pt = dynamic_cast<MaterialPoint&>(p);
      Vector postion_xyz = pt.getPosition();
    
      // Temperature at equilibrium
      if (postion_xyz[0] <= -0.5)
        pt.getTemperature()=-postion_xyz[0]-1;
      else if ((postion_xyz[0] > -0.5) && (postion_xyz[0] <= 0.5))
        pt.getTemperature()=postion_xyz[0];
      else 
        pt.getTemperature()=-postion_xyz[0]+1;

      if (fabs(postion_xyz[0] - 0.5) < L/(2*(size-1)))
        pt.getHeatRate() = 1.;
      else if (fabs(postion_xyz[0] + 0.5) < L/(2*(size-1))) 
        pt.getHeatRate() = -1.;
      else
        pt.getHeatRate() = 0.;
    }

    auto temperature = std::make_shared<ComputeTemperature>(dt, rho, C, kappa);
    
    // Temperature test
    for (UInt s = 0; s < steps; ++s) {
      temperature->compute(system);
      // don't test on the boundary
      for (UInt j = 1; j < size - 1; j++)
      {
          for (UInt i = 1; i < size - 1; i++)
          {
              MaterialPoint &pt = dynamic_cast<MaterialPoint &>(system.getParticle(j * size + i));
              Real x = pt.getPosition()[0];
              if (x <= -0.5)
              {
                  ASSERT_NEAR(pt.getTemperature(), -x - 1, TEST_TOLERANCE);
              }
              else if ((x > -0.5) && (x < 0.5))
                  ASSERT_NEAR(pt.getTemperature(), x, TEST_TOLERANCE);
              else
                  ASSERT_NEAR(pt.getTemperature(), -x + 1, TEST_TOLERANCE);
          }
      }
    }
}
/*****************************************************************/