/*
  Example melting a snowman at different speeds for each of its spheres.
  Each sphere is treated as a different material by the advection kernel.
*/

#include <iostream>
#include <array>

#include <lsBooleanOperation.hpp>
#include <lsDomain.hpp>
#include <lsExpand.hpp>
#include <lsGeometries.hpp>
#include <lsMakeGeometry.hpp>
#include <lsPrune.hpp>
#include <lsToSurfaceMesh.hpp>
#include <lsVTKWriter.hpp>
#include <lsAdvect.hpp>

// Implement own velocity field
class velocityField : public lsVelocityField<double> {
public:
  double getScalarVelocity(
      const std::array<double, 3> & /*coordinate*/, int material,
      const std::array<double, 3> &/*normalVector = hrleVectorType<double, 3>(0.)*/) {
    // Some arbitrary velocity function of your liking
    // (try changing it and see what happens :)
    double velocity;
    switch(material){
      case 0: velocity = -1.8;  // -1.8 per second
      break;
      case 1: velocity = -1.3;
      break;
      case 2: velocity = -1;
    }
    return velocity;
  }
};

int main() {

  constexpr int D = 3;
  omp_set_num_threads(4);

  double gridDelta = 0.25;

  lsDomain_double_3 sphere1(gridDelta);
  lsDomain_double_3 sphere2(gridDelta);
  lsDomain_double_3 sphere3(gridDelta);

  double origin[3] = {0., 0., 0.};
  double radius = 10.0;

  lsMakeGeometry<double, D>(sphere1, lsSphere<double, D>(origin, radius)).apply();
  origin[2] = 12.0;
  radius = 7.2;
  lsMakeGeometry<double, D>(sphere2, lsSphere<double, D>(origin, radius)).apply();
  origin[2] = 20.0;
  radius = 5.0;
  lsMakeGeometry<double, D>(sphere3, lsSphere<double, D>(origin, radius)).apply();

  // Perform boolean operations
  lsBooleanOperation<double, D>(sphere2, sphere3, lsBooleanOperationEnum::UNION).apply();
  lsBooleanOperation<double, D>(sphere1, sphere2, lsBooleanOperationEnum::UNION).apply();

  std::cout << "Extracting..." << std::endl;
  {
    lsMesh mesh1, mesh2, mesh3;

    lsToSurfaceMesh<double, D>(sphere1, mesh1).apply();
    lsToSurfaceMesh<double, D>(sphere2, mesh2).apply();
    lsToSurfaceMesh<double, D>(sphere3, mesh3).apply();

    lsVTKWriter(mesh1, "sphere1-0.vtk").apply();
    lsVTKWriter(mesh2, "sphere2-0.vtk").apply();
    lsVTKWriter(mesh3, "sphere3-0.vtk").apply();
  }

  velocityField velocities;

  std::cout << "Advecting" << std::endl;
  lsAdvect<double, D> advectionKernel;

  // set velocity field
  advectionKernel.setVelocityField(velocities);

  // insert all used level sets
  advectionKernel.insertNextLevelSet(sphere3);
  advectionKernel.insertNextLevelSet(sphere2);
  advectionKernel.insertNextLevelSet(sphere1);

  // Advect the snowman to have 1 second pass in real time
  advectionKernel.setAdvectionTime(1.0);
  advectionKernel.apply();
  double advectionSteps = advectionKernel.getNumberOfTimeSteps();
  std::cout << "Number of Advection steps taken: " << advectionSteps
            << std::endl;

  {
    lsMesh mesh1, mesh2, mesh3;

    std::cout << "Extracting..." << std::endl;
    lsToSurfaceMesh<double, D>(sphere1, mesh1).apply();
    lsToSurfaceMesh<double, D>(sphere2, mesh2).apply();
    lsToSurfaceMesh<double, D>(sphere3, mesh3).apply();

    lsVTKWriter(mesh1, "sphere1-1.vtk").apply();
    lsVTKWriter(mesh2, "sphere2-1.vtk").apply();
    lsVTKWriter(mesh3, "sphere3-1.vtk").apply();
  }

  return 0;
}
