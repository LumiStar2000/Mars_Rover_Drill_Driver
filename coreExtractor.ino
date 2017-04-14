const int CORE_ARM_MOTOR = 4; // pins 16-19
const long CORE_EXTRACT_DISTANCE_PER_ROTATION = 30;//units???
const double CORE_EXTRACT_ARM_TOTAL_ROTATIONS = 2.5;
const long CORE_EXTRACT_ARM_LENGTH = (long)(CORE_EXTRACT_ARM_TOTAL_ROTATIONS * stepsPerRevolution * CORE_EXTRACT_DISTANCE_PER_ROTATION);
long coreExtractArmPosition = 0; //0 = extract position.  positive means it's moving away from the auge
bool coreArmEndstop = false;
//MOTOR_FORWARD means AWAY from the auger.

//method lists for compiling
bool coreExtractArmIn();
void coreExtractArmIn(bool);
void coreExtractArmOut(bool);
bool coreExtractArmOut();
bool moveExtractionArmOneStep(bool);
bool moveExtractionArmOneStepNoDelay(bool);
bool coreArmAtExtractionPoint();
bool coreArmNotAtMaxDistance();
bool coreArmAtExtractPosition();

//moves the core extraction arm into extraction position
// returns false if the movement failed.
bool coreExtractArmIn(){
  while(!coreArmAtExtractionPoint()){
    if(!moveExtractionArmOneStep(MOTOR_FORWARD)){ return false;}
  }
  return true;
}
//method to be used with a button checker to manually align motors.
void coreExtractArmIn(bool movement){
  if(movement) {
    //attempt normal movement
    if(!moveExtractionArmOneStep(!MOTOR_FORWARD)){ //if initial movement fails, Override.
      coreExtractArmPosition = 1;// continuously set position to 1.
      moveExtractionArmOneStep(!MOTOR_FORWARD); //this moves it down to 0.
    }//hopefully that's the only reason the movement failed.
  }
}

//method to be used with a button checker to manually align motors.
//it will fail when max distance is reached.
void coreExtractArmOut(bool movement){
  if(movement){
    //attempt normal movement
    if(!moveExtractionArmOneStep(MOTOR_FORWARD))
    {
      if(!coreArmNotAtMaxDistance()){
        //if the core arm IS at max distance, then set it to one less than it.
        //then move the motor.
        coreExtractArmPosition = CORE_EXTRACT_ARM_LENGTH-1;
        if(!moveExtractionArmOneStep(MOTOR_FORWARD)){
          Serial.println("Programming error in coreExtractArmOut");
        }
      }
      }
    }
  }


//this is a straight up copypasta of the above method with a ! removed.
//good job todd, you coded correctly!
bool coreExtractArmOut(){
  long maximumSteps = stepsPerRevolution * 6;//6 full rotations before movement gives up.
  while((maximumSteps > 0) && moveExtractionArmOneStep(MOTOR_FORWARD))
  {maximumSteps--;} //stop when movement fails or when maximum steps was reached.
  
  return (maximumSteps > 0); //returns true if it ended before maximum steps was reached.
  }

//returns true if the movement succeeded.
//returns false if the movement failed.
bool moveExtractionArmOneStep(bool forward){
  bool retPally = moveExtractionArmOneStepNoDelay(forward);
  if(retPally){ //delay only if a retPally happened.
    delay(1);
    }
  return retPally; //who even MAINS a ret pally?!  so gross!
}

//moves the extraction arm one step, without delay.
//must be used in combination with other movements and a delay.
//returns true if the movement succeeded.
bool moveExtractionArmOneStepNoDelay(bool forward){
  dir[CORE_ARM_MOTOR] = forward; //proper direction
  //forward movement away from auger.
  if(forward && coreArmNotAtMaxDistance()){ //can it move farther away?
    halfStep(CORE_ARM_MOTOR);
    coreExtractArmPosition++;
    return true; //movement success
  }// not forward, now check can it move toward the auger.
  else if(!coreArmAtExtractionPoint()) //can it move closer in?
  {
    halfStep(CORE_ARM_MOTOR);
    coreExtractArmPosition--;
    return true; //movement success
  }

  return false; //your movement failed.
}
//returns true if it's at position 0 (extraction position)
bool coreArmAtExtractionPoint(){
  return (coreExtractArmPosition <= 0);
}
//returns true if it's not at max distance.
bool coreArmNotAtMaxDistance(){
  return (coreExtractArmPosition < CORE_EXTRACT_ARM_LENGTH);
}


