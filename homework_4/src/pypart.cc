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
  py::class_<ParticlesFactoryInterface>(
      m, "ParticlesFactoryInterface",
      py::dynamic_attr() // to allow new members to be created dynamically
      )
      // .def(py::init()) // no python constructor
      .def("getInstance", &ParticlesFactoryInterface::getInstance) // getInstance method
      .def("createSimulation", &ParticlesFactoryInterface::createSimulation); // createSimulation method


  py::class_<MaterialPointsFactory, ParticlesFactoryInterface>(
      m, "MaterialPointsFactory",
      py::dynamic_attr() // to allow new members to be created dynamically
      )
      // .def(py::init()) // no python constructor
      .def("getInstance", &MaterialPointsFactory::getInstance) // getInstance method
      .def("createSimulation", &MaterialPointsFactory::createSimulation); // createSimulation method
  
  py::class_<PlanetsFactory, ParticlesFactoryInterface>(
      m, "PlanetsFactory",
      py::dynamic_attr() // to allow new members to be created dynamically
      )
      // .def(py::init()) // no python constructor
      .def("getInstance", &PlanetsFactory::getInstance) // getInstance method
      .def("createSimulation", &PlanetsFactory::createSimulation); // createSimulation method
}
