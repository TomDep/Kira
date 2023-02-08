#ifndef __I2C_INTERFACE_H__
#define __I2C_INTERFACE_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#include <wiringPiI2C.h>

class I2C_Interface {
	private: 
	
	int fd;
	int address;
		
	public:
	
	I2C_Interface(uint8_t address);
	~I2C_Interface();
	
	void sendData(uint8_t* data, uint8_t length);
	void readData(uint8_t* buffer, uint8_t length);
	
	uint8_t readByte();
	uint16_t readShort();
	uint32_t readInt();
	uint64_t readLong();
	
	void sendByte(uint8_t data);
	
	void flushIn();
};



#endif // __I2C_INTERFACE_H__
