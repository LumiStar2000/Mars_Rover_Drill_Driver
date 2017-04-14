int sparesArmPosition = 0; //start it at 0.  forward is ++
const int MAXIMUM_ARM_ROTATION = stepsPerRevolution / 3; //360 degrees, into a slice of pie.
const int CORE_PLATE_ARM_MOTOR = 2;


bool sparesPlateArmOneStepNoDelay(bool forward) {
  dir[CORE_PLATE_ARM_MOTOR] = forward;
  if(forward && (sparesArmPosition < MAXIMUM_ARM_ROTATION)){//if we're going forward and we've not gone too far
    halfStep(CORE_PLATE_ARM_MOTOR);
    sparesArmPosition++;
    return true;//it moved
  } else if (!forward && (sparesArmPosition > 0)){
    halfStep(CORE_PLATE_ARM_MOTOR);
    sparesArmPosition--;
    return true;//it moved
  }

  //it didn't move, because its outside the arm position.
  return false;
}

bool sparesPlateArmOneStep(bool forward){
  bool retPally = sparesPlateArmOneStepNoDelay(forward);
  delay(1);
  return retPally; //return all ret pally's to sender.  We don't want their kind here.
}

//button methods
void sparesPlateArmForward(bool buttonPressed){
  if(buttonPressed){
    dir[CORE_PLATE_ARM_MOTOR] = MOTOR_FORWARD;

    //attempt to move.  If it fails, do it better.
    if(!sparesPlateArmOneStep(MOTOR_FORWARD)){
      //set the new maximum rotation to it's current position
      sparesArmPosition = MAXIMUM_ARM_ROTATION-1;
      sparesPlateArmOneStep(MOTOR_FORWARD); //it is now at the new max position.
    }
  }
}
void sparesPlateArmAway(bool buttonPressed){
  if(buttonPressed){
    dir[CORE_PLATE_ARM_MOTOR] = !MOTOR_FORWARD;

    //attempt to move.  if it fails, do it better.
    if(!sparesPlateArmOneStep(!MOTOR_FORWARD)){
      //sets the new minimum rotation to it's current position
      sparesArmPosition = 1;
      sparesPlateArmOneStep(!MOTOR_FORWARD); // new minimum position.
    }
  }
}

