# Car Computer

### This document will provide an overview for the structure of the Car Computer program.

The Car Computer program is the heart of the Baja Electrical telemetry system. Without this program, we would not be able to get any data. There are many different aspects of the program that are important to understand, all of which will be described on a high level here.

* [Build System](#build-bystem)
* [Code Architecture Overview](#code-architecture-overview)
* [CAN Setup](#can-setup)
* [Radio Setup](#radio-setup)
* [Data Storage](#data-storage)
* [Devices](#devices)
* [Subsystems](#subsystems)
* [Procedures](#procedures)
* [Car Container](#car-container)

#### Build System

The build system used for this project is [CMake](https://cmake.org/).

In order to build and run the project, run the following commands in the build directory of the repository.


> [!NOTE]
> Test the note here

`$ cmake ..`

`$ make`

`$ ./carComputer`

#### Code Architecture Overview

#### CAN Setup

#### Radio Setup

#### Data Storage

#### Devices

#### Subsystems

#### Procedures

#### Car Container

One potential issue with the way I'm doing things right now is the fact that subsystems can be accessed by multiple
procedures at the same time. In theory this is nice, because if two procedures wanted to read the RPM they could,
but if there is a subsystem where you can set values, and multiple procedures are both trying to set values at once,
this could be a problem.

Maybe there could be a flag so that if the user wants, a subsystem can only be accessible by one procedure at a time
(the dash for example)
