//Reference guide for drillDriver_v0.2b
//  Replaces the global declare file.

//Updated 4/13/2017.
//Created 4/12/2017.
//Author: Todd Oakes.    Samples provided by Elegoo.  
//Credits in section X.

// See section 06 for pinout and used pins.
// Unused pins: 20, 21, 24, 25, 26, 27, 31->,  A1->


//REQIREMENTS:
//  Arduino MEGA or other Arduino authentic/clone with:
//    at least 8K of program space and 2K of dynamic memory.
//    at least 30 usable digital pins, and 1 analog pin.
//  5x 4-pin(data) stepper motors (set stepsPerRotation for your motor)
//  2x 2-pin solenoids.
//  3x 2-pin switches (manual buttons for alignment)
//  1x magnetic sensor (configure globals for your sensor's tolerances.  May need to adjust methods too)



// ----------- TABLE OF CONTENTS -----------
//  00. CHANGELOG
//       Program development history, urgent TODO's, 
//        progress map, and other important comments.

//  01. GLOBALS
//       All globals.  USE CAUTION.

//  02. METHODS
//       All methods.  USE CAUTION.

//  03. ALIGNMENT_STEPS
//       Full pinout/alignment process with notes.

//  04. POWER_SETUP
//       Power setup should be last, 
//         after you've read the alignment steps fully,
//         and setup the data/motor/button pins.

//   X. THANK_YOU
//       Special thanks to all those involved,
//        including other team members, contributors, and suppliers.








// ----------- Sections: -----------
// 00. CHANGELOG

// UPDATE 4/18 : Continued work on final motions (animation finally posted)
//  still need extact distances/lengths of: [LIST OF ALL PARTS]
//  Updated TOC references to reflect removal of
//   specific sections that had been deleted 
//   due to conflicts with Github and readme.h integration (this is the main header)

// UPDATE 4/13:  Began work on final motions, 
//  but their dependencies (length data) is implemented as guesswork.
//  Make sure to run other tests and fully calculate their values before running their methods.
//  AUGER_LENGTH, AUGER_REVOLUTIONS_TO_EXTRACTION, etc.

// UPDATE 4/12:  DOCUMENTATION FILE!!!  Ready to be tested!
// See methodsAndGlobals.ino for full documentation. (that's this file)
// See CURRENT GOALS: section for most urgent TODO's.
// All movements are coded for Auger Vertical, Auger Drill, Core Extraction Arm.
//  Basic movements coded for Core Plate Arm and its rotating plate.
// Manual alignment is fully coded and is ready to be used for a test.
// current linecount:  Documentation: 457.  Code: 975.

// UPDATE 4/07:  Almost there!!!
// Current Line Count: 730.  All of the motor drivers are done.
// TODO:
//  1. Alignment on startup.  Align for demonstration start.
//  2. Run demonstration.  Pause.


// CURRENT GOALS:
// April 13th, 2017:
//  1. Run an alignment test.
//  2. Verify alignment results.
//    2a. Perform output (serial or LED) to verify values after alignment.
//    2b. Run a sample demonstration, one simple enough to verify movements without destroying device.
//     2bn: Maybe provide an escape method via button input if things go wrong?
//  3. Decide exactly how to demonstrate the device on 4/14 meeting (rough idea in comments)
//  4. Run said demonstration test.

// PROGRESS MAP:
// Progress Apr 17: Began work on final code.  v0.1b was completed (but not verified) on 4/12.
//  Began Changelog on final code, updated to v0.2b once all basic methods were completed.

// Progress Mar 17: Ran tests on various sensors, decided on using stepper's programmability for alignment.  
//  Contributed to design of drill, provided input on feasability for automated test model.
//  Helped minimize moving parts and utilize motor movements over mechanical tricks.
//  Spring break.
//  Verified drill's moving parts and expected movements.


// Progress Feb 17: Ran tests on a few basic stepper motors, finished stepperDriver.
//  Used static Pinning, halfStep() verified, fullStep() abandoned, delay(1) found to be minimum.
//  Tested various types of motors, ultimately decided to use stepper motors for all movement (for demonstration)
//  Dynamic pinning verified.  Verified sensor, completed interface drivers for motors and sensors. 

// Progress Jan 17: Did research into arduino/pi, research on motor types and programming ease.
//  Ultimately decided on using Arduino due to familiarity and ease of use for robotics.


//Motor notes:
// The minimum delay per half step is 1ms. The minimum delay per full step is 2ms.
// If the amount of instructions between steps exceeds 1ms, then it might be worthwhile
//  to write a working fullstep method.  But, be warned, fullsteps run at half torque.
//  it would be a better idea to eliminate delays first.
//  Apr17: I deleted the fullStep() method because it reduced torque and speed.
//  Feb17: Looked at Servo Motors, Stepper Motors, and traditional motors.  Opted for stepper motors for all applications 
//           due to versatility, ease of use, and accurate speed/position controls.
//           Also, budget.
//  Jan17: Simple motors are hard to control on a pi.  Eliminated pi.


// < N O T E   T O (FUTURE) C O D E R S > 
//  If you make any change to final program
//     YOU MUST Update the changelog, and expand credits if there's new credits.
//     Update documented methoods (PLEASE!).  Failure results in duplicate methods!
//     Violation will result in banning from the project (or at least coding it).
//     Exceptional contributors will be rewarded with extra (peanut butter) cookies.
//</ N O T E   T O (FUTURE) C O D E R S > 




// 01. GLOBALS:

//-- Main --
//  Alignment Buttons Pins 28,29,30
//  Talking points:  Efficiency of the code, use of const for optimization.
const int alignmentButtonPin[] = {28,29,30};
//  Solenoid Pins 22, 23
const int solenoidPin[] = {22, 23};
bool solenoid[] = {false, false}; 
//  Motor Pins on 0-19.
const int numMotors = 5; 
const long stepsPerRevolution = 4096;
int _step[] = {0,0,0,0,0}; 
const bool MOTOR_FORWARD = false; //forward is up.
bool dir[] = {MOTOR_FORWARD,MOTOR_FORWARD,MOTOR_FORWARD,MOTOR_FORWARD,MOTOR_FORWARD}; 
//alignWithMagnet
const double AUGER_PITCH_HEIGHT = 0.125; //pitch dia in inches
const double AUGER_TOTAL_HEIGHT = 10; //height in inches
const double AUGER_LENGTH = AUGER_TOTAL_HEIGHT / AUGER_PITCH_HEIGHT; //number of revolutions (maximum)
const long AUGER_INITIAL_CLEANING_POSITION = AUGER_LENGTH * 0.5;  //number of revolutions to the cleaning position (from top)
const long AUGER_DISTANCE_PER_REVOLUTION = 3; //pitch diameter in inches(? TODO check calculations)
const long AUGER_CLEANING_POSITION = AUGER_INITIAL_CLEANING_POSITION * AUGER_DISTANCE_PER_REVOLUTION * stepsPerRevolution;
//coreExtractor
const int CORE_ARM_MOTOR = 4; // pins 16-19
const double CORE_EXTRACT_PITCH_DIAMETER = 0.417; //inches
const double CORE_EXTRACT_ARM_WORM_GEAR_LENGTH = 10.000; //inches TODO change to a real measurement.
const double CORE_EXTRACT_DISTANCE_PER_ROTATION = CORE_EXTRACT_PITCH_DIAMETER * PI;//distance covered by one full rotation.
const double CORE_EXTRACT_ARM_TOTAL_REVOLUTIONS = CORE_EXTRACT_ARM_WORM_GEAR_LENGTH / CORE_EXTRACT_PITCH_DIAMETER;
const long CORE_EXTRACT_ARM_LENGTH = (long)(CORE_EXTRACT_ARM_TOTAL_REVOLUTIONS * stepsPerRevolution * CORE_EXTRACT_DISTANCE_PER_ROTATION);
long coreExtractArmPosition = 0; //0 = extract position.  positive means it's moving away from the auger
bool coreArmEndstop = false;
//drillRotation
const int DRILL_MOTOR = 1; //pins 4-7
const int SPARES_ANGULAR_POSITION = 0;
const int ANGULAR_POSITION_TOLERANCE = 5;
int drillPosition = 0;
//magneticDriver
int  hallSensorPin  =  A0;   
int  sensorValue =  0; 
int  threshold = 500;  
int  readNum = 0; 
//manualAlignment
const int MANUAL_DELAY = 500;
//moveAuger
const int AUGER_MOTOR = 0; //motor on pins 0-3.
const long MAX_REVOLUTIONS = AUGER_LENGTH;
const long MAX_AUGER_STEPS = (stepsPerRevolution * MAX_REVOLUTIONS);// 25 full turns.
const double AUGER_REVOLUTIONS_TO_EXTRACTION = AUGER_LENGTH * 0.2 ; //number of revolutions to the extraction position. (from top)
const long AUGER_EXTRACTION_POSITION = (long)(stepsPerRevolution * AUGER_REVOLUTIONS_TO_EXTRACTION); //steps to extraction position.
bool augerEndstop = false;
long stepsSinceEndstop = 0;
long theSurface = (long)(AUGER_LENGTH * stepsPerRevolution * 0.8 ); //number of steps to the surface. 
//restoreDrill
const int lateralStepsPerStep = 7; //tan(1/7) = 8.13 degrees.
const bool moveDrillDownwards = true;
const long MAX_CLEANING_ROTATIONS = 10; // 10 rotations max. 
const long MAX_CLEANING_STEPS = stepsPerRevolution * MAX_CLEANING_ROTATIONS / (long)lateralStepsPerStep;
long AUGER_CLEANING_HEIGHT = stepsPerRevolution * AUGER_LENGTH /2;
long AUGER_CLEANING_ROTATION = 0;
//sparesPlateArm
int sparesArmPosition = 0; //start it at 0.  forward is ++
const int MAXIMUM_ARM_ROTATION = stepsPerRevolution / 3; // 360/3 degrees, into a slice of pie.
const int CORE_PLATE_ARM_MOTOR = 2; //pins 8-11
//sparesPlateRotation
int currentPosition = 0;
const int CORE_PLATE_MOTOR = 3; //pins 12-15












// 02. METHODS:
//-- Main --
void setup(); //init pins.
void loop();
//alignWithMagnet
bool rotateDrillAndMoveAuger(bool forward, long stepsBeforeLateral); 
bool alignAugerWithMagnet();
//coreExtractor
void coreExtractArmIn(bool buttonPressed);
void coreExtractArmOut(bool buttonPressed);
bool coreExtractArmIn();
bool coreExtractArmOut();
bool moveExtractionArmOneStep(bool forward);
bool moveExtractionArmOneStepNoDelay(bool forward);
bool coreArmAtExtractionPoint();
bool coreArmNotAtMaxDistance();
//drillRotation
bool rotateDrillOneStepNoDelay(bool forward);
bool rotateDrillOneStep(bool forward);
bool rotateDrill(bool forward, long steps);
bool rotateDrillForwardToAlignment(int pos);
int getCurrentAlignment();
bool rotateDrillForward(bool buttonPressed);
bool rotateDrillBackward(bool buttonPressed);
bool readyForSpare();
//magneticDriver
bool hallSensorActive();
int hallSensorRead();
bool activateSolenoids();
bool disableSolenoids();
//manualAlignment
bool manualAlignment(); //all of these methods expect buttons to be hooked up.
bool manualAlignCoreHand();
bool manualAlignSparesPlate();
bool manualAlignAuger();
bool manualAlignDrill();
bool manualAlignSparesArm();
bool alignmentButtonError(int pin);
bool alignmentButtonOn(int pin);
bool alignmentButtonOff(int pin);
//moveAuger
bool moveAugerUp();
bool moveAugerUp(long steps);
bool moveAugerDown(long steps);
bool moveAugerOneStepNoDelay(bool forward);
bool moveAugerOneStep(bool forward);
void moveAugerUp(bool buttonPressed);
void moveAugerDown(bool buttonPressed);
void setTheSurface();
long getAugerCurrentPosition();
bool moveAugerToExtractionPosition();
bool moveAugerToTheSurface();
bool augerAtTop();
//restoreDrill
bool cleanDrill ();
bool cleaningMovement();
bool moveDrillIntoCleaningPosition();
bool moveDrillAndAugerOneStep();
//sparesPlateArm
bool sparesPlateArmOneStepNoDelay(bool forward);
bool sparesPlateArmOneStep(bool forward);
void sparesPlateArmForward(bool buttonPressed);
void sparesPlateArmAway(bool buttonPressed);
//sparesPlateRotation
bool sparesPlateRotateOneStepNoDelay(bool forward);
bool sparesPlateRotateOneStep(bool forward);
void sparesPlateForward(bool buttonPressed);
void sparesPlateBackward(bool buttonPressed);
void setCurrentPlatePosition(int pos); 
int getCurrentPlatePosition();
//stepperDriver
void halfStep(int motorNum); 
void rotate(bool motors[], int steps);
void rotate(double motorSpeed[], int steps);









// 03. ALIGNMENT_STEPS

// -- PIN SETUP: --
//  0 - 3 : AUGER_MOTOR
//  4 - 7 : DRILL_MOTOR
//  8 - 11: CORE_PLATE_ARM_MOTOR
// 12 - 15: CORE_PLATE_MOTOR
// 16 - 19: CORE_ARM_MOTOR (extraction motor)
// 22 - 23: Solenoids 0 and 1.
// 28 - 30: Alignment Buttons.
//    A0  : Hall Sensor (magnetic)

//-- ALIGNMENT PROCESS: --
//  NOTE: The three buttons are labelled 0,1,2.
//  NOTE: Button 0 moves up/in/towards/forward
//  NOTE: Button 1 moves down/out/away/backward
//  NOTE: Button 2 advances the step (configure next motor)

//IF ANY STEP MOVES THE WRONG DIRECTION:
//  Reverse the individual OneStepNoDelay methods MOTOR_FORWARD for !MOTOR_FORWARD
//   and vice versa.  Otherwise, you risk catastrophic failure. 

//IF ANY STEP FAILS OTHERWISE: 
//   Check "If it fails:", check individual methods.

//Step 1: Core Hand 
//  b0: Moves the core arm in.
//  b1: Moves the core arm out.
//   Suggested Process: Move arm to it's fully exterior position.
//   Minimum Requirements: Arm must hit either it's maximum or minimum position. 
//   If it fails:  Adjust CORE_EXTRACT_ARM_LENGTH or it's dependencies.  Check position logs.
//Step 2: Spares Plate
//  b0: Moves the spares plate forward
//  b1: Moves the spares plate backward
//    Suggested Process: Move plate into a significant position.
//    Minimum Requirements:  Plate must be able to move away from the drill 
//                             via arm movement in the next step.
//    If it fails:   Check where you reset the distance, check position logs.
//Step 3: Spares Arm
//  b0: Moves arm towards the body.
//  b1: Moves arm away from the body.
//    Suggested Process: Move plate fully away from the body.
//    Minimum Requirements: Plate must hit either it's maximum or minimum position.
//    If it fails:  Adjust MAXIMUM_ARM_ROTATION, check position logs.
//Step 4: Auger Height
//  b0: Move auger up
//  b1: Move auger down.
//    Suggested Process: Retract drill fully, then move it back out a little bit.
//    Minimum Requirements: The upmost position it reaches is assumed to be the top.
//    If it fails:  Check upmost position reset.  Check position logs.
//Step 5: Rotate the drill a little bit for your pleasure.
//  b0: Rotates drill forward.
//  b1: Rotates drill backward.
//    Suggested Process: Ensure the drill is rotating in the correct direction.
//    Minimum Requirements: None.
//    If it fails:  Check basic motor driver.  Check motor power.  Check for obstructions.
//                    No seriously, this really shouldn't fail unless the drill is fully retracted.














// 04. POWER_SETUP

// URGENT: Read section 3 before attempting any of these connections.
// -- PIN SETUP: --
//  0 - 3 : AUGER_MOTOR DATA
//  4 - 7 : DRILL_MOTOR DATA
//  8 - 11: CORE_PLATE_ARM_MOTOR DATA
// 12 - 15: CORE_PLATE_MOTOR DATA
// 16 - 19: CORE_ARM_MOTOR DATA
// 22 - 23: Solenoids 0 and 1. (negative pin)
// 28 - 30: Alignment Buttons (negative pin)
//    A0  : Hall Sensor (magnetic)

// URGENT: Read section 3 before attempting any of these connections.
// -- POWER PINS: --
//  +- -> AUGER_MOTOR
//  +- -> DRILL_MOTOR
//  +- -> CORE_PLATE_ARM_MOTOR
//  +- -> CORE_PLATE_MOTOR
//  +- -> CORE_ARM_MOTOR
//  +  -> Solenoids 0 and 1. (circuit might need a resistor)
//  +  -> Alignment Buttons (circuit might need a resistor)
//  +- -> Hall Sensor Power (maybe just hook this up to arduino?)
//  +- -> V+ V- on PS











//  X. THANK_YOU

// NOTE FROM THE AUTHOR:
// I, the author, Todd Oakes, would like to thank you for taking the time to read this.  
//  So many people skip the credits, me included.  You're really a special person.
//  I'd like to reward you a batch of my perfected peanut butter cookies. 
//  I've consulted with two chefs and spent 18 months perfecting theese cookies.
//  You want some.  Just let me know, I'm looking for any excuse to make more.


// --- CREDITS: ---

// AUTHOR: Todd Oakes, Spring, TX, USA.

// TEAM: LoneStar Cyclone Drilling, Lone Star College - CyFair, Cypress, TX.
//       Texas Space Grant Consortium Design Challenge
// TOPIC:  Mars Drill Design and Automation
//         Automated Drill Auger Cleaning and Core Extraction System.
// TEAM LEADER: Christopher Geisheker
// TEAM MEMBERS: Bryce Reynolds, Andy Bui, Dejuan Miller, Alexander Holcombe, Todd Oakes.
// TEAM SPONSORS: Dr. Yiheng Wang, Jared A. Cammon; Lone Star College Department of Engineering.
// TEAM MENTOR: Dr Humboldt C. Mandell, UT Center for Space Research.

// Special thanks to:
//   Chris Sides and Forrest Davis for providing help when I was completely lost on how to program an arduino/pi.
//   Github for making code collaboration and sharing easy and convenient.
//   Arduino.cc for making robotics easy and fun.

// ADDITIONAL CREDIT: Elegoo, Shenzhen, Guangdong, China for providing simple, affordable electronics and sample code.
//                      Elegoo turned this headache of single-person robotics challenge into a cakewalk.
//                      Thank you so much for giving me direction when I was lost.
//                    Adafruit Industries, Lower Manhattan, New York City, NY for providing
//                      the solenoids and the components to make them work.
//                    Professor Lucio Florez, Lone Star College - University Park, Houston, TX 
//                      for renewing my interest and expanding my knowledge about mechanical and electrical engineering.
//                      And for expanding my knowledge quickly enough to make this project a reality!
//                    Dr. Nolides Guzman, Lone Star College - University Park, Houston, TX
//                      for making engineering fun, and introducing me to the world of 3D design and 3D printing.
//                      And special thanks for providing extra academic help.

// Personal Appeal:  If you found this code fairly well organized, 
//                    and you're looking for someone to help you program or design robots, 
//                    I'm a mechanical engineering major, and I'd be happy to  
//                    send a copy of my resume to you.
//                    My email is [firstName].[lastName]@gmail.com, 
//                    where [firstName] and [lastName] are my name (Todd Oakes).



