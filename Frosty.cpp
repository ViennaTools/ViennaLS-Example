/*
  Example melting a snowman at different speeds for each of its spheres.
  Each sphere is treated as a different material by the advection kernel.
*/

#include <iostream>

#include <lsBooleanOperation.hpp>
#include <lsDomain.hpp>
#include <lsExpand.hpp>
#include <lsMakeGeometry.hpp>
#include <lsPrune.hpp>
#include <lsToExplicitMesh.hpp>
#include <lsVTKWriter.hpp>
#include <lsAdvect.hpp>

// implement own velocity field
class velocityField : public lsVelocityField<double> {
public:
  double getScalarVelocity(
      hrleVectorType<double, 3> /*coordinate*/, int material,
      hrleVectorType<double, 3> /*normalVector = hrleVectorType<double, 3>(0.)*/) {
    // Some arbitrary velocity function of your liking
    // (try changing it and see what happens :)
    double velocity;
    switch(material){
      case 0: velocity = -1.8;
      break;
      case 1: velocity = -1.3;
      break;
      case 2: velocity = -1;
    }
    return velocity;
  }

  hrleVectorType<double, 3> getVectorVelocity(
      hrleVectorType<double, 3> /*coordinate*/, int /*material*/,
      hrleVectorType<double,
                     3> /*normalVector = hrleVectorType<double, 3>(0.)*/) {
    return hrleVectorType<double, 3>(0.);
  }
};

int main() {

  constexpr int D = 3;
  omp_set_num_threads(6);

  double gridDelta = 0.25;

  lsDomain_double_3 sphere1(gridDelta);
  lsDomain_double_3 sphere2(gridDelta);
  lsDomain_double_3 sphere3(gridDelta);

  double origin[3] = {0., 0., 0.};
  double radius = 10.0;

  lsMakeGeometry<double, D>(sphere1).makeSphere(origin, radius);
  origin[2] = 12.0;
  radius = 7.2;
  lsMakeGeometry<double, D>(sphere2).makeSphere(origin, radius);
  origin[2] = 20.0;
  radius = 5.0;
  lsMakeGeometry<double, D>(sphere3).makeSphere(origin, radius);

  // Perform a boolean operation
  lsBooleanOperation<double, D>(sphere2).OR(sphere3);
  lsBooleanOperation<double, D>(sphere1).OR(sphere2);

  std::cout << "Extracting..." << std::endl;
  {
    lsMesh mesh1, mesh2, mesh3;

    std::cout << "Extracting..." << std::endl;
    lsToExplicitMesh<double, D>(sphere1, mesh1).apply();
    lsToExplicitMesh<double, D>(sphere2, mesh2).apply();
    lsToExplicitMesh<double, D>(sphere3, mesh3).apply();

    lsVTKWriter(mesh1).writeVTKLegacy("sphere1-0.vtk");
    lsVTKWriter(mesh2).writeVTKLegacy("sphere2-0.vtk");
    lsVTKWriter(mesh3).writeVTKLegacy("sphere3-0.vtk");
  }


  // fill vector with lsDomain pointers
  std::vector<lsDomain_double_3 *> lsDomains;
  lsDomains.push_back(&sphere3);
  lsDomains.push_back(&sphere2);
  lsDomains.push_back(&sphere1);

  velocityField velocities;

  std::cout << "Advecting" << std::endl;
  lsAdvect<double, D> advection(lsDomains, velocities);
  // advection.setIntegrationScheme(1);
  advection.setCalculateNormalVectors(false);
  double advectionSteps = advection.apply(1.0);
  std::cout << "Number of Advection steps taken: " << advectionSteps
            << std::endl;

  std::cout << "Extracting..." << std::endl;
  {
    lsMesh mesh1, mesh2, mesh3;

    std::cout << "Extracting..." << std::endl;
    lsToExplicitMesh<double, D>(sphere1, mesh1).apply();
    lsToExplicitMesh<double, D>(sphere2, mesh2).apply();
    lsToExplicitMesh<double, D>(sphere3, mesh3).apply();

    lsVTKWriter(mesh1).writeVTKLegacy("sphere1-1.vtk");
    lsVTKWriter(mesh2).writeVTKLegacy("sphere2-1.vtk");
    lsVTKWriter(mesh3).writeVTKLegacy("sphere3-1.vtk");
  }

  return 0;
}
