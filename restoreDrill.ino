//this group of methods aligns the drill with the magnetic sensor and prepares it for cleaning.
// we should:
// align to the magnet.
// adjust threads up or down to match the position of the notches. (auger height)
// move the cleaning blade in.
// begin synchronous rotation
// move cleaning blade out and restore back to magnetic position (or any position)
// angle of threads: 8.19 degrees
// angle of movement: 8.13 degrees.

const int lateralStepsPerStep = 7; //tan(1/7) = 8.13 degrees.
const bool moveDrillDownwards = true;
const long MAX_CLEANING_ROTATIONS = 10; // 10 rotations max. 
// calculates the amount of steps needed to revolve the full cleaning process.
// then divides it by the number of discrete steps.
const long MAX_CLEANING_STEPS = stepsPerRevolution * MAX_CLEANING_ROTATIONS / (long)lateralStepsPerStep;

long AUGER_CLEANING_HEIGHT = stepsPerRevolution * AUGER_LENGTH /2;
long AUGER_CLEANING_ROTATION = 0;
  

//returns true if cleaning succeeded, false if it failed.
//we should probably begin skipping steps if any of the previous steps failed.
bool cleanDrill (){
  bool methodSucceeded;
  methodSucceeded = moveDrillIntoCleaningPosition();
  //snap cleaning blade into place:
  if(methodSucceeded){ methodSucceeded = activateSolenoids();  }
  // dump out if alignment failed.
   if(!methodSucceeded) { return false; } 
  
  //begin cleaning movement.
  for(long i = 0; i < MAX_CLEANING_STEPS; i++){
    //if the drill fails, dump early.
    if(!cleaningMovement()){ 
      return false;
    }
  }
  return true; //drill finished it's cleaning movement.
}
//returns true if the drill did one cleaning step successfully (they're small steps).
bool cleaningMovement(){
   rotateDrill(MOTOR_FORWARD, lateralStepsPerStep -1); //rotate the drill 6 steps
    rotateDrillOneStepNoDelay(MOTOR_FORWARD); //6+1 = 7, no need for delay on last step.
    return moveAugerOneStep(!moveDrillDownwards); //moves down while cleaning.
}

bool moveDrillIntoCleaningPosition(){
  //steps needed to move downwards to get to auger cleaning height.
  //this number could be negative (need to move upwards)
  long stepsToMoveDown = AUGER_CLEANING_HEIGHT - getAugerCurrentPosition();
  bool moveDown = true;
  if(stepsToMoveDown < 0){
    stepsToMoveDown = (-1)*stepsToMoveDown;
    moveDown = false;
  }
  //this number will always be positive.
  long stepsToRotate = (stepsPerRevolution - getCurrentAlignment()) + AUGER_CLEANING_ROTATION;
  
  long stepCounter = 0;
  
  //first, spin them both until one aligns.
  while(stepCounter < stepsToMoveDown && stepCounter < stepsToRotate){
    moveDrillAndAugerOneStep(moveDown, MOTOR_FORWARD);
    stepCounter++;
  }
  
  //now spin whichever didn't finish.
  while(stepCounter < stepsToMoveDown){
    moveAugerOneStep(moveDown);
    stepCounter++;
  }
  while(stepCounter < stepsToRotate){
    if(!rotateDrillOneStep(MOTOR_FORWARD)){return true;} //TODO: replace all blind calls with if! dumps like this.
  }

  return true;
}

//returns true if both drills took one step.
bool moveDrillAndAugerOneStep(bool augerForward, bool drillForward){
  bool retPally = true;
  if(!moveAugerOneStepNoDelay(augerForward)) { return false; } // dump if auger failed.
  if(!rotateDrillOneStepNoDelay(drillForward)) { return false; } // dump if auger failed.
  delay(1);  //if both succeeded, delay and return true.
  return true;
}

