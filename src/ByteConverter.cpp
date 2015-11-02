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

#include "ByteConverter.h"

void ByteConverter::GetBytes(float value, byte data[])
{
	ufloat uvalue = ufloat();  
	uvalue.value = value;

	data[0] = uvalue.bytes[0];
	data[1] = uvalue.bytes[1];
	data[2] = uvalue.bytes[2];
	data[3] = uvalue.bytes[3];
}

unsigned int ByteConverter::bytesToUint(byte lower, byte upper)
{
	unsigned int returnValue = 0;

	// ***
	// *** The first 2 bytes are the reigisterId. This
	// *** is mapped to the index in the mapping array.
	// ***
	returnValue = upper;
	returnValue <<= 8;
	returnValue += lower;
	
	return returnValue;
}

unsigned long ByteConverter::bytesToUlong(byte lower1, byte lower2, byte upper1, byte upper2)
{
	unsigned long returnValue = 0;

	returnValue = upper2;
	returnValue <<= 8;
	returnValue += upper1;
	returnValue <<= 8;
	returnValue += lower2;
	returnValue <<= 8;
	returnValue += lower1;
	
	return returnValue;
}