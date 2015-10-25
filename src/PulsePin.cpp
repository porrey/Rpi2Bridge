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

#include "PulsePin.h"

void PulsePinInternal::begin()
{
	this->enabled = false;

	#ifdef DEBUG_MODE
	Serial.println("Registering Pulse Pin Commands...");
	#endif
	
	#ifdef DEBUG_MODE 
	Serial.print("\tPulsePin(): "); 
	#endif
	Rpi2Bridge.registerCommand(PULSE_STARTING_REGISTER_ID + 0, 12, PulsePinInternal::enable, PulsePinInternal::loop);
	
	#ifdef DEBUG_MODE
	Serial.print("\tnoPulsePin(): "); 
	#endif
	Rpi2Bridge.registerCommand(PULSE_STARTING_REGISTER_ID + 1, 3, PulsePinInternal::disable, NULL);	
}

bool PulsePinInternal::loop()
{
	if (PulsePin.enabled)
	{
		if (PulsePin.currentValue == PulsePin.offValue)
		{
			if (millis() - PulsePin.lastTransition >= PulsePin.offDuration)
			{
				digitalWrite(PulsePin.pin, !PulsePin.offValue);
				PulsePin.currentValue = !PulsePin.offValue;
				PulsePin.lastTransition = millis();
			}
		}
		else
		{
			if (millis() - PulsePin.lastTransition >= PulsePin.onDuration)
			{
				digitalWrite(PulsePin.pin, PulsePin.offValue);
				PulsePin.currentValue = PulsePin.offValue;
				PulsePin.lastTransition = millis();
			}
		}
	}
	
	return PulsePin.enabled;
}

void PulsePinInternal::enable(int bufferSize, byte buffer[])
{
	// ***
	// *** Get the on duration
	// ***
	PulsePin.onDuration = Converter.bytesToUlong(buffer[4], buffer[5], buffer[6], buffer[7]);
	
	// ***
	// *** Get the off duration
	// ***
	PulsePin.offDuration = Converter.bytesToUlong(buffer[8], buffer[9], buffer[10], buffer[11]);
		
	#ifdef DEBUG_MODE
	// ***
	// *** Digital write
	// ***
	Serial.print("\tPulsePin.enable(pin = ");
	Serial.print(buffer[2]);
	Serial.print(", offValue = ");
	if (buffer[3] == HIGH) Serial.print("HIGH"); else Serial.print("LOW");
	Serial.print(", onDuration = ");
	Serial.print(PulsePin.onDuration);	
	Serial.print(", offDuration = ");
	Serial.print(PulsePin.offDuration);
	Serial.println(")");
	#endif

	PulsePin.pin = buffer[2];
	PulsePin.offValue = buffer[3];
	PulsePin.currentValue = PulsePin.offValue;
	pinMode(PulsePin.pin, OUTPUT);
	digitalWrite(PulsePin.pin, PulsePin.offValue);
	PulsePin.lastTransition = millis();

	PulsePin.enabled = true;
	
	// ***
	// *** Set the result
	// ***
	Rpi2Bridge.setResult(RESULT_SUCCESS);
}

void PulsePinInternal::disable(int bufferSize, byte buffer[])
{
	#ifdef DEBUG_MODE
	// ***
	// *** Digital write
	// ***
	Serial.print("\tnoPulsePin.disable(pin = ");
	Serial.print(buffer[2]);
	Serial.println(")");
	#endif

	PulsePin.enabled = false;
	PulsePin.currentValue = PulsePin.offValue;
	digitalWrite(PulsePin.pin, PulsePin.offValue);
	
	// ***
	// *** Set the result
	// ***
	Rpi2Bridge.setResult(RESULT_SUCCESS);
}

// ***
// *** Global Object
// ***
PulsePinInternal PulsePin = PulsePinInternal();