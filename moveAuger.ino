

 //moves auger to the top.  Returns true if endstop was reached before max turns.
 //  max turns is hard coded to be 6 full revolutions.
 bool moveAugerUp() {
  //switch motor direction to forwards
  dir[AUGER_MOTOR] = MOTOR_FORWARD;
  //maximum motor turns before method quits.  Adjust once enstop is added.
  const int maxMotorTurns = (4096*6); // 6 full rotations
  int curTurns = 0;

  //move until either endstop is hit or maxMotorTurns
  while(augerEndstop && curTurns < maxMotorTurns) {
     halfStep(AUGER_MOTOR);
     curTurns++; //current turn increment
     stepsSinceEndstop--; //we're moving towards the endstop
     delay(1); //minimum motor delay between steps
    }//endwhile
  
  //if we're at the endstop, let's reset it.
  if(augerEndstop)  {
      stepsSinceEndstop = 0;
  }
  
  return augerEndstop; 
 } //end method 

 //move auger a number of steps.  
// returns false if top was hit and dumps the method.
 bool moveAugerUp(long steps) {
  //switch motor direction to forwards
  dir[AUGER_MOTOR] = MOTOR_FORWARD;
  int curTurns = 0;
  //move up only if we're not at the endstop
  while(augerEndstop && curTurns < steps) {
    halfStep(AUGER_MOTOR);
    curTurns++;
    stepsSinceEndstop--; //we're moving towards the endstop
    if(augerAtTop()){
      return false;
    }
    delay(1); //minimum motor delay between steps
  }
  return true; //return true if endstop wasn't hit.
  } 

//moves auger towards the ground.  Returns false if maximum steps since endstop has been reached.
  bool moveAugerDown(long steps){
    //switch motor direction to backwards
    dir[AUGER_MOTOR] = !MOTOR_FORWARD;
    for(int curStep = 0; curStep < steps; curStep++)
    {
      halfStep(AUGER_MOTOR);
      stepsSinceEndstop++;

      //if we go too far, terminate early.
      if(stepsSinceEndstop > MAX_AUGER_STEPS) {
        return false;
      }
      delay(1);
    }

    //function executed fully
    return true;
    } 


  //moves the motor forward without delay.
  //to be used only in combinational movement.
  //please use the other methods for singular movement.
  // "forward" is down.  returns true if movement was successful
   bool moveAugerOneStepNoDelay(bool forward){
      dir[AUGER_MOTOR] = forward;
      if((stepsSinceEndstop > MAX_AUGER_STEPS) && forward) {
        return false; // tried to move too far downwards.
      }
      if((stepsSinceEndstop < 1 || augerEndstop) && !forward) {
        return false; // tried to move too far upwards.
      }
      halfStep(AUGER_MOTOR); 
      if(forward){
        stepsSinceEndstop++;
      } else {
        stepsSinceEndstop--;
      }
      return true; 
      // returns false if endstop hit or we think we're at the endstop, 
      //  or if the maximum steps have been reached.
      // returns true if movement was successful.
   }
   

   //moves the auger motor forward one step with delay.
   //can be used for any movement.    
  // "forward" is down.  returns true if movement was successful
   bool moveAugerOneStep(bool forward){
    bool retPally = moveAugerOneStepNoDelay(forward);
    if(retPally){ //only delay if the damn pally moved
    delay(1);
    }
    return retPally; //aren't retribution paladins the WORST?!
   }
   
  //TODO: something about this method.
   //is this method even possible?
 bool moveAugerAskUser() {
  Serial.println("Number of steps?");  
  } //asks the user through serial input for a number of steps


  //moves the motor up when a button is pressed.  will reset stepsSinceEndstop if necessary.
  //can be called in a loop thanks to the bool input method (checks pin each time, no extra code needed)
  //ex: moveAugerUp(checkUpButton());
 void moveAugerUp(bool moveUp) { //'forward' is down.
  if(moveUp){
    //first, attempt to move the motor
    if(!moveAugerOneStep(!MOTOR_FORWARD)){ //"not forward" is up.
      //If the endstop was hit, we will pretend it doesnt exist.
      stepsSinceEndstop = 1; //resets the value to one continuously
      moveAugerOneStep(!MOTOR_FORWARD); //moves stepsSinceEndstop back to 0.
    }
  }
}

  //moves the motor down when a button is pressed.  Fails if maximum distance reached.
  //can be called in a loop thanks to the bool input method (checks pin each time, no extra code needed)
 void moveAugerDown(bool moveDown) {
  if(moveDown){ 
    //first, attempt to move the motor
    if(!moveAugerOneStep(MOTOR_FORWARD)){ //"forward" is down
      //If we're at the maximum distance, there's nothing we can do.
      Serial.println("Maximum distance reached.  Please adjust MAX_AUGUR_STEPS");
      //alert user.
    }
  }
}



//moves the auger to the surface.
// returns false if the method dumps (invalid starting position, movement fails, etc)
bool moveAugerToTheSurface(){
  //while we're above the surface
  if(stepsSinceEndstop > theSurface){
    Serial.println("You must be above the surface before you can move towards it!");
    return false;
  }
  while(stepsSinceEndstop < theSurface){
    //attempt to move to the surface
    if(!moveAugerOneStep(MOTOR_FORWARD)){
      //dump if we fail to move (the movement returns false)
      Serial.println("Auger motor failure");
      return false;
    }
  }
  return true;
}

//sets the surface to be the drill's current position.
void setTheSurface(){
  theSurface = stepsSinceEndstop;
}

//gets the current position.
long getAugerCurrentPosition(){
  return stepsSinceEndstop;
}

//moves the auger to the extraction position.
// returns true if it ended in said position.
bool moveAugerToExtractionPosition(){
  //move it up if it's too far down.
  while(stepsSinceEndstop > AUGER_EXTRACTION_POSITION){
    if(!moveAugerOneStep(MOTOR_FORWARD)){return false;}
  }
  //move it down if it's too far up.
  while(stepsSinceEndstop < AUGER_EXTRACTION_POSITION){
    if(!moveAugerOneStep(!MOTOR_FORWARD)){return false;}
  }
  //if all is right, this should be true.
  //if the lengths don't match, it'll be false.
  return (stepsSinceEndstop == AUGER_EXTRACTION_POSITION);
}

//moves the auger to the cleaning position.
// returns true if it ended in said position.
bool moveAugerToCleaningPosition(){
  //move it up if it's too far down.
  while(stepsSinceEndstop > AUGER_CLEANING_POSITION)  {
    if(!moveAugerOneStep(MOTOR_FORWARD)) {return false;}
  }
  //move it down if its too far up.
  while(stepsSinceEndstop < AUGER_CLEANING_POSITION) {
    if(!moveAugerOneStep(!MOTOR_FORWARD)) {return false;}
  }

  return (stepsSinceEndstop == AUGER_CLEANING_POSITION);
}


//checks endstop and current position.
//will not allow drill to run fully sheathed.
 bool augerAtTop(){
  if(augerEndstop || (stepsSinceEndstop <= 0)){
    return true;
  }
  return false;
}


