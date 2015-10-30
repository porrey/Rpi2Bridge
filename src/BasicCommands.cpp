// Raspberry Pi 2 to Arduino Bridge
// written by Daniel Porrey
<<<<<<< HEAD
// Version 1.0.2
=======
// Version 1.0.1
>>>>>>> origin/master
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

#include "BasicCommands.h"

void BasicCommandsInternal::begin()
{
	#ifdef DEBUG_MODE
	Serial.println("Registering Basic Commands...");
	#endif
	
	#ifdef DEBUG_MODE 
	Serial.print("\tpinMode(): "); 
	#endif
	Rpi2Bridge.registerCommand(STARTING_REGISTER_ID + 0, 4, BasicCommandsInternal::pinModeCommand, NULL);
	
	#ifdef DEBUG_MODE 
	Serial.print("\tdigitalRead(): "); 
	#endif
	Rpi2Bridge.registerCommand(STARTING_REGISTER_ID + 1, 3, BasicCommandsInternal::digitalReadCommand, NULL);
	
	#ifdef DEBUG_MODE 
	Serial.print("\tdigitalWrite(): "); 
	#endif
	Rpi2Bridge.registerCommand(STARTING_REGISTER_ID + 2, 4, BasicCommandsInternal::digitalWriteCommand, NULL);
	
	#ifdef DEBUG_MODE 
	Serial.print("\tanalogRead(): "); 
	#endif
	Rpi2Bridge.registerCommand(STARTING_REGISTER_ID + 3, 3, BasicCommandsInternal::analogReadCommand, NULL);
	
	#ifdef DEBUG_MODE 
	Serial.print("\tanalogWrite(): "); 
	#endif
	Rpi2Bridge.registerCommand(STARTING_REGISTER_ID + 4, 4, BasicCommandsInternal::analogWriteCommand, NULL);
	
	#ifdef DEBUG_MODE 
	Serial.print("\tanalogReference(): "); 
	#endif
	Rpi2Bridge.registerCommand(STARTING_REGISTER_ID + 5, 3, BasicCommandsInternal::analogReferenceCommand, NULL);
	
	#ifdef DEBUG_MODE 
	Serial.print("\ttone(): "); 
	#endif
	Rpi2Bridge.registerCommand(STARTING_REGISTER_ID + 6, 9, BasicCommandsInternal::toneCommand1, NULL);
	
	#ifdef DEBUG_MODE 
	Serial.print("\ttone(): "); 
	#endif
	Rpi2Bridge.registerCommand(STARTING_REGISTER_ID + 7, 5, BasicCommandsInternal::toneCommand2, NULL);
	
	#ifdef DEBUG_MODE 
	Serial.print("\tnoTone(): "); 
	#endif
	Rpi2Bridge.registerCommand(STARTING_REGISTER_ID + 8, 3, BasicCommandsInternal::noToneCommand, NULL);
	
	#ifdef DEBUG_MODE 
	Serial.print("\tshiftOut(): "); 
	#endif
	Rpi2Bridge.registerCommand(STARTING_REGISTER_ID + 9, 6, BasicCommandsInternal::shiftOutCommand, NULL);
	
	#ifdef DEBUG_MODE 
	Serial.print("\tinterrupts(): "); 
	#endif
	Rpi2Bridge.registerCommand(STARTING_REGISTER_ID + 10, 2, BasicCommandsInternal::interruptsCommand, NULL);
	
	#ifdef DEBUG_MODE 
	Serial.print("\tnoInterrupts(): "); 
	#endif
	Rpi2Bridge.registerCommand(STARTING_REGISTER_ID + 11, 2, BasicCommandsInternal::noInterruptsCommand, NULL);
	
	#ifdef DEBUG_MODE 
	Serial.println("\tRegistration completed."); 
	#endif
}

void BasicCommandsInternal::pinModeCommand(int bufferSize, byte buffer[])
{
	#ifdef DEBUG_MODE
	// ***
	// *** Set pin Mode
	// ***  
	Serial.print("\tpinMode(pin = ");
	Serial.print(buffer[2]);
	Serial.print(", mode = ");
	if (buffer[3] == 0) 
	{
		Serial.println("INPUT)");
	}
	else if (buffer[3] == 1)
	{
		Serial.println("OUTPUT)");
	}
	else if (buffer[3] == 2)
	{
		Serial.println("INPUT_PULLUP)");
	}
	#endif

	pinMode(buffer[2], buffer[3]);

	// ***
	// *** Set the result
	// ***
	Rpi2Bridge.setResult(RESULT_SUCCESS);
}

void BasicCommandsInternal::digitalReadCommand(int bufferSize, byte buffer[])
{
	#ifdef DEBUG_MODE
	// ***
	// *** Read from a digital port
	// ***
	Serial.print("\tdigitalRead(pin = D");
	Serial.print(buffer[2]);
	Serial.print(") = ");
	#endif

	byte value = digitalRead(buffer[2]);

	#ifdef DEBUG_MODE
	if (value == HIGH)
	{
		Serial.println("HIGH");
	}
	else
	{
		Serial.println("LOW");
	}
	#endif

	// ***
	// *** Set the result
	// ***
	Rpi2Bridge.setResult(RESULT_SUCCESS, value);
}

void BasicCommandsInternal::digitalWriteCommand(int bufferSize, byte buffer[])
{
	#ifdef DEBUG_MODE
	// ***
	// *** Digital write
	// ***
	Serial.print("\tdigitalWrite(pin = D");
	Serial.print(buffer[2]);
	Serial.print(", value = ");

	if (buffer[3] == 1)
	{
		Serial.println("HIGH)");
	}
	else
	{
		Serial.println("LOW)");
	}
	#endif

	digitalWrite(buffer[2], buffer[3]);

	// ***
	// *** Set the result
	// ***
	Rpi2Bridge.setResult(RESULT_SUCCESS);
}

void BasicCommandsInternal::analogReadCommand(int bufferSize, byte buffer[])
{	
	#ifdef DEBUG_MODE
	// ***
	// ***
	// ***
	Serial.print("\tanalogRead(pin = A");
	Serial.print(buffer[2]);
	Serial.print(") = ");
	#endif

	unsigned int value = analogRead(buffer[2]);

	#ifdef DEBUG_MODE
	Serial.println(value);
	#endif

	// ***
	// *** Set the result
	// ***
	Rpi2Bridge.setResult(RESULT_SUCCESS, value);
}

<<<<<<< HEAD
void BasicCommandsInternal::analogReferenceCommand(int bufferSize, byte buffer[])
=======
void BasicCommandsInternal::analogReferenceCommand(int bufferSize, byte buffer[]);
>>>>>>> origin/master
{
	#ifdef DEBUG_MODE
	// ***
	// *** Digital write
	// ***
	Serial.print("\tanalogReference(type = ");

	if (buffer[2] == DEFAULT)
	{
		Serial.println("DEFAULT)");
	}
	else if (buffer[2] == INTERNAL)
	{
		Serial.println("INTERNAL)");
	}
	else if (buffer[2] == INTERNAL1V1)
	{
		Serial.println("INTERNAL1V1)");
	}
	else if (buffer[2] == INTERNAL2V56)
	{
		Serial.println("INTERNAL2V56)");
	}
	else if (buffer[2] == EXTERNAL)
	{
		Serial.println("EXTERNAL)");
	}
	#endif

	analogReference(buffer[2]);

	// ***
	// *** Set the result
	// ***
	Rpi2Bridge.setResult(RESULT_SUCCESS);
	
	
}

void BasicCommandsInternal::analogWriteCommand(int bufferSize, byte buffer[])
{
	#ifdef DEBUG_MODE
	// ***
	// *** Analog write
	// ***
	Serial.print("\tanalogWrite(pin = A");
	Serial.print(buffer[2]);
	Serial.print(", value = ");
	Serial.println(buffer[3]);
	#endif
	
	analogWrite(buffer[2], buffer[3]);

	// ***
	// *** Set the result
	// ***
	Rpi2Bridge.setResult(RESULT_SUCCESS);
}

void BasicCommandsInternal::toneCommand1(int bufferSize, byte buffer[])
{
	// ***
	// *** Get the frequency
	// ***
	unsigned int frequency = Converter.bytesToUint(buffer[3], buffer[4]);

	// ***
	// *** Get the duration
	// ***
	unsigned long duration = Converter.bytesToUlong(buffer[5], buffer[6], buffer[7], buffer[8]);

	#ifdef DEBUG_MODE
	// ***
	// *** tone(pin, frequency, duration)
	// ***
	Serial.print("\ttone(Pin = ");
	Serial.print(buffer[2]);
	Serial.print(", Frequency = ");
	Serial.print(frequency);
	Serial.print(", Duration = ");
	Serial.print(duration);
	Serial.println(")");
	#endif
	
	tone(buffer[2], frequency, duration);

	// ***
	// *** Set the result
	// ***
	Rpi2Bridge.setResult(RESULT_SUCCESS);
}

void BasicCommandsInternal::toneCommand2(int bufferSize, byte buffer[])
{
	// ***
	// *** Get the frequency
	// ***
	unsigned int frequency = Converter.bytesToUint(buffer[3], buffer[4]);

	#ifdef DEBUG_MODE
	// ***
	// *** tone(pin, frequency)
	// ***
	Serial.print("\ttone(Pin = ");
	Serial.print(buffer[2]);
	Serial.print(", Frequency = ");
	Serial.print(frequency);
	Serial.println(")");
	#endif
	
	tone(buffer[2], frequency);

	// ***
	// *** Set the result
	// ***
	Rpi2Bridge.setResult(RESULT_SUCCESS);
}

void BasicCommandsInternal::noToneCommand(int bufferSize, byte buffer[])
{
	#ifdef DEBUG_MODE
	// ***
	// *** noTone(pin)
	// ***
	Serial.print("\tnoTone(Pin = ");
	Serial.print(buffer[2]);
	Serial.println(")");
	#endif

	noTone(buffer[2]);

	// ***
	// *** Set the result
	// ***
	Rpi2Bridge.setResult(RESULT_SUCCESS);
}

void BasicCommandsInternal::shiftOutCommand(int bufferSize, byte buffer[])
{
	#ifdef DEBUG_MODE
	// ***
	// *** shiftOut(dataPin, clockPin, bitOrder, value)
	// ***
	Serial.print("\tshiftOut(dataPin = ");
	Serial.print(buffer[2]);
	Serial.print(", clockPin = ");
	Serial.print(buffer[3]);
	Serial.print(", bitOrder = ");
	if (buffer[4] == MSBFIRST) Serial.print("MSBFIRST"); else Serial.print("LSBFIRST");
	Serial.print(", value = ");
	Serial.print(buffer[5]);	
	Serial.println(")");
	#endif
	
	shiftOut(buffer[2], buffer[3], buffer[4], buffer[5]);

	// ***
	// *** Set the result
	// ***
	Rpi2Bridge.setResult(RESULT_SUCCESS);
}

void BasicCommandsInternal::interruptsCommand(int bufferSize, byte buffer[])
{
	#ifdef DEBUG_MODE
	Serial.println("\tinterrupts()");
	#endif

	interrupts();
	
	// ***
	// *** Set the result
	// ***
	Rpi2Bridge.setResult(RESULT_SUCCESS);
}

void BasicCommandsInternal::noInterruptsCommand(int bufferSize, byte buffer[])
{
	#ifdef DEBUG_MODE
	Serial.println("\tnoInterrupts()");
	#endif

	noInterrupts();
	
	// ***
	// *** Set the result
	// ***
	Rpi2Bridge.setResult(RESULT_SUCCESS);
}

// ***
// *** Global Object
// ***
BasicCommandsInternal BasicCommands = BasicCommandsInternal();