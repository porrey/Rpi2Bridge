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

#ifndef BYTE_CONVERTER_H
#define BYTE_CONVERTER_H

union ufloat
{
    byte bytes[4];
    float value;
};

class ByteConverter
{
	public:
		static unsigned int bytesToUint(byte lower, byte upper);	
		static unsigned long bytesToUlong(byte lower1, byte lower2, byte upper1, byte upper2);
		static void GetBytes(float value, byte data[]);
};
#endif
