### Car Computer

---

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
* Known Issues

---

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

---

### Code Architecture Overview

The Car Computer code was largely inspired by the FIRST Robotics WPIlib. This library seemed easy to use, while still being largely flexible. The Car Computer code is largely intended to be easily used for new members of the Baja Electrical subteam, while abstracting away the complexity of the project for those who do not need to know how it works. The only files that should be touched by beginners are the CarContainer class, subsystems, and procedures, as those are the only classes that are needed in order to provide functionality for sensors, telemetry, live data, logging, and (in the future) the implementation of an ECVT or active suspension.

The easiest concept to understand are the *Subsystems*. Every subsystem can be thought of as a portion of the car. For example, the drivetrain subsystem is made up of all the sensors and devices that make up the drivetrain (Tachometer, wheel RPM, CVT temperature, etc). Subsystems can also be thought of as a collection of devices on the car.

*Procedures* can be thought of as individual processes that have a lifetime. Each procedure begins, runs on a clock cycle, and ends (if desired). The procedures are the part of the system that interact with the subsystems, providing logical behavior for them. For example, one procedure might interact with the drivetrain subsystem and the dash, sending RPM information to the dash. The subsystems by themselves do not (should not) have the capacity to interact with other subsystems, which is why the procedures must exist.

There are many useful utility classes written with the purpose of being used by the procedures. These classes include *Coms*, *CarLogger*, *DataStorage*, and *CarTime*. These classes will be further described in detail in the next sections of this document.

* The Coms class provides the ability to send a data stream to the pit
* The DataStorage class provides the ability to log data to the local SQL database.
* The CarLogger class provides the ability to write debug logs, warnings, errors, and panic messages to the car.log file found in /home/<user\>/car.log
* The CarTime class provides the ability to get information regarding the time in the car.

The next concept to understand is the idea of a command. One of the more challenging concepts to understand is the idea of binding procedures to commands. Procedures are processes that run on the car and have a lifetime. As such, that has to be something that starts and ends the procedure. Commands are the way in which procedures are started and stopped. When a command is  sent from the pit GUI software, it is sent to the *procedure scheduler*, which then processes the command and manages the procedures that are *bound* to the command. When a procedure is bound to a command, it has a list of start commands and end commands. When the command is received, the procedures that are bound with that start command are started, and the procedures that are bound to that end command are ended. The easiest way to understand this concept is with an example, described in the [procedures](#procedures) section.

The place where the subsystems and procedures are defined is in the *Car Container*. The subsystems are defined in the header file of the class. They are initialized in the constructor, usually taking the *Can Dispatcher* as a constructor parameter. The procedure objects are not actually instantiated in the car container class, rather, they are bound to commands using the procedure scheduler. The syntax of doing this might seem bizarre at first. Examples of doing this is described in the [car container](#car-container) section.

---

### CAN Setup

### Radio Setup

### Data Storage

For the most part, the BAJA electrical team works to provide sensor data for the rest of the team. Typically, this data is in the form of time series plots. As established, the ability to wirelessley send these time series data streams exists, but often it is not fast, reliable, or precice (30hz wirelessley vs ~120hz locally). For these reasons, the preferred method to store data is locally. Currently, there is a 500GB solid state drive connected via a USB adapter to the Raspberry PI to store this data. Because there are USB 3.0 ports on the Raspberry PI, the read and write speeds of the SSD are not an issue.

In order to automatically mount the SSD on startup, the fstab (file system table) file must be updated. This file is located at `/etc/fstab`, and must be edited with sudo permissions.

> Insert stuff about mounting the SSD here

> ![Note]
>
> Eventually, the goal is to be able to store the data on the SSD and transfer it (lossless) via the radios when the car is stationary.

The solution to store the data on the car is via a [SQLite](https://www.sqlite.org/) database. SQLite is an excellent relational database management system to use for embedded systems, and it is relatively easy to set up.

In order to verify the installation of sqlite3, run the following command:

`$ sqlite3 --version`

If it is not installed, run the following commands:

`$ sudo apt update`

`$ sudo apt install sqlite3`

Now that sqlite3 is installed, it can be included in a cpp file by writing the line `#include <sqlite3.h>` at the top.

In order to file a cpp program that includes this header, the compiler must know to link the sqlite3 library. In a standalone program using g++, simply add the argument `-l sqlite3` when compiling. However, in order to make sure this library is linked using CMake, add the following line to the CMakeLists.txt file:

```
target_link_libraries(carComputer PRIVATE sqlite3)
```

Now it should be possible to use the sqlite3 library in the car computer program.

> Author's (Matthew) note: Databases are a quite extensive, and often very complicated topic. It can take some time to wrap your head around them. If you're trying to learn how the code works, try not to get discouraged!

The next step is to understand the structure of the database that will store all of the time series data. The folowing graphic is a schema that represents the structure of the data and how all of the tables in the database relate to each other.

> TODO: Insert a schema of the database



### Devices

### Subsystems

### Procedures

### Car Container

### Known Issues

One potential issue with the way I'm doing things right now is the fact that subsystems can be accessed by multiple
procedures at the same time. In theory this is nice, because if two procedures wanted to read the RPM they could,
but if there is a subsystem where you can set values, and multiple procedures are both trying to set values at once,
this could be a problem.

Maybe there could be a flag so that if the user wants, a subsystem can only be accessible by one procedure at a time
(the dash for example)
