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

#ifndef RPI2_BRIDGE_H
#define RPI2_BRIDGE_H

#include <Arduino.h>
#include <Wire.h>
#include "Definitions.h"
#include "ByteConverter.h"

// ***
// *** Result/error codes
// ***
#define RESULT_SUCCESS                0
#define RESULT_UNEXPECTED_BUFFER      1
#define RESULT_BUFFER_ERROR			  2
#define RESULT_COMMAND_NOT_SUPPORTED  3
#define RESULT_MAPPING_MISMATCH		  4
#define RESULT_MISSING_CALLBACK		  5
#define RESULT_ID_TOO_LARGE			  6

// ***
// ***
// ***
typedef void (* CommandCallback)(int, byte*);
typedef bool (* LoopCallback)(void);

struct RegisterMap
{
	unsigned int id;
	unsigned int expectedByteCount;
	CommandCallback commandCallback;
	LoopCallback loopCallback;
};

class Rpi2BridgeInternal
{
	private:
		byte slaveAddress;
		RegisterMap registerMap[MAXIMUM_COMMANDS];
		int bufferLength = 0;
		byte outBuffer[MAX_OUTPUT_BUFFER];

		void initializeMap();
		int getDataFromWire(byte buffer[]);
		void resetOutputBuffer();

		static void onReceive(int byteCount);
		static void onRequest();
		
	public:
		Rpi2BridgeInternal(byte slaveAddress);
		void begin();
		bool loop();

		void setResult(byte result);
		void setResult(byte result, byte value);
		void setResult(byte result, unsigned int value);
		void setResultDirect(byte result[], int byteCount);

		bool registerCommand(unsigned int id, unsigned int expectedByteCount, CommandCallback commandCallback, LoopCallback loopCallback);
		bool unregisterCommand(unsigned int id);
};

extern Rpi2BridgeInternal Rpi2Bridge;

#endif