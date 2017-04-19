

//rotates the drill one step.  returns false is drill is fully sheathed.
//must be used in combination with other movements and at least one delay.
bool rotateDrillOneStepNoDelay(bool forward){
  if(augerAtTop()){
    return false;
  }
  dir[DRILL_MOTOR] = forward;
  halfStep(DRILL_MOTOR);

  //advances position vector. 
  //bounds are 0, stepsPerRevolution-1.
  // if it's out of bounds, it loops around.
  if(forward){
    drillPosition++;
    if(drillPosition >= stepsPerRevolution){
      drillPosition = 0;
    }
    }
  else {
    drillPosition--;
    if(drillPosition < 0)
    {
      drillPosition = stepsPerRevolution - 1;
    }
  }
  
  return true; 
}

//for independent movement of the drill.  delay(1).
bool rotateDrillOneStep(bool forward){
  rotateDrillOneStepNoDelay(forward);
  delay(1);
  return true;
}

//if the button is active, go forward.
bool rotateDrillForward(bool buttonOn){
  if(buttonOn) {
    return rotateDrillOneStep(MOTOR_FORWARD);
  } 
  return false;
}

//if the button is active, go backward.
bool rotateDrillBackward(bool buttonOn){
  if(buttonOn){
    return rotateDrillOneStep(!MOTOR_FORWARD);
  }
}

//allows rapid drilling.
bool rotateDrill(bool forward, long steps){
  dir[DRILL_MOTOR] = forward;
  for(long i = 0; i < steps; i++)
  {
    halfStep(DRILL_MOTOR);
    delay(1);
  }
  return true;
}

//rotates the drill forward until it's aligned at the current position.
// could be useful for aligning threads to screw on a new head.
// pos can be any int from 0 to stepsPerRevolution (4096).
bool rotateDrillForwardToAlignment(int pos){
  
  //dump out if alignment is impossible.
  if(pos < 0 || pos >= stepsPerRevolution){
    return false;
  }

  //input validates, just loop until it's done. 
  while(getCurrentAlignment() != pos){
    rotateDrillOneStep(MOTOR_FORWARD);
  }
  return true;
}

//returns current alignment.
int getCurrentAlignment(){
  return drillPosition;
}

//if its within POSITION_TOLERANCE rotations of SPARES_ANGULAR_POSITION, return true.
//  currently means if it's +-5 rotations (0.44 degrees)
bool readyForSpare(){
  if(drillPosition >= (SPARES_ANGULAR_POSITION - ANGULAR_POSITION_TOLERANCE) &&
      drillPosition <= (SPARES_ANGULAR_POSITION + ANGULAR_POSITION_TOLERANCE)){
        return true;
      }
  return false;
}


