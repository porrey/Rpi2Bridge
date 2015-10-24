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
// This is a partner library for the C# library for communicating
// to the Arduino via the i2c bus from Windows 10 IoT Core on the
// Raspberry Pi 2.
//
// https://www.hackster.io/porrey/rpi2bridge
//
// The C# library is available in NuGet; ID = IoT.Arduino
//

#include "Rpi2Bridge.h"

Rpi2BridgeInternal::Rpi2BridgeInternal(byte slaveAddress)
{
	this->slaveAddress  = slaveAddress;
}

void Rpi2BridgeInternal::begin()
{
	// ***
	// *** Make sure all the maps are marked as unused
	// ***
	this->initializeMap();

	// ***
	// *** Reset the buffer
	// ***
	this->resetOutputBuffer();
	
	// ***
	// *** Initialize i2c as slave
	// ***
	Wire.begin(this->slaveAddress);

	// ***
	// *** Define callbacks for i2c communication
	// ***
	Wire.onReceive(Rpi2BridgeInternal::onReceive);
	Wire.onRequest(Rpi2BridgeInternal::onRequest);
	
	// ***
	// *** Put a RESULT_SUCCESS on the output buffer
	// ***
	this->setResult(RESULT_SUCCESS);
}

bool Rpi2BridgeInternal::loop()
{
	int enabledCount = 0;

	for (int i =0; i < MAXIMUM_COMMANDS; ++i)
	{
		if (this->registerMap[i].id != -1)
		{
			if (this->registerMap[i].loopCallback != NULL)
			{
				// ***
				// *** The loop method will return true if it
				// *** is performing a function
				// ***
				bool enabled = this->registerMap[i].loopCallback();
				if (enabled) enabledCount++;
			}
		}
	}

	// ***
	// *** Check that at least 1 loop is enabled
	// ***
	if (enabledCount == 0)
	{
				
		// ***
		// *** No loop was enabled so introduce a delay
		// ***
		delay(LOOP_DELAY);
	}
}

void Rpi2BridgeInternal::initializeMap()
{
	// ***
	// *** Mark each map unused
	// ***
	for(int i = 0; i < MAXIMUM_COMMANDS; i++)
	{
		this->registerMap[i].id = -1;
	}
}

bool Rpi2BridgeInternal::registerCommand(unsigned int id, unsigned int expectedByteCount, CommandCallback commandCallback, LoopCallback loopCallback)
{
	bool returnValue = false;

	if (id < MAXIMUM_COMMANDS)
	{
		if (this->registerMap[id].id == -1)
		{
			this->registerMap[id].id = id;
			this->registerMap[id].expectedByteCount = expectedByteCount;
			this->registerMap[id].commandCallback = commandCallback;
			this->registerMap[id].loopCallback = loopCallback;

			returnValue = true;
			
			#ifdef DEBUG_MODE
			Serial.print("Command with Register ID = ");
			Serial.print(id);
			Serial.println(" was successfully mapped.");
			#endif
		}
		else
		{
			#ifdef DEBUG_MODE
			Serial.print("Command with Register ID = ");
			Serial.print(id);
			Serial.println(" failed to be mapped. A command with this ID is already mapped.");
			#endif

			returnValue = false;
		}
	}
	else
	{
		#ifdef DEBUG_MODE
		Serial.print("Command with Register ID = ");
		Serial.print(id);
		Serial.println(" failed to be mapped. Maximum number of commands have already been mapped.");
		#endif

		returnValue = false;
	}
	
	return returnValue;
}

bool Rpi2BridgeInternal::unregisterCommand(unsigned int id)
{
	bool returnValue = false;
	
	if (id < MAXIMUM_COMMANDS)
	{
		if (this->registerMap[id].id != -1)
		{
			this->registerMap[id].id = -1;
			returnValue = true;
		}
		else
		{
			returnValue = false;
		}
	}
	else
	{
		returnValue = false;
	}
	
	return returnValue;
}

// ***
// *** Read the data from the wire
// ***
int Rpi2BridgeInternal::getDataFromWire(byte buffer[])
{
  int index = 0;

  while (Wire.available())
  {
    buffer[index] = Wire.read();
    index++;
  }

  return index;
}

void Rpi2BridgeInternal::setResult(byte result)
{
	this->resetOutputBuffer();

	#ifdef DEBUG_MODE
	Serial.print("\tsetResult(");
	Serial.print(result);
	Serial.println(")");
	#endif

	this->outBuffer[0] = result;
	this->bufferLength = 1;
}

void Rpi2BridgeInternal::setResult(byte result, byte value)
{
	this->resetOutputBuffer();

	#ifdef DEBUG_MODE
	Serial.print("\tsetResult(");
	Serial.print(result);
	Serial.print(", ");
	Serial.print(value);
	Serial.println(")");
	#endif

	this->outBuffer[0] = result;
	this->outBuffer[1] = value;
	this->bufferLength = 2;
}

void Rpi2BridgeInternal::setResult(byte result, unsigned int value)
{
	this->resetOutputBuffer();

	#ifdef DEBUG_MODE
	Serial.print("\tsetResult(");
	Serial.print(result);
	Serial.print(", ");
	Serial.print(lowByte(value));
	Serial.print(", ");
	Serial.print(highByte(value));
	Serial.println(")");
	#endif

	this->outBuffer[0] = result;
	this->outBuffer[1] = lowByte(value);
	this->outBuffer[2] = highByte(value);
	this->bufferLength = 3;
}

void Rpi2BridgeInternal::setResultDirect(byte result[], int byteCount)
{
	this->resetOutputBuffer();
	
	#ifdef DEBUG_MODE
	Serial.print("\tsetResult(byte[");	
	#endif
	
	for(int i = 0; i < byteCount; ++i)
	{
		#ifdef DEBUG_MODE
		Serial.print(result[i]);
		if (i < (byteCount - 1)) Serial.print(", ");
		#endif
		
		this->outBuffer[i] = result[i];
	}
	
	#ifdef DEBUG_MODE	
	Serial.println("])");
	#endif
		
	this->bufferLength = byteCount;
}

// ***
// *** Callback for received data
// ***
void Rpi2BridgeInternal::onReceive(int byteCount)
{
	#ifdef DEBUG_MODE
	Serial.print("Data received: [");
	Serial.print(byteCount);
	Serial.println(" byte(s)]");
	#endif

	// ***
	// *** Get the data from the wire
	// ***
	byte buffer[byteCount];
	int readCount = Rpi2Bridge.getDataFromWire(buffer);
	
	if (readCount == byteCount)
	{
		unsigned int registerId = Converter.bytesToUint(buffer[0], buffer[1]);
		
		#ifdef DEBUG_MODE
		Serial.print("\tRegister ID =  ");
		Serial.println(registerId);
		#endif

		if (registerId < MAXIMUM_COMMANDS)
		{
			if (Rpi2Bridge.registerMap[registerId].id != -1)
			{
				if (Rpi2Bridge.registerMap[registerId].id == registerId)
				{
					if (byteCount == Rpi2Bridge.registerMap[registerId].expectedByteCount)
					{
						if (Rpi2Bridge.registerMap[registerId].commandCallback != NULL)
						{
							#ifdef DEBUG_MODE
							Serial.println("\tExecuting command:");
							#endif
							
							// ***
							// *** Call the commandCallback for this register
							// ***
							Rpi2Bridge.registerMap[registerId].commandCallback(byteCount, buffer);
						}
						else
						{
							Rpi2Bridge.setResult(RESULT_MISSING_CALLBACK);
							
							#ifdef DEBUG_MODE
							// ***
							// *** Display not supported message
							// ***
							Serial.println("\tMissing callback.");
							#endif
						}
					}
					else
					{
						// ***
						// *** Specify that the buffer size was unexpected.
						// ***
						Rpi2Bridge.setResult(RESULT_UNEXPECTED_BUFFER, Rpi2Bridge.registerMap[registerId].expectedByteCount);
						
						#ifdef DEBUG_MODE
						// ***
						// *** Display not supported message
						// ***
						Serial.println("\tUnexpected buffer size.");
						#endif
					}
				}
				else
				{
					// ***
					// *** Specify that the Register ID does not match the mapping
					// ***
					Rpi2Bridge.setResult(RESULT_MAPPING_MISMATCH);
					
					#ifdef DEBUG_MODE
					// ***
					// *** Display not supported message
					// ***
					Serial.println("\tMapping mismatch.");
					#endif
				}
			}
			else
			{
				// ***
				// *** Specify that the command is not supported/mapped.
				// ***
				Rpi2Bridge.setResult(RESULT_COMMAND_NOT_SUPPORTED);

				#ifdef DEBUG_MODE
				// ***
				// *** Display not mismatch message
				// ***
				Serial.println("\tThe command is not supported/mapped.");
				#endif
			}
		}
		else
		{
			// ***
			// *** Specify that the Register ID is too large.
			// ***
			Rpi2Bridge.setResult(RESULT_ID_TOO_LARGE);
			
			#ifdef DEBUG_MODE
			// ***
			// *** Display not supported message
			// ***
			Serial.println("\tThe Register ID is too large.");
			#endif
		}
	}
	else
	{
		// ***
		// *** Specify that there is a buffer error
		// ***
		Rpi2Bridge.setResult(RESULT_BUFFER_ERROR);
		
		#ifdef DEBUG_MODE
		// ***
		// *** Display not buffer error
		// ***
		Serial.println("\tBuffer size error.");
		#endif
	}
}

// ***
// *** Callback for sending data
// ***
void Rpi2BridgeInternal::onRequest()
{
	#ifdef DEBUG_MODE
	Serial.println("Request for data:");
	Serial.print("\tThere are ");
	Serial.print(Rpi2Bridge.bufferLength);
	Serial.println(" byte(s) available.");
	#endif

	if (Rpi2Bridge.bufferLength > 0)
	{
		#ifdef DEBUG_MODE
		for (int i = 0; i < Rpi2Bridge.bufferLength; i++)
		{
			Serial.print("\tWriting byte[ ");
			Serial.print(i);
			Serial.print("] = ");
			Serial.println(Rpi2Bridge.outBuffer[i]);
		}
		#endif

		// ***
		// *** Write the data
		// ***
		Wire.write(Rpi2Bridge.outBuffer, Rpi2Bridge.bufferLength);

		// ***
		// *** Reset the buffer
		// ***
		Rpi2Bridge.resetOutputBuffer();
	}
}

void Rpi2BridgeInternal::resetOutputBuffer()
{
	for (int i = 0; i < Rpi2Bridge.bufferLength; i++)
	{
		Rpi2Bridge.outBuffer[i]	= 255;
	}
	
	// ***
	// *** Clear the buffer
	// ***
	Rpi2Bridge.bufferLength = 0;
}

// ***
// *** Global Object
// ***
Rpi2BridgeInternal Rpi2Bridge = Rpi2BridgeInternal(DEFAULT_ADDRESS);