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

#ifndef PULSE_PIN_COMMANDS_H
#define PULSE_PIN_COMMANDS_H

#define PULSE_STARTING_REGISTER_ID 14

class PulsePinInternal
{	
	public:
		// ***
		// *** Sets whether the effect is enabled or not.
		// ***
		bool enabled;
		
		// ***
		// *** The pin on which the LED is connected.
		// ***
		byte pin;
		
		// ***
		// *** Current pin output value
		// ***
		byte currentValue;

		// ***
		// *** The "off" value for the pin
		// ***
		byte offValue;
		
		// ***
		// *** How long the pin is on
		// ***
		unsigned long onDuration;
				
		// ***
		// *** How long the pin is off
		// ***
		unsigned long offDuration;
		
		// ***
		// *** The "time" of the last transition
		// ***
		unsigned long lastTransition;
		
		// ***
		// *** Initializes the instance
		// ***
		void begin();
		
		// ***
		// *** Callbacks
		// ***
		static bool loop();		
		static void enable(int bufferSize, byte buffer[]);
		static void disable(int bufferSize, byte buffer[]);
};

extern PulsePinInternal PulsePin;

#endif