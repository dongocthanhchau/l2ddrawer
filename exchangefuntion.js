
//variable _s: distance from servo left to servo right (in mm)
//variable _a1: distance from servo to mid (in mm)
//variable _a2: distance from mid to pen (in mm)
_s = 47;
_a1 = 71;
_a2 = 93;
function distance(x1, y1, x2, y2){
  return Math.sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) );
}
function calcDeg(_opp, _side1, _side2){
  return 180*Math.acos( (_side1*_side1+_side2*_side2-_opp*_opp)/(2*_side1*_side2))/Math.PI;
}
function XYtoAngle(x,y) {
  __d1 = distance(0,0,x,y);
  __d2 = distance(_s,0,x,y);
  leftAng = calcDeg(__d2,_s,__d1)+calcDeg(_a2,_a1,__d1);
  rightAng = 180-calcDeg(__d1,_s,__d2)-calcDeg(_a2,_a1,__d2);
  return [leftAng, rightAng];
}
