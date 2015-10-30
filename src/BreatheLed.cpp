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

#include "BreatheLed.h"

void BreatheLedInternal::begin()
{
	this->enabled = false;

	#ifdef DEBUG_MODE
	Serial.println("Registering Breathe LED Commands...");
	#endif
	
	#ifdef DEBUG_MODE 
	Serial.print("\tbreatheLed(): "); 
	#endif
	Rpi2Bridge.registerCommand(BREATH_STARTING_REGISTER_ID + 0, 6, BreatheLedInternal::enable, BreatheLedInternal::loop);
	
	#ifdef DEBUG_MODE
	Serial.print("\tnoBreatheLed(): "); 
	#endif
	Rpi2Bridge.registerCommand(BREATH_STARTING_REGISTER_ID + 1, 3, BreatheLedInternal::disable, NULL);	
}

bool BreatheLedInternal::loop()
{
	if (BreatheLed.enabled)
	{
		// ***
		// *** Set the pin output
		// ***
		analogWrite(BreatheLed.pin, BreatheLed.currentValue);

		// ***
		// *** Change the brightness for next time through the loop
		// ***		
		BreatheLed.currentValue += (BreatheLed.step * BreatheLed.stepDirection);
		
		// ***
		// *** Reverse the direction of the fading at the ends of the fade
		// ***
		if (BreatheLed.currentValue == 0)
		{
			BreatheLed.stepDirection = 1;
			
		}	
		else if(BreatheLed.currentValue == 255)
		{
			BreatheLed.stepDirection = -1;
		}
		
		// ***
		// *** Delay is 255 - rate
		// ***
		delay(255 - BreatheLed.rate);
	}
	
	return BreatheLed.enabled;
}

void BreatheLedInternal::enable(int bufferSize, byte buffer[])
{
	#ifdef DEBUG_MODE
	// ***
	// *** Digital write
	// ***
	Serial.print("\tbreatheLed.enable(pin = ");
	Serial.print(buffer[2]);
	Serial.print(", rate = ");
	Serial.print(buffer[3]);
	Serial.print(", step = ");
	Serial.print(buffer[4]);
	Serial.print(", offValue = ");
	Serial.print(buffer[5]);
	Serial.println(")");
	#endif

	BreatheLed.pin = buffer[2];
	BreatheLed.rate = buffer[3];
	BreatheLed.step = buffer[4];
	BreatheLed.offValue = buffer[5];
	
	if (BreatheLed.offValue == 0)
	{
		// ***
		// *** Set direction to increase
		// ***
		BreatheLed.stepDirection = 1;
	}
	else
	{
		// ***
		// *** Off value is either 0 or 255. Always
		// *** ensure if it is not 0 then the value
		// *** is set to 255.
		// ***
		BreatheLed.offValue = 255;
		
		// ***
		// *** Set direction to decrease
		// ***
		BreatheLed.stepDirection = -1;
	}

	BreatheLed.currentValue = BreatheLed.offValue;		
	BreatheLed.enabled = true;

	// ***
	// *** Set the result
	// ***
	Rpi2Bridge.setResult(RESULT_SUCCESS);
}

void BreatheLedInternal::disable(int bufferSize, byte buffer[])
{
	#ifdef DEBUG_MODE
	// ***
	// *** Digital write
	// ***
	Serial.print("\tnoBreatheLed.disable(pin = ");
	Serial.print(buffer[2]);
	Serial.println(")");
	#endif


	BreatheLed.enabled = false;
	analogWrite(BreatheLed.pin, BreatheLed.offValue);

	// ***
	// *** Set the result
	// ***
	Rpi2Bridge.setResult(RESULT_SUCCESS);
}

// ***
// *** Global Object
// ***
BreatheLedInternal BreatheLed = BreatheLedInternal();