#include "I2C_Interface.hpp"

I2C_Interface::I2C_Interface(uint8_t address){
	this->address = address;
	this->fd = wiringPiI2CSetup(address);
	
	if(fd <= 0){
		printf("Error while creating I2C at address %d ! \n", address);
		return;
	}
}

I2C_Interface::~I2C_Interface(){
	close(this->fd);
}

void I2C_Interface::sendData(uint8_t* data, uint8_t length){
	for(uint8_t i=0;i<length;++i)
		wiringPiI2CWrite(this->fd, data[i]);
	
}

void I2C_Interface::readData(uint8_t* buffer, uint8_t length){
	for(uint8_t i=0;i<length;++i)
		buffer[i] = wiringPiI2CRead(this->fd);
}

uint8_t I2C_Interface::readByte(){
	uint8_t data;
	readData(&data, 1);
	return data;
}

uint16_t I2C_Interface::readShort(){
	uint16_t data;
	readData((uint8_t*)&data, 2);
	return data;
}

uint32_t I2C_Interface::readInt(){
	uint32_t data;
	readData((uint8_t*)&data, 4);
	return data;
}

uint64_t I2C_Interface::readLong(){
	uint64_t data;
	readData((uint8_t*)&data, 8);
	return data;
}

void I2C_Interface::sendByte(uint8_t data){
	sendData(&data, 1);
}


void I2C_Interface::flushIn(){
	uint8_t data = 0;
	while(readByte() != 0xA5);
}


