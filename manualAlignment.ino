//all of the maximums and minumums are programmed within their respective files.
//all this file does is use the methods of those files to set initial known values.
//the demonstration is responsible to adjust itself from the inital values.

// 4/12/17 calibration is going to be set to involve hard-coded distances of maximum rotation.
// these mthods will just reset the max or min value if it goes farther than expected.
// PROCEEDURE: move each motor from it's maximum position to it's minimum position.  
//    If the motor does not travel it's full path, then adjust the distance constant in it's file.
//    If it gets stunted, make sure all your >'s and <'s are in order.


//globals used:
// alignmentButtonPin[0 - 2] -> int containing pin number.
//  used to read HIGH / LOW via the last 2 methods (aB On / aB Off)
//  pin[0] : up/in direction 
//  pin[1] : down/out direction
//  pin[2] : next motor.  


bool manualAlignment() {
  
  //first, align the core hand to its outmost position.
  //  (or the innermost.  Break your drill, see if I care.)
  manualAlignCoreHand();
  //delay between each alignment step to avoid skipping alignment steps.
  delay(MANUAL_DELAY);
  
  //second, align the spares plate rotation, so that it can swing away freely.
  manualAlignSparesPlate();
  delay(MANUAL_DELAY);
  
  manualAlignSparesArm();
  delay(MANUAL_DELAY);
  
  manualAlignAuger();
  delay(MANUAL_DELAY);
  
  manualAlignDrill();//this may not be necessary, maybe just spin it a little bit for show?
  
  //delay(MANUAL_DELAY); //do we need to delay at the end?

  //TODO: Some other startup routine/LED flash/cool noise?
  return true;  //we finished alignment.
}

//TODO:  these 4 alignment methods.


//button 0: move core arm in.
//button 1: move core arm out.
//alignment occurs when hand has been moved to either it's outermost or innermost position (it does not need to remain there)
bool manualAlignCoreHand() {
  //so long as the third button isnt active
  while (alignmentButtonOff(alignmentButtonPin[2])) {
    //continuously try to adjust the motors, depending on which button is acte 
    // if both are active, motor will move +1, -1, +1, -1...
    coreExtractArmIn(alignmentButtonOn(alignmentButtonPin[0])); //TODO: auditory/visual(LED) error checking
    coreExtractArmOut(alignmentButtonOn(alignmentButtonPin[1])); //could use an if statement and make a sound if an error is occuring.
  }
  return true;
}

//button 0: move spares plate forward.
//button 1: move spares plate backward.
//sets ending position to 0.  TODO: what does this position mean?
bool manualAlignSparesPlate() {
  //loop while the third button isn't depressed.
  while (alignmentButtonOff(alignmentButtonPin[2])){
    //continuously try to adjust the motors, depending on which button is acte 
    // if both are active, motor will move +1, -1, +1, -1...
    sparesPlateForward(alignmentButtonOn(alignmentButtonPin[0]));
    sparesPlateBackward(alignmentButtonOn(alignmentButtonPin[1]));
  }
  setCurrentPlatePosition(0);
  return true;
}

//button 0: move auger up.
//button 1: move auger down.
//alignment occurs once the drill is fully retracted (it does not need to remain that way)
bool manualAlignAuger() {
//loop while the third button isn't depressed.
  while (alignmentButtonOff(alignmentButtonPin[2])){
    //continuously try to adjust the motors, depending on which button is acte 
    // if both are active, motor will move +1, -1, +1, -1...
    moveAugerUp(alignmentButtonOn(alignmentButtonPin[0]));
    moveAugerDown(alignmentButtonOn(alignmentButtonPin[1]));
  }
  return true;  
}

//button 0: rotate drill forward.
//button 1: rotate drill backwards.
// the drill is assumed to have 360 degrees of rotation when not in position 0.
bool manualAlignDrill() {
//loop while the third button isn't depressed.
  while (alignmentButtonOff(alignmentButtonPin[2])){
    //continuously try to adjust the motors, depending on which button is active.
    //if both are pressed, drill will move forward.
    if (alignmentButtonOn(alignmentButtonPin[0])){
      rotateDrillOneStep(MOTOR_FORWARD);
    } else if (alignmentButtonOn(alignmentButtonPin[1])){
      rotateDrillOneStep(!MOTOR_FORWARD);
    }
  }
  return true;  
}

//button 0: moves arm towards the body.
//button 1: moves arm away from the body.
// 
bool manualAlignSparesArm() { 
//loop while the third button isn't depressed.
  while (alignmentButtonOff(alignmentButtonPin[2])){
    //continuously try to adjust the motors, depending on which button is acte 
    // if both are active, motor will move +1, -1, +1, -1...
    sparesPlateArmForward(alignmentButtonOn(alignmentButtonPin[0]));
    sparesPlateArmAway(alignmentButtonOn(alignmentButtonPin[1]));
  }
  return true;  
}

//random debug method
bool alignmentButtonError(int pin){
  if(!alignmentButtonOn(pin)){ //if it's not on, we're expecting it to be off.
    return !alignmentButtonOff(pin); //so if it's off, it's not broken.
    
  } else if(!alignmentButtonOff(pin)){ //if it's not off, we're expecting it to be on.
    return !alignmentButtonOn(pin); //so if it's on, it's not broken.
  }

  //idk how it's both not off and not on, but if it's neither nor, it's really broken.
  return true; //it's really broken.
}
bool alignmentButtonOn(int pin) {
  if (digitalRead(pin) >= HIGH){
    return true;
  }
  return false;
}
bool alignmentButtonOff(int pin){
  if (digitalRead(pin) <= LOW){
    return true;
  }
  return false;
}

