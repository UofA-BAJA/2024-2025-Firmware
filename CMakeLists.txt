cmake_minimum_required(VERSION 3.0.0)
project(BajaCar VERSION 0.1.0 LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/src/CarComputer/src)

# Grab all the .cpp files in src
file(GLOB SRC_FILES ${CMAKE_SOURCE_DIR}/src/CarComputer/src/*.cpp)
# Grab all the .cpp files in subsystems recursively
file(GLOB_RECURSE SUBSYSTEM_FILES ${CMAKE_SOURCE_DIR}/src/CarComputer/src/subsystems/*.cpp)



set(SOURCE_FILES ${SRC_FILES} ${SUBSYSTEM_FILES})


# include(CTest)
# enable_testing()

add_executable(carComputer ${SOURCE_FILES})

# set_property(TARGET BajaCar PROPERTY CXX_STANDARD 17)