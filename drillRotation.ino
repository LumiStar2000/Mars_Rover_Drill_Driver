

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


