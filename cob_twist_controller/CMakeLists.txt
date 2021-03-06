cmake_minimum_required(VERSION 2.8.3)
project(cob_twist_controller)

# Load catkin and all dependencies required for this package
find_package(catkin REQUIRED COMPONENTS cmake_modules roscpp sensor_msgs geometry_msgs nav_msgs visualization_msgs kdl_parser kdl_conversions eigen_conversions tf tf_conversions dynamic_reconfigure cob_obstacle_distance)

find_package(Eigen REQUIRED)
add_definitions(${EIGEN_DEFINITIONS})

find_package(orocos_kdl REQUIRED)

generate_dynamic_reconfigure_options(
  cfg/TwistController.cfg
)

# Declare catkin package
catkin_package(
  INCLUDE_DIRS include
  LIBRARIES twist_controller inverse_differential_kinematics_solver damping_methods constraint_solvers limiters
  CATKIN_DEPENDS roscpp sensor_msgs geometry_msgs nav_msgs visualization_msgs cob_obstacle_distance
  DEPENDS eigen
)

include_directories(include ${catkin_INCLUDE_DIRS} ${EIGEN_INCLUDE_DIRS} ${orocos_kdl_INCLUDE_DIRS} )


## Build
set(SRC_C_DIR "src/constraint_solvers")
set(SRC_CS_DIR "${SRC_C_DIR}/solvers")

# Declare a cpp library
add_library(damping_methods src/damping_methods/damping.cpp)
add_library(inv_calculations src/inverse_jacobian_calculations/inverse_jacobian_calculation.cpp)
add_library(constraint_solvers ${SRC_CS_DIR}/unconstraint_solver.cpp ${SRC_CS_DIR}/wln_joint_limit_avoidance_solver.cpp ${SRC_CS_DIR}/weighted_least_norm_solver.cpp ${SRC_CS_DIR}/gradient_projection_method_solver.cpp ${SRC_C_DIR}/constraint_solver_factory_builder.cpp)
add_dependencies(constraint_solvers cob_obstacle_distance_gencpp)
target_link_libraries(constraint_solvers damping_methods inv_calculations)

add_library(limiters src/limiters/limiter.cpp)
target_link_libraries(limiters ${catkin_LIBRARIES} ${orocos_kdl_LIBRARIES})

add_library(inverse_differential_kinematics_solver src/inverse_differential_kinematics_solver.cpp src/callback_data_mediator.cpp)
target_link_libraries(inverse_differential_kinematics_solver constraint_solvers ${orocos_kdl_LIBRARIES})

add_library(twist_controller src/cob_twist_controller.cpp)
add_dependencies(inverse_differential_kinematics_solver cob_twist_controller_gencfg)
target_link_libraries(twist_controller inverse_differential_kinematics_solver limiters ${catkin_LIBRARIES} ${orocos_kdl_LIBRARIES})

## Declare a cpp executable
add_executable(cob_twist_controller_node src/cob_twist_controller_node.cpp)
target_link_libraries(cob_twist_controller_node twist_controller ${catkin_LIBRARIES} ${orocos_kdl_LIBRARIES})

## Install
# Mark executables and/or libraries for installation
install(TARGETS cob_twist_controller_node twist_controller inverse_differential_kinematics_solver constraint_solvers damping_methods limiters inv_calculations
 ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
 LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
 RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

## Mark cpp header files for installation
install(DIRECTORY include/cob_twist_controller/
 DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
 FILES_MATCHING PATTERN "*.h"
 PATTERN ".svn" EXCLUDE
)

# Mark other files for installation (e.g. launch and bag files, etc.)
install(PROGRAMS scripts/test_publisher_twist.py scripts/test_publisher_twist_stamped.py scripts/test_publisher_twist_series.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}/scripts
)

