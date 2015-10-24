// Raspberry Pi 2 to Arduino Bridge
// written by Daniel Porrey
// Version 1.0.0
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
// This is a partner sketch for the C# library for communicating
// to the Arduino via the i2c bus from Windows 10 IoT Core on the
// Raspberry Pi 2.
//
// https://www.hackster.io/porrey/rpi2bridge
//
// The C# library is available in NuGet; ID = IoT.Arduino
//

#include <Rpi2Bridge.h>
#include <Wire.h>

// ***
// *** Download and install the Adafruit library for the Dht
// *** https://github.com/adafruit/DHT-sensor-library/blob/master/DHT.cpp
// ***
#include <DHT.h>

// ***
// *** The pin on which the DHT is connected
// ***
#define DHTPIN 2

// ***
// *** Initialize DHT sensor. Specify the type as one
// *** of the following:
// *** DHT11
// *** DHT22 (DHT22, AM2302, AM2321)
// *** DHT21 (DHT21, AM2301)
// ***
DHT _dht(DHTPIN, DHT22);

// ***
// *** C# Code to call this command:
// ***
// ***
// *** Install NuGet package: IoT.Arduino
// ***
// *** Add using statement:
// ***
// ***     using Porrey.Uwp.IoT.Devices.Arduino;
// ***
// *** Add code:
// ***
// ***    Arduino arduino = new Arduino(0x04);
// *** 	  await arduino.InitializeAsync();
// ***
// ***    byte[] result = await arduino.CustomCommandAsync(15, new byte[0], 9);
// ***    float humidity = BitConverter.ToSingle(result, 1);
// ***    float temperature = BitConverter.ToSingle(result, 5);
// ***

void setup() 
{
	// ***
	// *** Start serial for output
	// ***
	Serial.begin(115200);

	// ***
	// *** Start the i2c bridge to receive commands
	// *** from the Raspberry Pi 2 C# library.
	// ***
	Serial.println("Initializing bridge...");
	Rpi2Bridge.begin();

	// ***
	// *** Register the command. Always use the value
	// *** FIRST_CUSTOM_REGISTER_ID to define the Register ID
	// *** Number them as follows:
	// ***
	// *** FIRST_CUSTOM_REGISTER_ID + 0
	// *** FIRST_CUSTOM_REGISTER_ID + 1
	// *** FIRST_CUSTOM_REGISTER_ID + 2
	// *** FIRST_CUSTOM_REGISTER_ID + n
	// ***
	Rpi2Bridge.registerCommand(FIRST_CUSTOM_REGISTER_ID + 0, 2, getDhtReading, NULL);

	// ***
	// *** Initialize the DHT
	// ***
	Serial.println("Initializing DHT...");
	_dht.begin();

	Serial.print("Current temperature is ");
	Serial.println(_dht.readTemperature());
	Serial.print("Current humidity is ");
	Serial.println(_dht.readHumidity());
  
	// ***
	// *** Show ready
	// ***
	Serial.println("Ready.");
}

void loop()
{
	Rpi2Bridge.loop();
}

// ***
// *** Called when the client (from C#) requests
// *** a reading from the DHT sensor.
// ***
void getDhtReading(int bufferSize, byte buffer[])
{
	Serial.println("\tgetDhtReading()");

	// ***
	// ***
	// ***
	byte writeBuffer[9];

	// ***
	// *** Read the humidity and convert it to a 
	// *** byte array
	// ***
	byte humidity[4];
	ByteConverterInternal::GetBytes(_dht.readHumidity(), humidity);

	// ***
	// *** Read temperature as Celsius (the default) and
	// *** convert it to a byte array
	// ***
	byte temperature[4];
	ByteConverterInternal::GetBytes(_dht.readTemperature(), temperature);

	// ***
	// *** This is the data writtern back to the client. The
	// *** client code knows to expect 9 bytes.
	// ***
	writeBuffer[0] = 0;
	writeBuffer[1] = humidity[0];
	writeBuffer[2] = humidity[1];
	writeBuffer[3] = humidity[2];
	writeBuffer[4] = humidity[3];
	writeBuffer[5] = temperature[0];
	writeBuffer[6] = temperature[1];
	writeBuffer[7] = temperature[2];
	writeBuffer[8] = temperature[3];

	Rpi2Bridge.setResultDirect(writeBuffer, 9);
}
