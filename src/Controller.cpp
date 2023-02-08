#include "Controller.hpp"

Controller::Controller(){
	this->i2c = new I2C_Interface(ARDUINO_MOTOR_I2C_ADDRESS);
}

Controller::~Controller(){
	delete this->i2c;
}

void Controller::stopAll(){
	this->i2c->sendByte(I2C_COMMAND__STOP_ALL);
}


void Controller::enablePower(){
	this->i2c->sendByte(I2C_COMMAND__ENABLE_POWER);
}


void Controller::disablePower(){
	this->i2c->sendByte(I2C_COMMAND__DISABLE_POWER);
}

uint16_t Controller::getBatteryLevel(){
	this->i2c->sendByte(I2C_COMMAND__GET_BATTERY_LVL);
	return this->i2c->readShort();
}

void Controller::getPosition(int16_t* posX, int16_t* posY, int16_t* angle){
	this->i2c->sendByte(I2C_COMMAND__GET_POSITION);
	*posX = this->i2c->readShort();
	*posY = this->i2c->readShort();
	*angle = this->i2c->readShort();
	return;
}

void Controller::flush(){
	this->i2c->flushIn();
}

