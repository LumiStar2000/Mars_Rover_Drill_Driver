//----- HALL SENSOR GLOBALS -----
int  hallSensorPin  =  A0;     // select the input  pin for  the potentiometer 
int  sensorValue =  0;  // variable to  store  the value  coming  from  the sensor
int  threshold = 500;  //it appears to be at least 500 when active.
int  readNum = 0; //variable to store last read value.
//---- END HALL SENOSOR GLOBALS ----
// --------------------
// --- HALL METHODS ---
// ------ ^sensor -----
// --------------------
bool hallSensorActive()
{
  return (hallSensorRead() > threshold);
}
int hallSensorRead()
{
  return analogRead(hallSensorPin);
}



//Solenoids On/Off methods

//puts voltage low, which should differ from the power supply and turn them on.
bool activateSolenoids()
{
  if(!solenoid[0]){//if it's off...
    digitalWrite(solenoidPin[0], LOW);
    digitalWrite(solenoidPin[1], LOW);
  } else {return false;} // fail if solenoid was already activated

  //keep track
  solenoid[0] = true;
  solenoid[1] = true;

  return true; //solenoids are now on.
}

//puts voltage high, which should match the power supply and turn them off.
bool disableSolenoids()
{
  if(solenoid[0]){// if it's on...
    digitalWrite(solenoidPin[0], HIGH);
    digitalWrite(solenoidPin[1], HIGH);
  } else {return false;} // fail if solenoid was already activated

  //keep track
  solenoid[0] = false;
  solenoid[1] = false;

  return true; //solenoids are now off.
}

