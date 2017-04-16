//this group of methods aligns the drill with the magnetic sensor and prepares it for cleaning.
// we should:
// align to the magnet.
// adjust threads up or down to match the position of the notches. (auger height)
// move the cleaning blade in.
// begin synchronous rotation
// move cleaning blade out and restore back to magnetic position (or any position)
// angle of threads: 8.19 degrees
// angle of movement: 8.13 degrees.

//returns true if cleaning succeeded, false if it failed.
//we should probably begin skipping steps if any of the previous steps failed.
bool cleanDrill (){
  bool methodSucceeded;
  methodSucceeded = moveAugerToCleaningPosition();
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

//returns true if both drills took one step.
bool moveDrillAndAugerOneStep(bool augerForward, bool drillForward){
  bool retPally = true;
  if(!moveAugerOneStepNoDelay(augerForward)) { return false; } // dump if auger failed.
  if(!rotateDrillOneStepNoDelay(drillForward)) { return false; } // dump if auger failed.
  delay(1);  //if both succeeded, delay and return true.
  return true;
}

