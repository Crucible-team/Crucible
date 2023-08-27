# Install script for directory: /home/deck/Crucible/WickedEngine/BULLET

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/WickedEngine/BULLET" TYPE FILE FILES
    "/home/deck/Crucible/WickedEngine/BULLET/Bullet-C-Api.h"
    "/home/deck/Crucible/WickedEngine/BULLET/btBulletCollisionCommon.h"
    "/home/deck/Crucible/WickedEngine/BULLET/btBulletDynamicsCommon.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/WickedEngine/BULLET/BulletCollision/BroadphaseCollision" TYPE FILE FILES
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/BroadphaseCollision/btAxisSweep3.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/BroadphaseCollision/btBroadphaseInterface.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/BroadphaseCollision/btBroadphaseProxy.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/BroadphaseCollision/btCollisionAlgorithm.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/BroadphaseCollision/btDbvt.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/BroadphaseCollision/btDbvtBroadphase.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/BroadphaseCollision/btDispatcher.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/BroadphaseCollision/btMultiSapBroadphase.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/BroadphaseCollision/btOverlappingPairCache.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/BroadphaseCollision/btOverlappingPairCallback.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/BroadphaseCollision/btQuantizedBvh.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/BroadphaseCollision/btSimpleBroadphase.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/WickedEngine/BULLET/BulletCollision/CollisionDispatch" TYPE FILE FILES
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionDispatch/SphereTriangleDetector.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionDispatch/btActivatingCollisionAlgorithm.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionDispatch/btBox2dBox2dCollisionAlgorithm.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionDispatch/btBoxBoxCollisionAlgorithm.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionDispatch/btBoxBoxDetector.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionDispatch/btCollisionConfiguration.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionDispatch/btCollisionCreateFunc.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionDispatch/btCollisionDispatcher.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionDispatch/btCollisionObject.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionDispatch/btCollisionObjectWrapper.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionDispatch/btCollisionWorld.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionDispatch/btCompoundCollisionAlgorithm.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionDispatch/btCompoundCompoundCollisionAlgorithm.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionDispatch/btConvex2dConvex2dAlgorithm.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionDispatch/btConvexConcaveCollisionAlgorithm.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionDispatch/btConvexConvexAlgorithm.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionDispatch/btConvexPlaneCollisionAlgorithm.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionDispatch/btEmptyCollisionAlgorithm.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionDispatch/btGhostObject.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionDispatch/btHashedSimplePairCache.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionDispatch/btInternalEdgeUtility.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionDispatch/btManifoldResult.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionDispatch/btSimulationIslandManager.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionDispatch/btSphereBoxCollisionAlgorithm.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionDispatch/btSphereTriangleCollisionAlgorithm.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionDispatch/btUnionFind.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/WickedEngine/BULLET/BulletCollision/CollisionShapes" TYPE FILE FILES
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionShapes/btBox2dShape.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionShapes/btBoxShape.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionShapes/btCapsuleShape.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionShapes/btCollisionMargin.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionShapes/btCollisionShape.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionShapes/btCompoundShape.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionShapes/btConcaveShape.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionShapes/btConeShape.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionShapes/btConvex2dShape.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionShapes/btConvexHullShape.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionShapes/btConvexInternalShape.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionShapes/btConvexPointCloudShape.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionShapes/btConvexPolyhedron.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionShapes/btConvexShape.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionShapes/btConvexTriangleMeshShape.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionShapes/btCylinderShape.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionShapes/btEmptyShape.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionShapes/btMaterial.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionShapes/btMinkowskiSumShape.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionShapes/btMultiSphereShape.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionShapes/btMultimaterialTriangleMeshShape.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionShapes/btOptimizedBvh.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionShapes/btPolyhedralConvexShape.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionShapes/btShapeHull.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionShapes/btSphereShape.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionShapes/btStaticPlaneShape.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionShapes/btStridingMeshInterface.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionShapes/btTetrahedronShape.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionShapes/btTriangleBuffer.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionShapes/btTriangleCallback.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionShapes/btTriangleIndexVertexArray.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionShapes/btTriangleIndexVertexMaterialArray.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionShapes/btTriangleInfoMap.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionShapes/btTriangleMesh.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionShapes/btTriangleMeshShape.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionShapes/btTriangleShape.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/CollisionShapes/btUniformScalingShape.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/WickedEngine/BULLET/BulletCollision/Gimpact" TYPE FILE FILES
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/Gimpact/btBoxCollision.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/Gimpact/btClipPolygon.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/Gimpact/btCompoundFromGimpact.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/Gimpact/btContactProcessing.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/Gimpact/btGImpactBvh.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/Gimpact/btGImpactMassUtil.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/Gimpact/btGImpactQuantizedBvh.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/Gimpact/btGImpactShape.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/Gimpact/btGenericPoolAllocator.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/Gimpact/btGeometryOperations.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/Gimpact/btQuantization.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/Gimpact/btTriangleShapeEx.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/Gimpact/gim_array.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/Gimpact/gim_basic_geometry_operations.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/Gimpact/gim_bitset.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/Gimpact/gim_box_collision.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/Gimpact/gim_box_set.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/Gimpact/gim_clip_polygon.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/Gimpact/gim_contact.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/Gimpact/gim_geom_types.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/Gimpact/gim_geometry.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/Gimpact/gim_hash_table.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/Gimpact/gim_linear_math.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/Gimpact/gim_math.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/Gimpact/gim_memory.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/Gimpact/gim_radixsort.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/Gimpact/gim_tri_collision.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/WickedEngine/BULLET/BulletCollision/NarrowPhaseCollision" TYPE FILE FILES
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/NarrowPhaseCollision/btContinuousConvexCollision.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/NarrowPhaseCollision/btConvexCast.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/NarrowPhaseCollision/btConvexPenetrationDepthSolver.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/NarrowPhaseCollision/btDiscreteCollisionDetectorInterface.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/NarrowPhaseCollision/btGjkConvexCast.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/NarrowPhaseCollision/btGjkEpa2.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/NarrowPhaseCollision/btGjkEpaPenetrationDepthSolver.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/NarrowPhaseCollision/btGjkPairDetector.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/NarrowPhaseCollision/btManifoldPoint.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/NarrowPhaseCollision/btMinkowskiPenetrationDepthSolver.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/NarrowPhaseCollision/btPersistentManifold.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/NarrowPhaseCollision/btPointCollector.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/NarrowPhaseCollision/btPolyhedralContactClipping.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/NarrowPhaseCollision/btRaycastCallback.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/NarrowPhaseCollision/btSimplexSolverInterface.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/NarrowPhaseCollision/btSubSimplexConvexCast.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/WickedEngine/BULLET/BulletDynamics/Character" TYPE FILE FILES
    "/home/deck/Crucible/WickedEngine/BULLET/BulletDynamics/Character/btCharacterControllerInterface.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletDynamics/Character/btKinematicCharacterController.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/WickedEngine/BULLET/BulletDynamics/ConstraintSolver" TYPE FILE FILES
    "/home/deck/Crucible/WickedEngine/BULLET/BulletDynamics/ConstraintSolver/btConeTwistConstraint.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletDynamics/ConstraintSolver/btConstraintSolver.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletDynamics/ConstraintSolver/btContactConstraint.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletDynamics/ConstraintSolver/btContactSolverInfo.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletDynamics/ConstraintSolver/btFixedConstraint.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletDynamics/ConstraintSolver/btGearConstraint.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletDynamics/ConstraintSolver/btGeneric6DofSpringConstraint.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletDynamics/ConstraintSolver/btHinge2Constraint.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletDynamics/ConstraintSolver/btHingeConstraint.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletDynamics/ConstraintSolver/btJacobianEntry.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletDynamics/ConstraintSolver/btPoint2PointConstraint.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletDynamics/ConstraintSolver/btSliderConstraint.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletDynamics/ConstraintSolver/btSolve2LinearConstraint.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletDynamics/ConstraintSolver/btSolverBody.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletDynamics/ConstraintSolver/btSolverConstraint.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletDynamics/ConstraintSolver/btTypedConstraint.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletDynamics/ConstraintSolver/btUniversalConstraint.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/WickedEngine/BULLET/BulletDynamics/Dynamics" TYPE FILE FILES
    "/home/deck/Crucible/WickedEngine/BULLET/BulletDynamics/Dynamics/btActionInterface.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletDynamics/Dynamics/btDynamicsWorld.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletDynamics/Dynamics/btRigidBody.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletDynamics/Dynamics/btSimpleDynamicsWorld.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/WickedEngine/BULLET/BulletDynamics/Featherstone" TYPE FILE FILES
    "/home/deck/Crucible/WickedEngine/BULLET/BulletDynamics/Featherstone/btMultiBody.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletDynamics/Featherstone/btMultiBodyConstraint.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletDynamics/Featherstone/btMultiBodyConstraintSolver.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletDynamics/Featherstone/btMultiBodyDynamicsWorld.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletDynamics/Featherstone/btMultiBodyJointLimitConstraint.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletDynamics/Featherstone/btMultiBodyJointMotor.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletDynamics/Featherstone/btMultiBodyLink.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletDynamics/Featherstone/btMultiBodyLinkCollider.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletDynamics/Featherstone/btMultiBodyPoint2Point.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletDynamics/Featherstone/btMultiBodySolverConstraint.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/WickedEngine/BULLET/BulletDynamics/MLCPSolvers" TYPE FILE FILES
    "/home/deck/Crucible/WickedEngine/BULLET/BulletDynamics/MLCPSolvers/btDantzigLCP.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletDynamics/MLCPSolvers/btDantzigSolver.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletDynamics/MLCPSolvers/btMLCPSolver.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletDynamics/MLCPSolvers/btMLCPSolverInterface.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletDynamics/MLCPSolvers/btPATHSolver.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletDynamics/MLCPSolvers/btSolveProjectedGaussSeidel.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/WickedEngine/BULLET/BulletDynamics/Vehicle" TYPE FILE FILES
    "/home/deck/Crucible/WickedEngine/BULLET/BulletDynamics/Vehicle/btRaycastVehicle.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletDynamics/Vehicle/btVehicleRaycaster.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletDynamics/Vehicle/btWheelInfo.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/WickedEngine/BULLET/BulletSoftBody" TYPE FILE FILES
    "/home/deck/Crucible/WickedEngine/BULLET/BulletSoftBody/btDefaultSoftBodySolver.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletSoftBody/btSoftBody.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletSoftBody/btSoftBodyConcaveCollisionAlgorithm.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletSoftBody/btSoftBodyData.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletSoftBody/btSoftBodyHelpers.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletSoftBody/btSoftBodyInternals.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletSoftBody/btSoftBodySolverVertexBuffer.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletSoftBody/btSoftBodySolvers.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletSoftBody/btSoftRigidCollisionAlgorithm.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletSoftBody/btSoftRigidDynamicsWorld.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletSoftBody/btSoftSoftCollisionAlgorithm.h"
    "/home/deck/Crucible/WickedEngine/BULLET/BulletSoftBody/btSparseSDF.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/WickedEngine/BULLET/LinearMath" TYPE FILE FILES
    "/home/deck/Crucible/WickedEngine/BULLET/LinearMath/btAabbUtil2.h"
    "/home/deck/Crucible/WickedEngine/BULLET/LinearMath/btAlignedAllocator.h"
    "/home/deck/Crucible/WickedEngine/BULLET/LinearMath/btAlignedObjectArray.h"
    "/home/deck/Crucible/WickedEngine/BULLET/LinearMath/btConvexHull.h"
    "/home/deck/Crucible/WickedEngine/BULLET/LinearMath/btConvexHullComputer.h"
    "/home/deck/Crucible/WickedEngine/BULLET/LinearMath/btDefaultMotionState.h"
    "/home/deck/Crucible/WickedEngine/BULLET/LinearMath/btGeometryUtil.h"
    "/home/deck/Crucible/WickedEngine/BULLET/LinearMath/btGrahamScan2dConvexHull.h"
    "/home/deck/Crucible/WickedEngine/BULLET/LinearMath/btHashMap.h"
    "/home/deck/Crucible/WickedEngine/BULLET/LinearMath/btIDebugDraw.h"
    "/home/deck/Crucible/WickedEngine/BULLET/LinearMath/btList.h"
    "/home/deck/Crucible/WickedEngine/BULLET/LinearMath/btMatrix3x3.h"
    "/home/deck/Crucible/WickedEngine/BULLET/LinearMath/btMatrixX.h"
    "/home/deck/Crucible/WickedEngine/BULLET/LinearMath/btMinMax.h"
    "/home/deck/Crucible/WickedEngine/BULLET/LinearMath/btMotionState.h"
    "/home/deck/Crucible/WickedEngine/BULLET/LinearMath/btPolarDecomposition.h"
    "/home/deck/Crucible/WickedEngine/BULLET/LinearMath/btPoolAllocator.h"
    "/home/deck/Crucible/WickedEngine/BULLET/LinearMath/btQuadWord.h"
    "/home/deck/Crucible/WickedEngine/BULLET/LinearMath/btQuaternion.h"
    "/home/deck/Crucible/WickedEngine/BULLET/LinearMath/btQuickprof.h"
    "/home/deck/Crucible/WickedEngine/BULLET/LinearMath/btRandom.h"
    "/home/deck/Crucible/WickedEngine/BULLET/LinearMath/btScalar.h"
    "/home/deck/Crucible/WickedEngine/BULLET/LinearMath/btSerializer.h"
    "/home/deck/Crucible/WickedEngine/BULLET/LinearMath/btStackAlloc.h"
    "/home/deck/Crucible/WickedEngine/BULLET/LinearMath/btTransform.h"
    "/home/deck/Crucible/WickedEngine/BULLET/LinearMath/btTransformUtil.h"
    "/home/deck/Crucible/WickedEngine/BULLET/LinearMath/btVector3.h"
    )
endif()

