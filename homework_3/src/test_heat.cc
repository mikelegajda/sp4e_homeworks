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
        for (UInt i = 0; i < size * size; ++i) {
            MaterialPoint p;
            p.getTemperature() = 1.;
            p.getMass() = 1.;
            p.getHeatRate() = 0.0;
            mPoints.push_back(p);
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
TEST_F(MaterialPointsSystem, homogeneous) {
    // default system is homogeneous

    temperature->setDelta(dt);
    temperature->setHeatCapacity(1);
    // run 1000 iterations
    for (int i = 0; i < 1000; ++i){
        temperature->compute(system);
        for (auto &p: system){
            ASSERT_NEAR(static_cast<MaterialPoint&>(p).getTemperature(), 1, 1e-10);
        }
    }
}
/*****************************************************************/
