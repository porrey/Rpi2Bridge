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
// *** Download and install the DHT library from
// *** http://playground.arduino.cc//Main/DHTLib or
// *** https://github.com/RobTillaart/Arduino/tree/master/libraries/DHTlib
// ***
#include <dht.h>

// ***
// *** The pin on which the DHT is connected
// ***
#define DHT11_PIN 2

// ***
// *** Create the DHT11 instance
// ***
dht _dht;

// ***
// *** Result of last call
// ***
int _result = 0;

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
      switch (result[0])
      {
        case 1:
          throw new ArduinoException("DHT11 checksum error.");
        case 2:
          throw new ArduinoException("DHT11 timeout.");
        case 3:
          throw new ArduinoException("DHT11 failed to connect.");
        case 4:
          throw new ArduinoException("DHT11 failed to acknowledge request.");
        case 5:
          throw new ArduinoException("DHT11 failed to acknowledge request.");
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
  Rpi2Bridge.registerCommand(FIRST_CUSTOM_REGISTER_ID + 0, 2, respondToRequest, getDhtReading);

#ifdef DEBUG_MODE
  // ***
  // *** Display the sensor readign so we know
  // *** the sensor is working.
  // ***
  Serial.println("\nChecking DHT11...");
  int result = _dht.read11(DHT11_PIN);

  if (result == DHTLIB_OK)
  {
    Serial.print("\tCurrent humidity is ");
    Serial.println(_dht.humidity, 1);
    Serial.print("\tCurrent temperature is ");
    Serial.println(_dht.temperature, 1);
  }
  else
  {
    Serial.println("Failed to read from DHT11!");
  }

  // ***
  // *** Show ready
  // ***
  Serial.println("\nReady. Waiting for commands...");
#endif
}

void loop()
{
  Rpi2Bridge.loop();
}

// ***
// *** This function is registered as the
// *** loop callback for this command.
// ***
bool getDhtReading()
{
  // ***
  // *** Read the sensor every second
  // ***
  _dht.humidity = 0;
  _dht.temperature = 0;
  _result = _dht.read11(DHT11_PIN);

  // ***
  // *** Wait...
  // ***
  delay(1000);

  // ***
  // *** Return true to let the library know
  // *** we are processing in the loop.
  // ***
  return true;
}

// ***
// *** Called when the client (from C#) requests
// *** a reading from the DHT sensor.
// ***
void respondToRequest(int bufferSize, byte buffer[])
{
#ifdef DEBUG_MODE
  Serial.println("\tgetDhtReading()");
#endif

  // ***
  // *** The is the data writtern back to the client using the
  // *** writeBuffer. The client code knows to expect 9 bytes.
  // ***
  byte writeBuffer[9];

  // ***
  // *** Show the result of the last
  // *** reading attempt
  // ***
#ifdef DEBUG_MODE
  Serial.print("\tLast Sensor Result = ");
  Serial.println(_result);
#endif

  // ***
  // *** Get the humidity and convert it to a
  // *** byte array.
  // ***
  byte humidityBytes[4];
  ByteConverter::GetBytes(_dht.humidity, humidityBytes);
#ifdef DEBUG_MODE
  Serial.print("\tCurrent humidity is ");
  Serial.println(_dht.humidity);
#endif

  // ***
  // *** Get the temperature and convert it to a
  // *** byte array.
  // ***
  byte temperatureBytes[4];
  ByteConverter::GetBytes(_dht.temperature, temperatureBytes);
#ifdef DEBUG_MODE
  Serial.print("\tCurrent temperature is ");
  Serial.println(_dht.temperature);
#endif

  // ***
  // *** The first byte is the result code (0 always indicates success).
  // *** This is an example of how to send multiple result codes.
  // ***
  // *** The resulting values of the Dht library call are:
  // ***
  // *** DHTLIB_OK                    0
  // *** DHTLIB_ERROR_CHECKSUM       -1
  // *** DHTLIB_ERROR_TIMEOUT        -2
  // *** DHTLIB_ERROR_CONNECT        -3
  // *** DHTLIB_ERROR_ACK_L          -4
  // *** DHTLIB_ERROR_ACK_H          -5
  // ***
  writeBuffer[0] = abs(_result);

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
