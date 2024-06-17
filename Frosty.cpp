/*
  Example melting a snowman at different speeds for each of its spheres.
  Each sphere is treated as a different material by the advection kernel.
*/

#include <array>
#include <iostream>

#include <lsAdvect.hpp>
#include <lsBooleanOperation.hpp>
#include <lsDomain.hpp>
#include <lsExpand.hpp>
#include <lsGeometries.hpp>
#include <lsMakeGeometry.hpp>
#include <lsPrune.hpp>
#include <lsToSurfaceMesh.hpp>
#include <lsVTKWriter.hpp>

using namespace viennals;

// Implement own velocity field
class velocityField : public VelocityField<double> {
public:
  double getScalarVelocity(const Vec3D<double> & /*coordinate*/, int material,
                           const Vec3D<double> & /*normalVector*/,
                           unsigned long pointID) final {
    // Some arbitrary velocity function of your liking
    // (try changing it and see what happens :)
    double velocity;
    switch (material) {
    case 0:
      velocity = -1.8; // -1.8 per second
      break;
    case 1:
      velocity = -1.3;
      break;
    case 2:
      velocity = -1;
    }
    return velocity;
  }
};

int main() {
  using NumericType = double;
  constexpr int D = 3;
  omp_set_num_threads(4);

  double gridDelta = 0.25;

  auto sphere1 = SmartPointer<Domain<NumericType, D>>::New(gridDelta);
  auto sphere2 = SmartPointer<Domain<NumericType, D>>::New(gridDelta);
  auto sphere3 = SmartPointer<Domain<NumericType, D>>::New(gridDelta);

  NumericType origin[3] = {0., 0., 0.};
  NumericType radius = 10.0;

  MakeGeometry<NumericType, D>(
      sphere1, SmartPointer<Sphere<NumericType, D>>::New(origin, radius))
      .apply();
  origin[2] = 12.0;
  radius = 7.2;
  MakeGeometry<NumericType, D>(
      sphere2, SmartPointer<Sphere<NumericType, D>>::New(origin, radius))
      .apply();
  origin[2] = 20.0;
  radius = 5.0;
  MakeGeometry<NumericType, D>(
      sphere3, SmartPointer<Sphere<NumericType, D>>::New(origin, radius))
      .apply();

  // Perform boolean operations
  BooleanOperation<NumericType, D>(sphere2, sphere3,
                                   BooleanOperationEnum::UNION)
      .apply();
  BooleanOperation<NumericType, D>(sphere1, sphere2,
                                   BooleanOperationEnum::UNION)
      .apply();

  {
    auto mesh = SmartPointer<Mesh<NumericType>>::New();

    std::cout << "Extracting..." << std::endl;
    ToSurfaceMesh<NumericType, D>(sphere1, mesh).apply();
    VTKWriter<NumericType>(mesh, "sphere1-0.vtk").apply();
    ToSurfaceMesh<NumericType, D>(sphere2, mesh).apply();
    VTKWriter<NumericType>(mesh, "sphere2-0.vtk").apply();
    ToSurfaceMesh<NumericType, D>(sphere3, mesh).apply();
    VTKWriter<NumericType>(mesh, "sphere3-0.vtk").apply();
  }

  auto velocities = SmartPointer<velocityField>::New();

  std::cout << "Advecting" << std::endl;
  Advect<NumericType, D> advectionKernel;

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
    auto mesh = SmartPointer<Mesh<NumericType>>::New();

    std::cout << "Extracting..." << std::endl;
    ToSurfaceMesh<NumericType, D>(sphere1, mesh).apply();
    VTKWriter<NumericType>(mesh, "sphere1-1.vtk").apply();
    ToSurfaceMesh<NumericType, D>(sphere2, mesh).apply();
    VTKWriter<NumericType>(mesh, "sphere2-1.vtk").apply();
    ToSurfaceMesh<NumericType, D>(sphere3, mesh).apply();
    VTKWriter<NumericType>(mesh, "sphere3-1.vtk").apply();
  }

  return 0;
}
