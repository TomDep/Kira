#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include <stdint.h>
#include <unistd.h> 

#include "I2C_Interface.hpp"

#define ARDUINO_MOTOR_I2C_ADDRESS 0x0C


#define I2C_COMMAND__STOP_ALL         0
#define I2C_COMMAND__ENABLE_POWER     1
#define I2C_COMMAND__DISABLE_POWER    2
#define I2C_COMMAND__GET_BATTERY_LVL  3
#define I2C_COMMAND__GET_POSITION     4



class Controller {
	
	private:

	I2C_Interface* i2c;
	
	public:
	
	Controller();
	~Controller();
	
	void stopAll();
	void enablePower();
	void disablePower();
	uint16_t getBatteryLevel();
	void getPosition(int16_t* posX, int16_t* posY, int16_t* angle);
	
	void flush();
	
};

#endif // __CONTROLLER_H__
