#include <pybind11/pybind11.h>

namespace py = pybind11;

#include "compute_gravity.hh"
#include "compute_temperature.hh"
#include "compute_verlet_integration.hh"
#include "csv_writer.hh"
#include "material_points_factory.hh"
#include "ping_pong_balls_factory.hh"
#include "planets_factory.hh"

PYBIND11_MODULE(pypart, m) {

  m.doc() = "pybind of the Particles project";

  // bind the routines here
  // wrap the class
  py::class_<ParticlesFactoryInterface>(m, "ParticlesFactoryInterface")
      // .def(py::init()) // no python constructor
      .def("getInstance",
           &ParticlesFactoryInterface::getInstance)  // getInstance method
      .def("createSimulation",
           [](const std::string& fname, Real timestep,
              py::function createComputes) {
             createComputes(ParticlesFactoryInterface::getInstance(), timestep);
             ParticlesFactoryInterface::getInstance().createSimulation(
                 fname, timestep);
           })
      .def_property_readonly(
          "system_evolution",
          &ParticlesFactoryInterface::getSystemEvolution);  // createSimulation
                                                            // method

  py::class_<MaterialPointsFactory, ParticlesFactoryInterface>(
      m, "MaterialPointsFactory")
      // .def(py::init()) // no python constructor
      .def("getInstance",
           &MaterialPointsFactory::getInstance)  // getInstance method
      .def("createSimulation", [](const std::string& fname, Real timestep,
                                  py::function createComputes) {
        createComputes(MaterialPointsFactory::getInstance(), timestep);
        MaterialPointsFactory::getInstance().createSimulation(fname, timestep);
      });  // createSimulation method

  py::class_<PlanetsFactory, ParticlesFactoryInterface>(m, "PlanetsFactory")
      // .def(py::init()) // no python constructor
      .def("getInstance", &PlanetsFactory::getInstance)  // getInstance method
      .def("createSimulation", [](const std::string& fname, Real timestep,
                                  py::function createComputes) {
        createComputes(PlanetsFactory::getInstance(), timestep);
        PlanetsFactory::getInstance().createSimulation(fname, timestep);
      });  // createSimulation method

  py::class_<PingPongBallsFactory, ParticlesFactoryInterface>(
      m, "PingPongBallsFactory")
      // .def(py::init()) // no python constructor
      .def("getInstance",
           &PingPongBallsFactory::getInstance)  // getInstance method
      .def("createSimulation", [](const std::string& fname, Real timestep,
                                  py::function createComputes) {
        createComputes(PingPongBallsFactory::getInstance(), timestep);
        PingPongBallsFactory::getInstance().createSimulation(fname, timestep);
      });  // createSimulation method

  // binding for CsvWriter class
  py::class_<CsvWriter>(m, "CsvWriter")
      .def(py::init<const std::string&>())
      .def("write", &CsvWriter::write);

  // binding for Compute class
  py::class_<Compute>(m, "Compute");

  // binding for ComputeInteraction class
  py::class_<ComputeInteraction, Compute>(m, "ComputeInteraction");

  // binding for ComputeGravity class
  py::class_<ComputeGravity, Compute>(m, "ComputeGravity")
      .def(py::init())
      .def("setG", &ComputeGravity::setG);

  // binding for ComputeTemperature class
  py::class_<ComputeTemperature, Compute>(m, "ComputeTemperature")
      .def(py::init<>())
      .def_property("conductivity", &ComputeTemperature::getConductivity,
                    [](ComputeTemperature& self, Real newConduct) {
                      self.getConductivity() = newConduct;
                    })
      // .def_readwrite("conductivity", &ComputeTemperature::getConductivity)
      .def_property(
          "L", &ComputeTemperature::getL,
          [](ComputeTemperature& self, Real newL) { self.getL() = newL; })
      .def_property("capacity", &ComputeTemperature::getCapacity,
                    [](ComputeTemperature& self, Real newCap) {
                      self.getCapacity() = newCap;
                    })
      .def_property("density", &ComputeTemperature::getDensity,
                    [](ComputeTemperature& self, Real newDensi) {
                      self.getDensity() = newDensi;
                    })
      .def_property("deltat", &ComputeTemperature::getDeltat,
                    [](ComputeTemperature& self, Real newDelta) {
                      self.getDeltat() = newDelta;
                    });

  // binding for ComputeVerletIntergration class
  py::class_<ComputeVerletIntegration, Compute>(m, "ComputeVerletIntegration")
      .def(py::init<Real>()) // TODO
      .def("addInteraction", &ComputeVerletIntegration::addInteraction);

  // binding for SystemEvolution
  py::class_<SystemEvolution>(m, "SystemEvolution")
      .def("getSystem", &SystemEvolution::getSystem)
      .def("evolve", &SystemEvolution::evolve)
      .def("setNSteps", &SystemEvolution::setNSteps)
      .def("setDumpFreq", &SystemEvolution::setDumpFreq)
      .def("addCompute", &SystemEvolution::addCompute);
}
