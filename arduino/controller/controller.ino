#include "stdint.h"
#include "CONFIG.hpp"
#include "Motor.hpp"
#include "Utils.hpp"
#include "I2C.hpp"


void I2C_onReceive(uint8_t data);


// Position and angle is in um and urad (micro-meters and micro-radian)
int32_t positionX, positionY, angle;
// Velocity and angular velocity in um/s and urad/s
int32_t velocity, rotation_velocity;

const int16_t distancePerTick = 1000L * ODO_WHEEL_D_MM * PI / ODO_TICK_PER_TURN;

int8_t roueCodeuseDTick, roueCodeuseGTick;
uint32_t odometryTick;
int16_t delta_distance, delta_angle, delta_X, delta_Y;

static const int32_t delta_t_us = (1000000/ODO_TICK_SPEED);


#ifdef DEBUG
uint16_t odometryMeanTime;
#endif



bool power_enable, aru_state;

// ----- PID CONTROLLER ------
#define CONTROLLER_TASK__NONE   0x00
#define CONTROLLER_TASK__ANGLE  0x01
#define CONTROLLER_TASK__POINT  0x02

#define ANGLE__KP 0.000040f
#define ANGLE__KI 0.00000003f

uint8_t controllerTask = CONTROLLER_TASK__NONE;
uint16_t targetAnglePrecision = 5000;


// Targets
float targetAngle = 1000000.0f * PI;
float targetAngleI = 0;

void setup() {

  Serial.begin(SERIAL_SPEED);
  I2C_Init(I2C_onReceive);
  
  initGPIO();




  attachInterrupt(digitalPinToInterrupt(PIN_CODEUR_D_PULSE), ISR_codeurD, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIN_CODEUR_G_PULSE), ISR_codeurG, FALLING);


#ifdef FORCE_POWER_ENABLE
  digitalWrite(PIN_POWER_ENABLE, true);
  power_enable = true;
#endif
  
  positionX = positionY = angle = velocity = rotation_velocity = 0;
  roueCodeuseDTick = roueCodeuseGTick = odometryTick = 0;

  aru_state = false;
  
  configureTimer();

  //delay(1000);
  controllerTask = CONTROLLER_TASK__ANGLE;
}


void loop() {
#ifdef DEBUG
  Serial.print("\n\n  --== Loop ==--\n\n");
#endif

  // Check battery level
  updateBatteryLevel();
  aru_state = getARU();

  if(aru_state){
    power_enable = false;
  }

  // Check if need to turn ON power
  bool state = false;
#ifdef FORCE_POWER_ENABLE
  #ifdef IGNORE_BATTERY_LEVEL
    state = true;
  #else
    state = !low_battery;
  #endif
#else
  #ifdef IGNORE_BATTERY_LEVEL
    state = power_enable;
  #else
    if(!low_battery){
      state = power_enable;
    }
  #endif
#endif

  digitalWrite(PIN_POWER_ENABLE, state);


  

#ifdef DEBUG
  uint32_t tps = 1000 * odometryTick / millis();
  Serial.print(F("Position : \n  X : "));
  Serial.print(positionX/1000L);
  Serial.print(F(" mm\n  Y : "));
  Serial.print(positionY/1000L);
  Serial.print(F(" mm\n  Angle : "));
  Serial.print(angle/1000L*180L/PI);
  Serial.println(F(" mdeg\n"));
  
  Serial.print(F("Linear velocity : "));
  Serial.print(velocity/1000L);
  Serial.print(F(" mm/s\nAngular velocity : "));
  Serial.print(rotation_velocity/1000L);
  Serial.println(F(" mrad/s\n"));
  
  Serial.print(F("Odometry tick count : "));
  Serial.print(odometryTick);
  Serial.print(F(" (Tick/s : "));
  Serial.print(tps);
  Serial.print(F(")\nOdometry mean time : "));
  Serial.print(odometryMeanTime);
  Serial.print(F(" µs ("));
  Serial.print(odometryMeanTime / (10000.0/ODO_TICK_SPEED) );
  Serial.println(F("%)\n"));

  Serial.print(F("ARU state : "));
  Serial.println(aru_state?F("STOP !"):F("Power enable"));
  Serial.print(F("Power state : "));
  Serial.println(power_enable?F("On"):F("Off"));
  
  Serial.print(F("\nBattery : "));
  Serial.print(battery_level);
  Serial.print(F("mV"));
  if(low_battery)
    Serial.print(F(" !!! LOW BATTERY !!!"));
  Serial.println();



  delay(200);

#endif


  updateMotors();


}

void controlAngle() {
  if(!power_enable)
    return;
  
  float angleError = angle - targetAngle;
  
  if(abs(angleError) < targetAnglePrecision){
    Serial.println("Target Done");
    targetAngleI = 0;
    return;
  }
  
  targetAngleI += angleError * ANGLE__KI;
  
  float command = (angleError * ANGLE__KP) + targetAngleI;
  
  int16_t cmdLeft = - (int16_t)command;
  int16_t cmdRight = (int16_t)command;
  
  setMotorSpeed(cmdLeft, cmdRight);
  //Serial.print(F("Angle error : "));
  //Serial.println(angleError);
}

void controlPoint() {

}


void odometryUpdate(){
#ifdef DEBUG
  odometryMeanTime = micros();
#endif
  ++odometryTick;
  
  /*
  int32_t delta_distance = (roueCodeuseDTick + roueCodeuseGTick) * distancePerTick / 2L;
  int32_t delta_angle = 1000L * (roueCodeuseDTick - roueCodeuseGTick) * distancePerTick / (ODO_ENTRAXE_MM);
  
*/

  delta_distance = ((roueCodeuseDTick + roueCodeuseGTick) * distancePerTick) / 2L;
  /*
  delta_angle = 1000L * (roueCodeuseDTick - roueCodeuseGTick) * distancePerTick / (ODO_ENTRAXE_MM);
  delta_angle /= 2L;*/
  delta_angle = 500L * (roueCodeuseDTick - roueCodeuseGTick) * distancePerTick / (ODO_ENTRAXE_MM);
  
  angle += delta_angle;
  delta_X = delta_distance * cos(angle/1000000.0);
  delta_Y = delta_distance * sin(angle/1000000.0);
  angle += delta_angle;

  
  positionX += delta_X;
  positionY += delta_Y;

  
  
  velocity = (velocity * 30L + delta_distance * delta_t_us) / 31L;
  rotation_velocity = (rotation_velocity * 30L + delta_angle * 2L * delta_t_us) / 31L;


  // ===== PID =====
  
  switch(controllerTask) {
    case CONTROLLER_TASK__ANGLE:
      controlAngle();
      break;
    case CONTROLLER_TASK__POINT:
      controlPoint();
      break;
    default: break;
  }


  
  roueCodeuseDTick = roueCodeuseGTick = 0;
  
#ifdef DEBUG
  odometryMeanTime = micros() - odometryMeanTime;
#endif
}






void I2C_onReceive(uint8_t data){

#ifdef I2C_DEBUG
    Serial.print(F("Data received from I2C : "));
    Serial.println(data);
#endif

  uint8_t uint8 = 0;
  switch(data){
  case I2C_COMMAND__STOP_ALL:
    reset();
    power_enable = false;
    digitalWrite(PIN_POWER_ENABLE, false);
    setMotorSpeed(0, 0);
    break;
  case I2C_COMMAND__ENABLE_POWER:
    power_enable = true;
    digitalWrite(PIN_POWER_ENABLE, true);
    break;
  case I2C_COMMAND__DISABLE_POWER:
    power_enable = false;
    digitalWrite(PIN_POWER_ENABLE, false);
    break;
  case I2C_COMMAND__GET_BATTERY_LVL:
    I2C_Write(battery_level);
    I2C_Write(battery_level >> 8);
    break;
  case I2C_COMMAND__GET_POSITION:
    I2C_Write(positionX/1000L);
    I2C_Write((positionX/1000L) >> 8);
    I2C_Write(positionY/1000L);
    I2C_Write((positionY/1000L) >> 8);
    I2C_Write(angle/1000L);
    I2C_Write((angle/1000L) >> 8);
    break;
  case 100:
    uint8 = I2C_Read();
    if(uint8 > 100){
      setMotorSpeed(-uint8 + 100, 0);
    }else{
      setMotorSpeed(uint8, 0);
    }
    break;
  case 101:
    uint8 = I2C_Read();
    if(uint8 > 100){
      setMotorSpeed(0, -uint8 + 100);
    }else{
      setMotorSpeed(0, uint8);
    }

    break;
  default:

    break;
  }
}




void ISR_codeurD(){
  bool dir = digitalRead(PIN_CODEUR_D_DIR);
  roueCodeuseDTick += dir?-1:1;
#ifdef CODEUR_DEBUG
  Serial.print("Codeur D : ");
  Serial.print(dir?"UP":"DOWN");
  Serial.print(" = ");
  Serial.println(roueCodeuseDTick);
#endif

}

void ISR_codeurG(){
  bool dir = digitalRead(PIN_CODEUR_G_DIR);
  roueCodeuseGTick += dir?1:-1;
#ifdef CODEUR_DEBUG
  Serial.print("Codeur G : ");
  Serial.print(dir?"UP":"DOWN");
  Serial.print(" = ");
  Serial.println(roueCodeuseGTick);
#endif
}
