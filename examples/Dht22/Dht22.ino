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
// *** 1) Install NuGet package: IoT.Arduino
// ***
// *** 2) Add using statement:
// ***
/*
    using Porrey.Uwp.IoT.Devices.Arduino;
*/
// ***
// *** 3) Add this C# code to your Visual Studio project
// ***
/*
    // ***
    // *** Initialize the Arduino library
    // ***
    Arduino arduino = new Arduino(0x04);
    await arduino.InitializeAsync();

		// ***
		// *** Note the Arduino.FirstCustomRegisterId + 0 should match the
		// *** Rpi2Bridge.registerCommand(FIRST_CUSTOM_REGISTER_ID + 0, 2, getDhtReading, NULL);
		// *** command in setup().Change the value of 0 to any value depending on the number of
    // *** custom commands (ensure each register ID is unique).
		// ***
		byte[] result = await arduino.CustomCommandAsync(Arduino.FirstCustomRegisterId + 0, new byte[0], 9);

    if (result[0] == 0)
    {
		  float humidity = BitConverter.ToSingle(result, 1);
		  float temperature = BitConverter.ToSingle(result, 5);
    }
    else
    {
      // ***
      // *** Map your custom errors
      // ***
      switch(result[0])
      {
        case 1:
          // Add code...
          break;
        case 2:
          // Add code...
          break;
        default:
          // Add code...
          break;
      }
    }
*/
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
  // *** and Number them as follows:
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
#ifdef DEBUG_MODE
  Serial.println("Initializing DHT...");
#endif
  _dht.begin();

#ifdef DEBUG_MODE
  // ***
  // *** Display the sensor readign so we know
  // *** the sensor is working.
  // ***
  Serial.print("\tCurrent humidity is ");
  Serial.println(_dht.readHumidity(true));
  Serial.print("\tCurrent temperature is ");
  Serial.println(_dht.readTemperature(false, true));

  // ***
  // *** Show ready
  // ***
  Serial.println("Ready. Waiting for commands...");
#endif
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
#ifdef DEBUG_MODE
  Serial.println("\tgetDhtReading()");
#endif

  // ***
  // *** The is the data writtern back to the client using the
  // *** writeBuffer. The client code knows to expect 9 bytes.
  // ***
  byte writeBuffer[9];

  _dht.read();

  // ***
  // *** Read the humidity and convert it to a
  // *** byte array
  // ***
  byte humidityBytes[4];
  float humidityValue = _dht.readHumidity();
  ByteConverter::GetBytes(humidityValue, humidityBytes);
#ifdef DEBUG_MODE
  Serial.print("\tCurrent humidity is ");
  Serial.println(humidityValue);
#endif

  // ***
  // *** Read temperature as Celsius (the default) and
  // *** convert it to a byte array
  // ***
  byte temperatureBytes[4];
  float temperatureValue = _dht.readTemperature();
  ByteConverter::GetBytes(temperatureValue, temperatureBytes);
#ifdef DEBUG_MODE
  Serial.print("\tCurrent temperature is ");
  Serial.println(temperatureValue);
#endif

  // ***
  // *** The first byte is the result code (0 always indicates success).
  // *** This is an example of how to send multiple result codes.
  // ***
  if (!isnan(humidityValue) && !isnan(temperatureValue))
  {
    // ***
    // *** Success
    // ***
    writeBuffer[0] = 0;
  }
  else if (isnan(humidityValue) || isnan(temperatureValue))
  {
    // ***
    // *** Failed to read from DHT sensor!
    // ***
    writeBuffer[0] = 1;
  }

  // ***
  // *** Write the humidity in the
  // *** first 4 bytes
  // ***
  writeBuffer[1] = humidityBytes[0];
  writeBuffer[2] = humidityBytes[1];
  writeBuffer[3] = humidityBytes[2];
  writeBuffer[4] = humidityBytes[3];

  // ***
  // *** Write the temperature in the
  // *** second 4 bytes
  // ***
  writeBuffer[5] = temperatureBytes[0];
  writeBuffer[6] = temperatureBytes[1];
  writeBuffer[7] = temperatureBytes[2];
  writeBuffer[8] = temperatureBytes[3];

  // ***
  // *** Send the response back to the client
  // *** using the bridge.
  // ***
  Rpi2Bridge.setResultDirect(writeBuffer, 9);
}
