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

### Build System

The build system used for this project is [CMake](https://cmake.org/).

In order to build and run the project, run the following commands in the build directory of the repository.

> [!NOTE]
> These commands will most likely not work unless the code is running on a raspberry pi with the correct libraries and configuration as described later in this document.

`$ cmake ..`

`$ make`

`$ ./carComputer`

The way the CmakeLists.txt file is set up is most likely not following best practices. In fact, it might not even be in a very good place in the repository. This is something that should be updated in the future.

> TODO: Insert description of how the current build system works

### Code Architecture Overview

The Car Computer code was largely inspired by the FIRST Robotics WPIlib. This library seemed easy to use, while still being largely flexible. The Car Computer code is largely intended to be easily used for new members of the Baja Electrical subteam, while abstracting away the complexity of the project for those who do not need to know how it works. The only files that should be touched by beginners are the CarContainer class, subsystems, and procedures, as those are the only classes that are needed in order to provide functionality for sensors, telemetry, live data, logging, and (in the future) the implementation of an ECVT or active suspension.

The easiest concept to understand are the *Subsystems*. Every subsystem can be thought of as a portion of the car. For example, the drivetrain subsystem is made up of all the sensors and devices that make up the drivetrain (Tachometer, wheel RPM, CVT temperature, etc). Subsystems can also be thought of as a collection of devices on the car.

*Procedures* can be thought of as individual processes that have a lifetime. Each procedure begins, runs on a clock cycle, and ends (if desired). The procedures are the part of the system that interact with the subsystems, providing logical behavior for them. For example, one procedure might interact with the drivetrain subsystem and the dash, sending RPM information to the dash. The subsystems by themselves do not (should not) have the capacity to interact with other subsystems, which is why the procedures must exist.

There are many useful utility classes written with the purpose of being used by the procedures. These classes include *Coms*, *CarLogger*, *DataStorage*, and *CarTime*. These classes will be further described in detail in the next sections of this document.

* The Coms class provides the ability to send a data stream to the pit
* The DataStorage class provides the ability to log data to the local SQL database.
* The CarLogger class provides the ability to write debug logs, warnings, errors, and panic messages to the car.log file found in /home/<user\>/car.log
* The CarTime class provides the ability to get information regarding the time in the car.

The next concept to understand is the idea of a command.

One of the more challenging concepts to understand is the idea of binding procedures to commands.




### CAN Setup

### Radio Setup

### Data Storage

### Devices

### Subsystems

### Procedures

### Car Container

One potential issue with the way I'm doing things right now is the fact that subsystems can be accessed by multiple
procedures at the same time. In theory this is nice, because if two procedures wanted to read the RPM they could,
but if there is a subsystem where you can set values, and multiple procedures are both trying to set values at once,
this could be a problem.

Maybe there could be a flag so that if the user wants, a subsystem can only be accessible by one procedure at a time
(the dash for example)
