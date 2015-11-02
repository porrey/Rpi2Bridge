// Raspberry Pi 2 to Arduino Bridge
// written by Daniel Porrey
// Version 1.0.3
// Copyright © 2015 Daniel Porrey. All Rights Reserved.
//
// ***********************************************************************
// This file is part of the RPi2 Bridge project.
// 
// RPi2 Bridge is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// RPi2 Bridge is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with RPi2 Bridge. If not, 
// see http://www.gnu.org/licenses/.
// ***********************************************************************
//
// This is a partner library for the C# library for communicating
// to the Arduino via the i2c bus from Windows 10 IoT Core on the
// Raspberry Pi 2.
//
// https://www.hackster.io/porrey/rpi2bridge
//
// The C# library is available in NuGet; ID = IoT.Arduino
//
#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#ifndef DEBUG_MODE
// ***
// *** Comment/Uncomment this line to disable/enable
// *** debugging statements to the Serial device.
// ***
//#define DEBUG_MODE
// ***
#endif

#ifndef INTERNAL1V1
// ***
// *** If not using a Mega then this will get defined. Since
// *** it is only supproted on the Mega it is mapped back
// *** INTERNAL.
// ***
#define INTERNAL1V1 INTERNAL
#endif

#ifndef INTERNAL2V56
// ***
// *** If not using a Mega then this will get defined. Since
// *** it is only supproted on the Mega it is mapped back
// *** INTERNAL.
// ***
#define INTERNAL2V56 INTERNAL
#endif

// ***
// *** Default address for the Arduino on the i2c bus
// ***
#define DEFAULT_ADDRESS 0x04

// ***
// *** Number of milliseconds to delay in the loop
// ***
#define LOOP_DELAY 10

// ***
// *** If this is set too large there may not be enogh memory
// ***
#define MAXIMUM_COMMANDS 20

// ***
// *** This currently does not need to be larger
// *** than 4. If you create custom commands,
// *** increase this value if you will return more
// *** than 4 bytes for any command.
// ***
#define MAX_OUTPUT_BUFFER 4

// ***
// *** First register ID
// ***
#define STARTING_REGISTER_ID 0

// ***
// *** Custom Register ID's start here
// ***
#define FIRST_CUSTOM_REGISTER_ID 16

#endif