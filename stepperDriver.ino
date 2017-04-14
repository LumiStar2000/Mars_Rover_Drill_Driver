

//this performs a standard halfstep.
// you must delay at least 1ms before the next halfStep. delay(1); 
// This method does not provide the needed delay.
void halfStep(int motorNum){
  int Pin0 = (motorNum*4); //estabolish first pin of motor

  //makes its move, moves to next step.
  switch(_step[motorNum]){ 
   case 0: 
     digitalWrite(Pin0, LOW);  
     digitalWrite(Pin0+1, LOW); //are all this additions a slowdown?
     digitalWrite(Pin0+2, LOW); 
     digitalWrite(Pin0+3, HIGH); 
   break;  
   case 1: 
     digitalWrite(Pin0, LOW);  
     digitalWrite(Pin0+1, LOW); 
     digitalWrite(Pin0+2, HIGH); 
     digitalWrite(Pin0+3, HIGH); 
   break;  
   case 2: 
     digitalWrite(Pin0, LOW);  
     digitalWrite(Pin0+1, LOW); 
     digitalWrite(Pin0+2, HIGH); 
     digitalWrite(Pin0+3, LOW); 
   break;  
   case 3: 
     digitalWrite(Pin0, LOW);  
     digitalWrite(Pin0+1, HIGH); 
     digitalWrite(Pin0+2, HIGH); 
     digitalWrite(Pin0+3, LOW); 
   break;  
   case 4: 
     digitalWrite(Pin0, LOW);  
     digitalWrite(Pin0+1, HIGH); 
     digitalWrite(Pin0+2, LOW); 
     digitalWrite(Pin0+3, LOW); 
   break;  
   case 5: 
     digitalWrite(Pin0, HIGH);  
     digitalWrite(Pin0+1, HIGH); 
     digitalWrite(Pin0+2, LOW); 
     digitalWrite(Pin0+3, LOW); 
   break;  
     case 6: 
     digitalWrite(Pin0, HIGH);  
     digitalWrite(Pin0+1, LOW); 
     digitalWrite(Pin0+2, LOW); 
     digitalWrite(Pin0+3, LOW); 
   break;  
   case 7: 
     digitalWrite(Pin0, HIGH);  
     digitalWrite(Pin0+1, LOW); 
     digitalWrite(Pin0+2, LOW); 
     digitalWrite(Pin0+3, HIGH); 
   break;  
   default: 
     digitalWrite(Pin0, LOW);  
     digitalWrite(Pin0+1, LOW); 
     digitalWrite(Pin0+2, LOW); 
     digitalWrite(Pin0+3, LOW); 
   break;  
}
//move to next desired step
  if(dir[motorNum]){ //forward step
     _step[motorNum]++;
   }else{ //backward step
    _step[motorNum]--; 
   } 
   if(_step[motorNum]>7){ //too far forward 
     _step[motorNum]=0; 
   } 
   if(_step[motorNum]<0){ //too far backward
     _step[motorNum]=7; //this ruins backwards fullstep via %2 manip
   }                      //so fullstep is unusable, stick to halfstep.
}


//rotates all motors at same speed (halfStep)
void rotate(bool motors[], int steps)
{
  //increments through the steps
  for(int curStep = 0; curStep < steps; curStep++)
  {
    //increments through the motors
    for(int x = 0; x < numMotors; x++) //check each motor (possible slowdown)
    {
      if(motors[x]) //if the bool is true
      {
       halfStep(x); //increment by a half step.
      } //end increment positive
    }// end current motor check
    delay(1); //delay after all motors have been stepped once
  }// end current step
} //end synchronous rotation

//input: double motors[]: an array of numMotors size that 
//  depicts the ratio of each motors speed.  Please make 1 motor's speed 1.
//  to disable a motor, set it's speed to 0.
//  speeds not between 0 and 1 will result in unpredictable results
//input: int steps: number of steps for the motor at speed 1 to take.
//TODO: VERIFY IT WORKS!
void rotate(double motorSpeed[], int steps)
{
  double nextStep[numMotors]; //first motor step
  
  //copies array into one we can incremenet
  for (int x = 0; x < numMotors; x++)
  {
    nextStep[x] = (double)_step[x]; //sync step to counter
  }

  //increments through all the steps
  for (int curStep = 0; curStep < steps; curStep++)
  {
    //increments through each motor
    for(int curMotor = 0; curMotor < numMotors; curMotor++)
    {
      nextStep[curMotor] += motorSpeed[curMotor]; //increment its step by its current speed.
      
      if(nextStep[curMotor] > _step[curMotor]){ //ready for next step?
        halfStep(curMotor); //take the next step, but only if ready.
      }
    } //end MotorIncrement loop
  }//end StepIncrement unsync loop
}//end Unsync rotation.

