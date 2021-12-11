#include "csv_reader.hh"
#include "csv_writer.hh"
#include "material_point.hh"
#include "material_points_factory.hh"
#include "system.hh"
#include "compute_temperature.hh"
#include <gtest/gtest.h>
#include <iostream>

/*****************************************************************/
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

/*****************************************************************/
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
/*****************************************************************/

/*****************************************************************/
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