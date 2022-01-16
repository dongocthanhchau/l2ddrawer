#include "servo.h"

servo::servo(void){
  this->_pinout =0;
}
servo::servo(int _pinout){
  this->_pinout = _pinout;
}
void servo::pinoutChange(int _pinout){
  this->_pinout = _pinout;
}
void servo::init(){
  _ser.attach(this->_pinout,Servo::CHANNEL_NOT_ATTACHED,0,180,500,2500);
  //ser.attach(this->pinout);
  delay(1);
  _ser.write(0);
  _cAng= 0;
}
void servo::init(int _defaultAng){
  _ser.attach(this->_pinout,Servo::CHANNEL_NOT_ATTACHED,0,180,500,2500);
  //ser.attach(this->pinout);
  delay(1);
  _ser.write(_defaultAng);
  _cAng = _defaultAng;
}
void servo::moveToAngle(float _ang){
  _ser.write(_ang);
  delay(abs(_cAng-_ang)*_response);
  this->_cAng=_ang;
  //Serial.print("moved to ");
  //Serial.println(_ang);
}
float servo::getAngle(){
  return(_cAng);
}
