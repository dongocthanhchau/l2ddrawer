#ifndef __servo_h__
#define __servo_h__
/* custom servo class
 *  based on main Servo.h for ESP32.
 *  https://github.com/RoboticsBrno/ESP32-Arduino-Servo-Library
 *  create at 050122 by chad
 */
#include "Arduino.h"
#include <Servo.h>
class servo {
///////
  private:
    int _pinout;
    float _cAng; //currentAngle
    int _response = 4; // 4ms per 1 deg at 7.4Volt
    Servo _ser;
///////    
  public:
    servo(void);
    servo(int _pinout);
    void pinoutChange(int _pinout);
    void init();
    void init(int _defaultAng);
    void moveToAngle(float _ang);
    float getAngle();
};

#endif
