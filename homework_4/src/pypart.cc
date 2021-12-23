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
      .def("getInstance", &ParticlesFactoryInterface::getInstance,
           py::return_value_policy::reference)  // getInstance method, cannot
                                                // copy singleton
    //   .def("createSimulation",
    //        [](ParticlesFactoryInterface& self, const std::string& fname,
    //           Real timestep, py::function createComputes) {
    //          auto ret = &self.createSimulation(fname, timestep);
    //          createComputes(self, timestep);
    //          return ret;
    //        })
      .def_property_readonly("system_evolution",
                             &ParticlesFactoryInterface::getSystemEvolution);

  py::class_<MaterialPointsFactory, ParticlesFactoryInterface>(
      m, "MaterialPointsFactory")
      // .def(py::init()) // no python constructor
      .def("getInstance", &MaterialPointsFactory::getInstance,
           py::return_value_policy::reference)  // getInstance method
      .def("createSimulation",
           [](MaterialPointsFactory& self, const std::string& fname,
              Real timestep, py::function createComputes) {
             // have to call this method first to instantiate system_evolution
             auto ret = &self.createSimulation(fname, timestep); 

             createComputes(self, timestep);
             return ret;
           });  // createSimulation method

  py::class_<PlanetsFactory, ParticlesFactoryInterface>(m, "PlanetsFactory")
      // .def(py::init()) // no python constructor
      .def("getInstance", &PlanetsFactory::getInstance,
           py::return_value_policy::reference)  // getInstance method
      .def("createSimulation",
           [](PlanetsFactory& self, const std::string& fname, Real timestep,
              py::function createComputes) {
             auto ret = &self.createSimulation(fname, timestep);
             createComputes(self, timestep);
             return ret;
           });  // createSimulation method

  py::class_<PingPongBallsFactory, ParticlesFactoryInterface>(
      m, "PingPongBallsFactory")
      // .def(py::init()) // no python constructor
      .def("getInstance", &PingPongBallsFactory::getInstance,
           py::return_value_policy::reference)  // getInstance method
      .def("createSimulation",
           [](PingPongBallsFactory& self, const std::string& fname,
              Real timestep, py::function createComputes) {
             auto ret = &self.createSimulation(fname, timestep);
             createComputes(self, timestep);
             return ret;
           });  // createSimulation method

  // binding for CsvWriter class
  py::class_<CsvWriter>(m, "CsvWriter")
      .def(py::init<const std::string&>())
      .def("write", &CsvWriter::write);

  // binding for Compute class
  py::class_<Compute,
             std::shared_ptr<
                 Compute> /* holder type, default is std::unique_ptr<Type>*/>(
      m, "Compute");

  // binding for ComputeInteraction class
  py::class_<ComputeInteraction, Compute, std::shared_ptr<ComputeInteraction>>(
      m, "ComputeInteraction");

  // binding for ComputeGravity class
  py::class_<ComputeGravity, ComputeInteraction,
             std::shared_ptr<ComputeGravity>>(m, "ComputeGravity")
      .def(py::init())
      .def("setG", &ComputeGravity::setG);

  // binding for ComputeTemperature class
  py::class_<ComputeTemperature, Compute, std::shared_ptr<ComputeTemperature>>(
      m, "ComputeTemperature")
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
  py::class_<ComputeVerletIntegration, Compute,
             std::shared_ptr<ComputeVerletIntegration>>(
      m, "ComputeVerletIntegration")
      .def(py::init<Real>())
      .def("addInteraction", &ComputeVerletIntegration::addInteraction);

  // binding for SystemEvolution
  py::class_<SystemEvolution>(m, "SystemEvolution")
      .def("getSystem", &SystemEvolution::getSystem)
      .def("evolve", &SystemEvolution::evolve)
      .def("setNSteps", &SystemEvolution::setNSteps)
      .def("setDumpFreq", &SystemEvolution::setDumpFreq)
      .def("addCompute", &SystemEvolution::addCompute);

  // binding for System
  py::class_<System>(m, "System");



}
