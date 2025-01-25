cmake_minimum_required(VERSION 3.0.0)
project(BajaCar VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

include_directories(
        ${CMAKE_SOURCE_DIR}/src/CarComputer/src
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src/DevLib/IMU/WrapperLibrary
        ${CMAKE_SOURCE_DIR}/src/DevLib/Dash/WrapperLibrary
        ${CMAKE_SOURCE_DIR}/src/DevLib/Temperature/WrapperLibrary
        ${CMAKE_SOURCE_DIR}/src/DevLib/Tachometer/WrapperLibrary
        ${CMAKE_SOURCE_DIR}/src/CarComputer/include)


# ! Apparently using GLOB is discouraged in cmake. It's working for now, but I guess I need to figure out
#   the correct way to do it.

# TEMPORARILY GRAB THE IMU LIBRARY
file(GLOB TEMP_IMU ${CMAKE_SOURCE_DIR}/src/DevLib/IMU/WrapperLibrary/*.cpp)
file(GLOB TEMP_DASH ${CMAKE_SOURCE_DIR}/src/DevLib/Dash/WrapperLibrary/*.cpp)
file(GLOB TEMP_TEMPERATURE ${CMAKE_SOURCE_DIR}/src/DevLib/Temperature/WrapperLibrary/*.cpp)
file(GLOB TEMP_TACHOMETER ${CMAKE_SOURCE_DIR}/src/DevLib/Tachometer/WrapperLibrary/*.cpp)


# Grab all the .cpp files in src
file(GLOB SRC_FILES ${CMAKE_SOURCE_DIR}/src/CarComputer/src/*.cpp)
# Grab all the .cpp files in subsystems recursively
file(GLOB_RECURSE SUBSYSTEM_FILES ${CMAKE_SOURCE_DIR}/src/CarComputer/src/subsystems/*.cpp)
# Grab all the .cpp files in commands recursively
file(GLOB_RECURSE PROCEDURE_FILES ${CMAKE_SOURCE_DIR}/src/CarComputer/src/procedures/*.cpp)
# Grab all the .cpp files in coms recursively
file(GLOB_RECURSE COMS_FILES ${CMAKE_SOURCE_DIR}/src/CarComputer/src/coms/*.cpp)

set(SOURCE_FILES ${SRC_FILES} ${SUBSYSTEM_FILES} ${PROCEDURE_FILES} ${COMS_FILES} ${TEMP_IMU} ${TEMP_TEMPERATURE} ${TEMP_DASH} ${TEMP_TACHOMETER})

add_executable(carComputer ${SOURCE_FILES})


# Make sure we can use multithreading
target_link_libraries(carComputer PRIVATE pthread)
# SQLite library
target_link_libraries(carComputer PRIVATE sqlite3)
# radio library
target_link_libraries(carComputer PRIVATE rf24)