import viennals3d as vls


# Implement own velocity field
class velocityField(vls.lsVelocityField):
    def getScalarVelocity(self, coord, material, normal, pointId):
        # Some arbitrary velocity function of your liking
        # (try changing it and see what happens :)
        velocity = 0.0

        if material == 0:
            velocity = -1.8  # -1.8 per second
        elif material == 1:
            velocity = -1.3
        else:
            velocity = -1.0

        return velocity


gridDelta = 0.25
extent = 30

bounds = (-extent, extent, -extent, extent)
boundaryCons = (0, 1, 0)  # 0 = reflective, 1 = infinite, 2 = periodic

sphere1 = vls.lsDomain(gridDelta)
sphere2 = vls.lsDomain(gridDelta)
sphere3 = vls.lsDomain(gridDelta)

origin = [0.0, 0.0, 0.0]
radius = 10.0

vls.lsMakeGeometry(sphere1, vls.lsSphere(origin, radius)).apply()

origin[2] = 12.0
radius = 7.2

vls.lsMakeGeometry(sphere2, vls.lsSphere(origin, radius)).apply()

origin[2] = 20.0
radius = 5.0

vls.lsMakeGeometry(sphere3, vls.lsSphere(origin, radius)).apply()

# Perform boolean operations
vls.lsBooleanOperation(sphere2, sphere3, vls.lsBooleanOperationEnum.UNION).apply()
vls.lsBooleanOperation(sphere1, sphere2, vls.lsBooleanOperationEnum.UNION).apply()

print("Extracting...")
mesh = vls.lsMesh()
vls.lsToSurfaceMesh(sphere1, mesh).apply()
vls.lsVTKWriter(mesh, "sphere1-0.vtp").apply()
vls.lsToSurfaceMesh(sphere2, mesh).apply()
vls.lsVTKWriter(mesh, "sphere2-0.vtp").apply()
vls.lsToSurfaceMesh(sphere3, mesh).apply()
vls.lsVTKWriter(mesh, "sphere3-0.vtp").apply()

velocities = velocityField()

print("Advecting")
advectionKernel = vls.lsAdvect()

# set velocity field
advectionKernel.setVelocityField(velocities)

# insert all used level sets
advectionKernel.insertNextLevelSet(sphere3)
advectionKernel.insertNextLevelSet(sphere2)
advectionKernel.insertNextLevelSet(sphere1)

# Advect the snowman to have 1 second pass in real time
advectionKernel.setAdvectionTime(1.0)
advectionKernel.apply()
advectionSteps = advectionKernel.getNumberOfTimeSteps()
print("Number of Advection steps taken:", advectionSteps)

print("Extracting...")
mesh = vls.lsMesh()
vls.lsToSurfaceMesh(sphere1, mesh).apply()
vls.lsVTKWriter(mesh, "sphere1-1.vtp").apply()
vls.lsToSurfaceMesh(sphere2, mesh).apply()
vls.lsVTKWriter(mesh, "sphere2-1.vtp").apply()
vls.lsToSurfaceMesh(sphere3, mesh).apply()
vls.lsVTKWriter(mesh, "sphere3-1.vtp").apply()
