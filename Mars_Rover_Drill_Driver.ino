// See readme.ino for Full Documentation and credits.
#include "readme.h"
//     Mars Rover Drill Driver 
//          Version: v0.2b
//      Last edited: 04/12/17
//    Last verified: 03/31/17  
//           Author: Todd Oakes

//          Purpose: TSGC Design Team Challenge
//     Organization: Lone Star College - CyFair
//Presentation Date: April 23, 2017.


//DESCRIPTION:
//  Drives a custom drill.  Design specs can be found at [insert site here].
//  Full documentation and credits can be found in readme.txt.


//REQIREMENTS:
//  See documentation file.

//CHANGELOG:
//  See documentation file.

//DOCUMENTATION, SETUP, PINOUT:
//  See documentation file.








//Note: You don't have to run all 5 motors.  If you call false/0 for a motor, it won't activate during that step.
//  However, the pins will remain off.  Therefore,
//  Pins 0-19 are reserved for the motors.
//  Pins 20-43 are open for other functions.



void setup() {
  // Initialize your pins here.

  //--- DIGITAL PINS ---
  // motors
  for(int x = 0; x < numMotors; x++)
  {
    //start pin = currentMotor*4.
    //end pin = currentMotor*4 + 3;
    for(int pin = (x*4); pin < ((x*4)+4); pin++) // 4 pins per motor
    {
      pinMode(pin, OUTPUT); //initialize pins for output to motors
    }
  }
  
  // solenoids
  for(int x = 0; x < 2; x++)
  {
    pinMode(solenoidPin[x], OUTPUT);
  }
  disableSolenoids();
  // button pins
  for(int x = 0; x < 3; x++){
    pinMode(alignmentButtonPin[x], INPUT);
  }
  // --- END DIGITAL PINS ---
  
  //Analog pins don't need initialization.
  //opening analog communication port
  Serial.begin(9600); //open sensor baud 9600 (why this value?)

  
}

  //IDEA: Start the program with an "alignment phase", where I press 2 buttons to align the motors.
  //      Once the motor is aligned, hit both buttons at once to advance (or use a third button).
  //      Once all motors are aligned, begin "demonstration loop", but allow for the buttons to halt and repeat.

//TODO: For testing, comment out all but 1 action (except alignment)
void loop() {

// --- ALL METHODS MUST RETURN A BOOL OR INT ----
// --- THIS IS ESSENTIAL TO ERROR DETECTION -----
  //TODO 1. restore all objects to known positions
   manualAlignment();
  
  //TODO 2. Extend auger to magnetic alignment
  alignAugerWithMagnet();
  
  //TODO 3. Move drill down to surface. 
  moveAugerToTheSurface();

  //TODO 4. Do a drilling motion, retrieve drill.
  long numberOfDrillingRevolutions = 8;
  rotateDrill(MOTOR_FORWARD, stepsPerRevolution * numberOfDrillingRevolutions); //
  //TODO any other steps after drilling (backing out maybe?)
  
  //TODO 5. (need specifics) Remove core sample.
  //   5.1. Move grabber in, move auger up significantly, move grabber out, dump sample, restore positions.
  moveAugerToExtractionPosition();
  coreExtractArmIn();
  //TODO finish extraction steps

  //TODO 6. Move auger to cleaning position
  //   6.1. Activate solenoids, rotate drill SLOWLY.   
  cleanDrill();
  
  //TODO 7. Write Debug methods that uses Serial.println() to output useful debug info.
  //         Include things like method calls, variables, positions.
  
  
  
  
}
  

//bool restoreDrill(){} //move core hand out, deactivate solenoids, move auger to the top, then align with magnet. 


  //maybe rotate auger 2pi then move it a couple MM up.  idea was voted down by group
  //maybe rotate auger and move it slowly, covering the same area in a cool spiral looking fashion.
  //  2nd idea was voted up by group
//bool moveAugerDown(double holeDepth){} //calculates steps then runs below method

//bool drill(double drillDistance){} //drills into the ground. 
  //After the drilling process, we will not have an accurate read on the auger's current rotational alignment.
  // We will need to use magnetic alignment.
//bool cleanAuger(){} // move drill into cleaning position, activate solenoids, rotate SLOWLY in a pre-programmed fashion.
// and by slowly I mean damn slow, let's use delay(10) to start (2 < x < 15) 
// bool lockCleaningBlade() {} // locks cleaning blade into place
// bool unlockCleaningBlade() {} //unlocks cleaning blade



