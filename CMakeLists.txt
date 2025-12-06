cmake_minimum_required(VERSION 3.0.0)
project(BajaCar VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

include_directories(
        #Main Sources
        ${CMAKE_SOURCE_DIR}/src/CarMain/src
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src/CarMain/include
        #Wrapper Libraries
        ${CMAKE_SOURCE_DIR}/src/CarMain/src/devices/include
        #Coms
        ${CMAKE_SOURCE_DIR}/src/CarMain/src/coms/include
        #Subsystems
        ${CMAKE_SOURCE_DIR}/src/CarMain/src/subsystems/include
)


# ! Apparently using GLOB is discouraged in cmake. It's working for now, but I guess I need to figure out
#   the correct way to do it.

# Add the wrapper library cpps
file(GLOB WRAPPER_FILES ${CMAKE_SOURCE_DIR}/src/CarMain/src/devices/*.cpp)
# Grab all the .cpp files in src
file(GLOB SRC_FILES ${CMAKE_SOURCE_DIR}/src/CarMain/src/*.cpp)
# Grab all the .cpp files in subsystems recursively
file(GLOB_RECURSE SUBSYSTEM_FILES ${CMAKE_SOURCE_DIR}/src/CarMain/src/subsystems/*.cpp)
# Grab all the .cpp files in commands recursively
file(GLOB_RECURSE PROCEDURE_FILES ${CMAKE_SOURCE_DIR}/src/CarMain/src/procedures/*.cpp)
# Grab all the .cpp files in coms recursively
file(GLOB_RECURSE COMS_FILES ${CMAKE_SOURCE_DIR}/src/CarMain/src/coms/*.cpp)

set(SOURCE_FILES ${SRC_FILES} ${SUBSYSTEM_FILES} ${PROCEDURE_FILES} ${COMS_FILES} ${WRAPPER_FILES})

add_executable(carComputer ${SOURCE_FILES})


# Make sure we can use multithreading
target_link_libraries(carComputer PRIVATE pthread)
# SQLite library
target_link_libraries(carComputer PRIVATE sqlite3)
# radio library
target_link_libraries(carComputer PRIVATE rf24)



# Chat guide for installing
include(GNUInstallDirs)        # gives you ${CMAKE_INSTALL_BINDIR} → usually “bin”

install(TARGETS carComputer    # what to install
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})   # where to put the executable