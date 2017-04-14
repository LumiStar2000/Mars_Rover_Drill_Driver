
  //runTheseMotors[3] = corePlateRotate

//rotates the spares plate one step
//must be used in combination with other movements and a delay.
// returns true because idk what to check.
bool sparesPlateRotateOneStepNoDelay(bool forward){
  dir[3] = forward;
  if(forward){
    currentPosition++;
    if(currentPosition > stepsPerRevolution) { //if we surpass 360 degrees
      currentPosition = 0; // just go back to 0.
    }
  } else {
    currentPosition--;
    if (currentPosition < 0) { //same if we're going backwards.
      currentPosition = stepsPerRevolution; //reset to max.
    }
  //actually do the step  
  halfStep(CORE_PLATE_MOTOR);
  }
  return true; //rotation can't fail.
}

//rotate one step with proper delay.
// returns true because idk what to check.
bool sparesPlateRotateOneStep(bool forward){
  sparesPlateRotateOneStepNoDelay(forward);
  delay(1);
  return true;
}

//button methods for alignment
void sparesPlateForward(bool buttonPressed) {
  if(buttonPressed){
    sparesPlateRotateOneStep(MOTOR_FORWARD);
  }
}

//button method for alignment
void sparesPlateBackward(bool buttonPressed) {
  if(buttonPressed){
    sparesPlateRotateOneStep(!MOTOR_FORWARD);
  }
}

//DOES NOT MOVE MOTOR:
//sets it within the bounds of stepsPerRevolution.
//most useful to reset it to useful positions.
void setCurrentPlatePosition(int pos){
  currentPosition = pos;
}
int getCurrentPlatePosition(){
  return currentPosition;
}


