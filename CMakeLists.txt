cmake_minimum_required(VERSION 3.8)
project(matplotlibcpp)
if(POLICY CMP0148)
  cmake_policy(SET CMP0148 OLD)
endif()

## Normal Flags
add_compile_options(-std=c++17)
## Debugging Flags
#add_compile_options(-g3 -ggdb3 -O0 -fno-inline -fno-omit-frame-pointer)
## Release Flags
add_compile_options(-O3 -march=native -mtune=native -DNDEBUG -fno-math-errno)

##################
## Dependencies ##
##################

find_package(ament_cmake REQUIRED)

find_package(Python COMPONENTS Interpreter NumPy Development REQUIRED)
find_package(eigen3_cmake_module REQUIRED)
find_package(Eigen3 REQUIRED)

###########
## Build ##
###########

add_library(${PROJECT_NAME} STATIC src/matplotlibcpp.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC
  ${PYTHON_INCLUDE_DIRS}
  ${Python_NumPy_INCLUDE_DIRS}
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PYTHON_LIBRARIES} ${Python_NumPy_LIBRARIES})
ament_target_dependencies(${PROJECT_NAME} Python Eigen3)

############
## Export ##
############

ament_export_targets(${PROJECT_NAME}Targets HAS_LIBRARY_TARGET)
ament_export_dependencies(eigen3_cmake_module)
ament_export_dependencies(Python Eigen3)

# list(APPEND your_package_name_INCLUDE_DIRS ${Eigen3_INCLUDE_DIRS})

#############
## Install ##
#############

install(
  DIRECTORY include/
  DESTINATION include
)

install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)

#############
## Testing ##
#############

ament_package()

