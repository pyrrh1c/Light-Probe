//########################
// Includes and Variables
//########################

#include <Servo.h>

//Declase the servo's
Servo hServo, vServo; 

//Set the servo pins
int hServCurPos = 90, vServCurPos = 90;

//Set the photoresister pins
int ulPin = 0, urPin = 1, llPin = 2, lrPin = 3;

//declare the sensor values
int ulValInt, urValInt, llValInt, lrValInt;

//Declare the calibration variables (servo and sensor offsets)
int hOffset, vOffset, ulOffset, urOffset, llOffset, lrOffset;

//Declare the string variables of the sensor values and servo positions
//These are used for serial communication
String ulValStr, urValStr, llValStr, lrValStr, hServCurPosStr,vServCurPosStr;

//Declare the string variable used to send serial data
String nextLine;
String line;

//Declare the manual mode status variable
bool manualMode;
bool transmitValues = true;

//#######
// Setup
//#######

void setup() 
{
  //Start serial communication
  Serial.begin(9600);

  //Initialize the servos
  initializeServo(hServo, 9);
  initializeServo(vServo, 10);

  displayWelcomeMessage();
}

//###########
// Main Loop
//###########

void loop() 
{
  //Load all serial data into the line variable.
  while (Serial.available()) {
    delay(3);  //delay to allow buffer to fill 
    if (Serial.available() >0) {
      char c = Serial.read();  //gets one byte from serial buffer
      line += c; //makes the string line
    }
  }
  //Check if the line has data.
  if (line != "")
  {
    //Check if the line has a command
    if(checkForCommand(line))
    {
      //Horizontal Servo Offset
      if (line.indexOf("INCREASE-HOFFSET") >= 0)
      {
        hOffset = hOffset + 1;
        Serial.print("Horizontal servo offset is now: ");
        Serial.println(hOffset);
        line = "";
      }
      else if (line.indexOf("DECREASE-HOFFSET") >= 0)
      {
        hOffset = hOffset - 1;
        Serial.print("Horizontal servo is now: ");
        Serial.println(hOffset);
        line = "";
      }

      //Vertical Servo Offset
      else if (line.indexOf("INCREASE-VOFFSET") >= 0)
      {
        vOffset = vOffset + 1;
        Serial.print("Vertical Servo Offset is now: ");
        Serial.println(vOffset);
        line = "";
      }
      else if (line.indexOf("DECREASE-VOFFSET") >= 0)
      {
        vOffset = vOffset - 1;
        Serial.print("Vertical servo offset is now: ");
        Serial.println(vOffset);
        line = "";
      }

      //Upper Left Sensor
      else if (line.indexOf("INCREASE-ULOFFSET") >= 0)
      {
        ulOffset = ulOffset + 1;
        Serial.print("Upper left sensor offset is now: ");
        Serial.println(ulOffset);
        line = "";
      }
      else if (line.indexOf("DECREASE-ULOFFSET") >= 0)
      {
        ulOffset = ulOffset - 1;
        Serial.print("Upper left sensor is now: ");
        Serial.println(ulOffset);
        line = "";
      }

      //Upper Right Sensor
      else if (line.indexOf("INCREASE-UROFFSET") >= 0)
      {
        urOffset = urOffset + 1;
        Serial.print("Upper right sensor offset is now: ");
        Serial.println(urOffset);
        line = "";
      }
      else if (line.indexOf("DECREASE-UROFFSET") >= 0)
      {
        urOffset = urOffset - 1;
        Serial.print("Upper right sensor is now: ");
        Serial.println(urOffset);
        line = "";
      }
      
      //Lower Left Sensor
      else if (line.indexOf("INCREASE-LLOFFSET") >= 0)
      {
        llOffset = llOffset + 1;
        Serial.print("Lower left sensor offset is now: ");
        Serial.println(llOffset);
        line = "";
      }
      else if (line.indexOf("DECREASE-LLOFFSET") >= 0)
      {
        llOffset = llOffset - 1;
        Serial.print("Lower left sensor is now: ");
        Serial.println(llOffset);
        line = "";
      }

      //Lower Right Sensor
      else if (line.indexOf("INCREASE-LROFFSET") >= 0)
      {
        lrOffset = lrOffset + 1;
        Serial.print("Lower right sensor offset is now: ");
        Serial.println(lrOffset);
        line = "";
      }
      else if (line.indexOf("DECREASE-LROFFSET") >= 0)
      {
        lrOffset = lrOffset - 1;
        Serial.print("Lower right sensor is now: ");
        Serial.println(lrOffset);
        line = "";
      }

      //Manual Override
      else if (line.indexOf("ENABLE-MANUALMODE") >= 0)
      {
        manualMode = true;
        Serial.println("Manual mode is: on");
        line = "";
      }
      else if (line.indexOf("DISABLE-MANUALMODE") >= 0)
      {
        manualMode = false;
        Serial.println("Manual mode is: off");
        line = "";
      }
      
      //Transmitting of Values over Serial Port
      else if (line.indexOf("ENABLE-TRANSMITVALUES") >= 0)
      {
        transmitValues = true;
        Serial.println("Transmission of values: on");
        line = "";
      }
      else if (line.indexOf("DISABLE-TRANSMITVALUES") >= 0)
      {
        transmitValues = false;
        Serial.println("Transmission of values: off");
        line = "";
      }      

      //Show all current calibration data
      else if (line.indexOf("SHOW-CALIBRATION") >= 0)
      {
        Serial.println("Current calibration data");
        Serial.print("hOffset is: ");
        Serial.println(hOffset);
        Serial.print("vOffset is: ");
        Serial.println(vOffset);
        Serial.print("ulOffset is: ");
        Serial.println(ulOffset);
        Serial.print("urOffset is: ");
        Serial.println(urOffset);
        Serial.print("llOffset is: ");
        Serial.println(llOffset);
        Serial.print("lrOffset is: ");
        Serial.println(lrOffset);
        Serial.print("Manual mode: ");
        if (manualMode == false)
        {
          Serial.println("off");
        } else {
          Serial.println("on");
        }
        line = "";
      }

      //Show the help menu
      else if (line.indexOf("SHOW-HELP") >= 0)
      {
        displayWelcomeMessage();
        line = "";
      }

      //Handle bad data
      else
      {
        Serial.println("An invalid command received."); 
        line = "";
      }
    }
    else
    {
      Serial.println("Invalid serial data received.");
      line = "";
    }
  } 
  
  //Read the sensor values.
  //The C# input expects ranges between 0 and 100, so we map values accordingly
  ulValInt = map(analogRead(ulPin), 0, 1024, 0, 100);
  urValInt = map(analogRead(urPin), 0, 1024, 0, 100);
  llValInt = map(analogRead(llPin), 0, 1024, 0, 100);
  lrValInt = map(analogRead(lrPin), 0, 1024, 0, 100);

  hServCurPos = moveServo(hServo, hServCurPos, ulValInt, urValInt); 
  vServCurPos = moveServo(vServo, vServCurPos, llValInt, lrValInt);

  hServCurPosStr = map(hServCurPos, 0, 180, 0, 100);
  vServCurPosStr = map(vServCurPos, 0, 180, 0, 100);

  if (transmitValues == true)
  {
      printNexLine();
  }

  //Pause execution for a moment so the servo can move
  delay(100);
}

//##################################
// Function Declarations Begin Here
//##################################

int moveServo(Servo s, int sPos, int val1, int val2)
{
  if ((val1 > val2) and ((sPos + 1) <= 135) and (abs(val1 - val2) > 10))
  {
    sPos = sPos + 1;
    s.write(sPos);
    return(sPos);
  }
  else if ((val1 < val2) and ((sPos - 1)  >= 35) and (abs(val1 - val2) > 10))
  {
    sPos = sPos - 1;
    s.write(sPos);
    return(sPos);
  }
  else
  {
    return sPos;
  }
}

void initializeServo(Servo s, int pin)
{
  //Initialize the vertical servo
  s.attach(pin);
  s.write(90);
  delay(1000);
}

String ConvertAndPad (int i)
//this function takes an integer and produces a string with padded 0's
{
  String s = String(i);
  do
  {
    s = "0" + s;
  } while (s.length() < 3);
  return s;
}

void printNexLine()
{
  //Convert the sensor values and servo positions to padded numbers in string format
  ulValStr = ConvertAndPad(ulValInt);
  urValStr = ConvertAndPad(urValInt);
  llValStr = ConvertAndPad(llValInt);
  lrValStr = ConvertAndPad(lrValInt);
  hServCurPosStr = ConvertAndPad(map(hServCurPos, 34, 136, 0, 100));
  vServCurPosStr = ConvertAndPad(map(vServCurPos, 34, 136, 0, 100));
  
  //assemble all the values into a single line
  nextLine = 
    "UL=" + ulValStr + ";" +
    "UR=" + urValStr + ";" +
    "LL=" + llValStr + ";" +
    "LR=" + lrValStr + ";" +
    "hSrv=" + hServCurPosStr + ";" +
    "lSrv=" + vServCurPosStr + ";";
  
  //write the assembled message to the serial port
  Serial.println(nextLine);
}

bool checkForCommand(String line)
{
  if((line.indexOf("-")) and (line.indexOf(";") > 0))
  {
    return true;
  }
  else
  {
    return false;
  }
}

void displayWelcomeMessage()
{
  Serial.println("Welcome to Light-Probe 1.0");
  Serial.println("==========================");
  Serial.println("The following commands are avaialable:");
  Serial.println("INCREASE-HOFFFSET");
  Serial.println("DECREASE-HOFFFSET");
  Serial.println("INCREASE-VOFFFSET");
  Serial.println("DECREASE-VOFFFSET");
  Serial.println("INCREASE-ULOFFFSET");
  Serial.println("DECREASE-ULOFFFSET");
  Serial.println("INCREASE-UROFFFSET");
  Serial.println("DECREASE-UROFFFSET");
  Serial.println("INCREASE-LLOFFFSET");
  Serial.println("DECREASE-LLOFFFSET");
  Serial.println("INCREASE-LROFFFSET");
  Serial.println("DECREASE-LROFFFSET");
  Serial.println("ENABLE-MANUALMODE");
  Serial.println("DISABLE-MANUALMODE");
  Serial.println("ENABLE-TRANSMITVALUES");
  Serial.println("DISABLE-TRANSMITVALUES");  
  Serial.println("SHOW-CALIBRATION");
  Serial.println("SHOW-HELP");
}