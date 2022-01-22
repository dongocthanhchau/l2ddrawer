#ifndef __arm_h__
#define __arm_h__
/* 
 *  custom drawer machine 
 *  using 3 servo:
 *  - left servo
 *  - right servo
 *  - pen lift servo
 *  using https://www.instructables.com/Mini-Drawing-Bot-Live-Android-App-Trignomentry/
 *  and based on servo controller library servo by chad 
 *  create at 060122 by chad
 */
 #include "servo.h"
 #include "Arduino.h"
 class arm{
  private:
    // device configuration
    int _a1; //arm 1 long in centimeters
    int _a2; //arm 2 long in centimeters
    int _s; //distance between ser1 and ser2
    //pintout
    int SERL_PIN= 5;//(0,0) servo attach
    int SERR_PIN= 6;
    int SERP_PIN= 7; 
    int currentX,currentY;
    float _leftAng,_rightAng,_penAng;
    float _penUpAngle=0,_penDownAngle=90;
    //motor/servo library
    servo lServ,rServ,pServ;
    double distance(float x1, float y1, float x2,float y2);
    double calcDeg(double _opp, double _side1, double _side2);
    void servoCalc(float x,float y);
  public:
    arm(int leftpin, int rightpin, int penpin);
    arm(int leftpin, int rightpin, int penpin,float a1, float a2, float s, float penupAngle, float pendownAngle);
    void init(void);
    void moveXY(float x, float y);
    void penUp(void);
    void penDown(void);
    void line(float x1, float y1, float x2,float y2);
    void getXY(float &x, float &y);
    void penAngle(float anglePen);
    void moveAngle(float angleL, float angleR, int delayTime);
    
};
#endif
