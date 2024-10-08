cmake_minimum_required(VERSION 3.30.3)
project(BajaCar VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

include_directories(
        ${CMAKE_SOURCE_DIR}/src/CarComputer/src
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src/DevLib/IMU/WrapperLibrary
        ${CMAKE_SOURCE_DIR}/src/CarComputer/include)


# ! Apparently using GLOB is discouraged in cmake. It's working for now, but I guess I need to figure out
#   the correct way to do it.

# TEMPORARILY GRAB THE IMU LIBRARY
file(GLOB TEMP_IMU ${CMAKE_SOURCE_DIR}/src/DevLib/IMU/WrapperLibrary/*.cpp)


# Grab all the .cpp files in src
file(GLOB SRC_FILES ${CMAKE_SOURCE_DIR}/src/CarComputer/src/*.cpp)
# Grab all the .cpp files in subsystems recursively
file(GLOB_RECURSE SUBSYSTEM_FILES ${CMAKE_SOURCE_DIR}/src/CarComputer/src/subsystems/*.cpp)
# Grab all the .cpp files in commands recursively
file(GLOB_RECURSE PROCEDURE_FILES ${CMAKE_SOURCE_DIR}/src/CarComputer/src/procedures/*.cpp)

set(SOURCE_FILES ${SRC_FILES} ${SUBSYSTEM_FILES} ${PROCEDURE_FILES} ${TEMP_IMU})

add_executable(carComputer ${SOURCE_FILES})


# Make sure we can use multithreading
target_link_libraries(carComputer PRIVATE pthread)