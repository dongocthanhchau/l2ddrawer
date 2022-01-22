#include "arm.h"

double arm::distance(float x1, float y1, float x2,float y2){
  return sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) );
}
double arm::calcDeg(double _opp, double _side1, double _side2){
  return 180*acos( (_side1*_side1+_side2*_side2-_opp*_opp)/(2*_side1*_side2))/3.141592;
}
void arm::servoCalc(float x,float y){
  float __d1,__d2;
  __d1 = distance(0,0,x,y);
  __d2 = distance(_s,0,x,y);
  this->_leftAng = calcDeg(__d2,_s,__d1)+calcDeg(_a2,_a1,__d1);
  if (_leftAng>180) _leftAng=180;
  if (_leftAng<30) _leftAng=30;
  if (_rightAng>150) _rightAng=150;
  if (_rightAng<0) _rightAng=0;
  this->_rightAng = 180-calcDeg(__d1,_s,__d2)-calcDeg(_a2,_a1,__d2);
//  Serial.println("LA");
//  Serial.println(_leftAng);
//  Serial.println("RA");
//  Serial.println(_rightAng);
  
}
arm::arm(int leftpin, int rightpin, int penpin){
  SERL_PIN = leftpin;
  SERR_PIN = rightpin;
  SERP_PIN = penpin;
}
arm::arm(int leftpin, int rightpin, int penpin,float a1, float a2, float s, float penupAngle, float pendownAngle){
  SERL_PIN = leftpin;
  SERR_PIN = rightpin;
  SERP_PIN = penpin;
  this->_a1 = a1;
  this->_a2 = a2;
  this->_s  = s;
  this->_penUpAngle=penupAngle;
  this->_penDownAngle=pendownAngle;
}
void arm::init(void){
  //servo init
  lServ.pinoutChange(SERL_PIN);
  rServ.pinoutChange(SERR_PIN);
  pServ.pinoutChange(SERP_PIN);
  lServ.init();
  rServ.init();
  pServ.init();
  pServ.moveToAngle(_penUpAngle);
  delay(100);
  lServ.moveToAngle(90.0);
  rServ.moveToAngle(90.0);
  this->currentX= sqrt(_a2*_a2-_s*_s/4)+_a1 ;
  this->currentY= _s/2 ;
}
void arm::moveXY(float x, float y){
  // seperate into 2 problem

  servoCalc(x,y);
  this->currentX= x;
  this->currentY= y;
  _penAng=pServ.getAngle();
  int delayTime =0;
  if (_penAng == _penUpAngle){
    delayTime=0;
  } else{
    delayTime=5;
  }
  float __lAng,__rAng,__maxAng,__clAng,__crAng;
  __clAng = lServ.getAngle();
  __crAng = rServ.getAngle();
  __lAng = -__clAng+_leftAng;
  __rAng = -__crAng+_rightAng;
  __maxAng = max(abs(__lAng),abs(__rAng));
  __maxAng /=0.3;
  for (int __i=0;__i<__maxAng;__i++){
    lServ.moveToAngle(__clAng+__i*__lAng/__maxAng);
    rServ.moveToAngle(__crAng+__i*__rAng/__maxAng);
    delay(delayTime);
  }
}
void arm::penUp(void){
  int _a=1;
  if (_penDownAngle-_penUpAngle>0) _a=-1;
  for (int i=0;i<abs(_penDownAngle-_penUpAngle);i++){
    pServ.moveToAngle(_penDownAngle+_a*i);
  }
  pServ.moveToAngle(_penUpAngle);
  Serial.println("PENUP");
  delay(10);
}
void arm::penDown(void){
  int _a=1;
  if (_penDownAngle-_penUpAngle<0) _a=-1;
  for (int i=0;i<abs(_penDownAngle-_penUpAngle);i++){
    pServ.moveToAngle(_penUpAngle+_a*i);
  }
  pServ.moveToAngle(_penDownAngle);
  Serial.println("PENDOWN");
  delay(10);
}

void arm::line(float x1, float y1, float x2,float y2){
  //if (pServ.getAngle()==
//  Serial.println(x1);
//  Serial.println(y1);
//  Serial.println(x2);
//  Serial.println(y2);
  if (this->currentX!=x1 || this->currentY!=y1){
    this->penUp();
    this->moveXY(x1,y1);
    this->penDown();
  }
  if (pServ.getAngle()==_penUpAngle)
    this->penDown();
  this->moveXY(x2,y2);
  //this->penUp();
}
void arm::getXY(float &x, float &y){
  x = this->currentX;
  x = this->currentY;
}
void arm::moveAngle(float angleL, float angleR, int delayTime){
  float __maxAng,__clAng,__crAng,__lAng,__rAng;
  __clAng = lServ.getAngle();
  __crAng = rServ.getAngle();
//  __lAng = -__clAng+_leftAng;
//  __rAng = -__crAng+_rightAng;
  __lAng = -__clAng+angleL;
  __rAng = -__crAng+angleR;
  __maxAng = max(abs(__lAng),abs(__rAng));
  __maxAng /=0.3;
  for (int __i=0;__i<__maxAng;__i++){
    lServ.moveToAngle(__clAng+__i*__lAng/__maxAng);
    rServ.moveToAngle(__crAng+__i*__rAng/__maxAng);
    delay(delayTime);
  }
}
void arm::penAngle(float anglePen){
  int _pAng=pServ.getAngle();
  
  int _a=1;
  if (_pAng-anglePen<0) _a=-1;
  for (int i=0;i<abs(_penDownAngle-_penUpAngle);i++){
    pServ.moveToAngle(_penUpAngle+_a*i);
  }
  pServ.moveToAngle(_penDownAngle);
  delay(5);
}
