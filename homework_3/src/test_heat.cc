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
    
      // equilibrium temperature (should hold throughout)
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
    
    // test
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




/*=================================================================*/
/*****************************OLD************************************

// Fixture class
class MaterialPointsSystem : public ::testing::Test {
    protected:
    void SetUp() override {
        size = 100;
        dt = 0.1;
        temperature = std::make_shared<ComputeTemperature>();

        // Initialize the material points
        MaterialPointsFactory::getInstance();
        std::vector<MaterialPoint> mPoints;

        Real dx = 2.0 / (size -1);
        Real dy = 2.0 / (size -1);
        for (UInt j = 0; j < size; j++){
            for (UInt i = 0; i < size; i++){
                MaterialPoint p;
                p.getTemperature() = 0.0;
                p.getMass() = 1.; // mass density 
                p.getHeatRate() = 0.0;
                p.getPosition()[0] = -1.0 + j * dx; // x coordinate
                p.getPosition()[1] = -1.0 + i * dy; // y coordinate
                mPoints.push_back(p);
            }
        }

        // add to system
        for (auto& p: mPoints){
            system.addParticle(std::make_shared<MaterialPoint>(p));

        }
  }

    System system;
    UInt size;
    double dt;
    std::shared_ptr<ComputeTemperature> temperature;
};

/*****************************************************************
TEST_F(MaterialPointsSystem, homogeneousNoHeat) {
    // default system is homogeneous

    temperature->setDelta(dt);
    temperature->setHeatCapacity(1);
    // run 1000 iterations
    for (int i = 0; i < 1000; ++i){
        temperature->compute(system);
        for (auto &p: system){
            ASSERT_NEAR(static_cast<MaterialPoint&>(p).getTemperature(), 0.0, 1e-10);
        }
    }
}
/*****************************************************************

TEST_F(MaterialPointsSystem, sinHeat) {
    // initial system is in homogeneous temperature

    // set sin heat flux
    Real L = 1;
    for (UInt j = 0; j < size; j++){
        for (UInt i = 0; i < size; i++){
            MaterialPoint& p = dynamic_cast<MaterialPoint&>(system.getParticle(j*size+i));
            p.getHeatRate() = pow(2.0*M_PI/L, 2) * sin(2.0*M_PI*p.getPosition()[0]/L);
        }
    }

    temperature->setDelta(dt);
    temperature->setHeatCapacity(1.0);
    temperature->setK(1.0);

    // run 1000 iterations
    for (int i = 0; i < 1000; ++i){
        temperature->compute(system);
    }


    // check temperature when reach equilibrium
    for (UInt j = 0; j < size; j++){
        for (UInt i = 0; i < size; i++){
            MaterialPoint& p = dynamic_cast<MaterialPoint&>(system.getParticle(j*size+i));
            ASSERT_NEAR(p.getTemperature(), sin(2.0 * M_PI * p.getPosition()[0] / L), 1e-10);
            // std::cout << p << std::endl;
        }
    }
}

/*****************************************************************
TEST_F(MaterialPointsSystem, stepHeat) {
    // initial system is in homogeneous temperature

    // set step heat flux
    for (UInt j = 0; j < size; j++){
        for (UInt i = 0; i < size; i++){
            MaterialPoint& p = dynamic_cast<MaterialPoint&>(system.getParticle(j*size+i));
            if (j == size/4){
                p.getHeatRate() = -1.0;
            }else if (j == 3 * size / 4){
                p.getHeatRate() = 1.0;
            }else{
                p.getHeatRate() = 0.0;
            }
        }
    }

    temperature->setDelta(dt);
    temperature->setHeatCapacity(1.0);
    temperature->setK(1.0);
    // run 1000 iterations
    for (int i = 0; i < 1000; ++i){
        temperature->compute(system);
    }


    // check temperature when reach equilibrium
    // TODO: temperature seems to be nan
    for (UInt j = 10; j < size; j++){
        for (UInt i = 10; i < size; i++){
            MaterialPoint& p = dynamic_cast<MaterialPoint&>(system.getParticle(j*size+i));
            if (j <= size/4){
                ASSERT_NEAR(p.getTemperature(), -1.0 - p.getPosition()[0], 1e-10);
            }else if (j > size / 4 && j <= 3 * size/4){
                ASSERT_NEAR(p.getTemperature(), p.getPosition()[0], 1e-10);
            }else{
                ASSERT_NEAR(p.getTemperature(), + 1.0 - p.getPosition()[0], 1e-10);
            }
        }
    }
}

/*****************************************************************
TEST_F(MaterialPointsSystem, radialHeat) {
    // initial system is in homogeneous temperature

    // set radial heat flux
    Real R = 0.5;
    for (UInt j = 0; j < size; j++){
        for (UInt i = 0; i < size; i++){
            MaterialPoint& p = dynamic_cast<MaterialPoint&>(system.getParticle(j*size+i));
            Real x = p.getPosition()[0];
            Real y = p.getPosition()[1];
            if ((pow(x,2) + pow(y,2)) < R){
                p.getHeatRate() = 1.0;
            }else{
                p.getHeatRate() = 0.0;
            }
        }
    }

    temperature->setDelta(dt);
    temperature->setHeatCapacity(1.0);
    temperature->setK(1.0);
    // run 1000 iterations
    for (int i = 0; i < 1000; ++i){
        temperature->compute(system);
    }
}
*****************************OLD************************************/