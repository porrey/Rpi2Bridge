// Raspberry Pi 2 to Arduino Bridge
// written by Daniel Porrey
// Version 1.0.1
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

#include <Arduino.h>
#include "Rpi2Bridge.h"
#include "ByteConverter.h"

#ifndef BASIC_COMMANDS_H
#define BASIC_COMMANDS_H

#define BASIC_STARTING_REGISTER_ID 0

class BasicCommandsInternal
{
	public:
		// ***
		// *** Initializes the instance
		// ***
		void begin();
		
		// ***
		// *** Callbacks
		// ***
		static void pinModeCommand(int bufferSize, byte buffer[]);
		static void digitalReadCommand(int bufferSize, byte buffer[]);
		static void digitalWriteCommand(int bufferSize, byte buffer[]);
		static void analogReadCommand(int bufferSize, byte buffer[]);
		static void analogWriteCommand(int bufferSize, byte buffer[]);
		static void analogReferenceCommand(int bufferSize, byte buffer[]);
		static void toneCommand1(int bufferSize, byte buffer[]);
		static void toneCommand2(int bufferSize, byte buffer[]);
		static void noToneCommand(int bufferSize, byte buffer[]);
		static void shiftOutCommand(int bufferSize, byte buffer[]);
		static void interruptsCommand(int bufferSize, byte buffer[]);
		static void noInterruptsCommand(int bufferSize, byte buffer[]);		
};

extern BasicCommandsInternal BasicCommands;

#endif