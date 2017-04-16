



//rotates the drill and moves the auger up or down, depending on "forward".
// "stepsBeforeLateral" is how much the drill should rotate before the auger moves one step.
//  I recommend setting this value between 3 and 5.
//  returns true if auger movement was successful.
bool rotateDrillAndMoveAuger(bool forward, long stepsBeforeLateral){
  rotateDrill(forward, stepsBeforeLateral - 1); //rotates drill with delay to n-1 steps
  rotateDrillOneStepNoDelay(forward); // rotate the drill one more step without delay
  return moveAugerOneStep(forward); //and then move the auger and put in a delay.
}


//returns true if magnet was found.
//returns false if all motions did not result in a magnet find.
bool alignAugerWithMagnet(){
   //first attempt to move the auger up while spinning the drill.
   //if this begins to fail (we went too far up), then go the other way.
   //rememeber, forward is down.
   long positionBeforeBegin = 0;
   while(rotateDrillAndMoveAuger(MOTOR_FORWARD, 3)) {
    if(hallSensorActive()){ //we found the magnet!
      return true; //we're done!
    }
    positionBeforeBegin++; //track how far the auger moved
    }//end upwards movement

    //so, now we've reached the top and havent found the magnet. 
    //first of all, let's get the auger back to it's starting position.
    moveAugerDown(positionBeforeBegin);

    //now we need to do the rotate thing in the other direction.
    while(rotateDrillAndMoveAuger(!MOTOR_FORWARD, 3)) {
      if(hallSensorActive()){ //finally found it!
        return true; //we're done!
      }
    }

    //okay, so we reached both the top and the bottom and didn't find shit.
    //adjust either the magnetic sensor or the movement parameters
    //you idiot.
    return false;
   
  }
