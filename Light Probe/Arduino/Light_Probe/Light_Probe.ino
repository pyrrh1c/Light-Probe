//########################
// Includes and Variables
//########################
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

//LCD address is 0x37. It has 20 columns and 4 rows.
LiquidCrystal_I2C lcd(0x3f, 20, 4);
int lcdInterval = 0;

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
String ulValStr, urValStr, llValStr, lrValStr, hServCurPosStr,vServCurPosStr,ulOffsetStr,urOffsetStr,llOffsetStr,lrOffsetStr,hServoOffset,vServoOffset;

//Declare the string variable used to send serial data
String writeLine;
String readLine;

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

  // initialize the LCD
  lcd.begin();
  lcd.cursor();

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
  parseAndExecuteCommands(); // Checks for any pending serial data, checks for commands, and executes them.
  
  //Read the sensor values.
  //The C# input expects ranges between 0 and 100, so we map values accordingly
  ulValInt = map(analogRead(ulPin), 0, 1024, 0, 100);
  urValInt = map(analogRead(urPin), 0, 1024, 0, 100);
  llValInt = map(analogRead(llPin), 0, 1024, 0, 100);
  lrValInt = map(analogRead(lrPin), 0, 1024, 0, 100);

  hServCurPos = moveServo(hServo, hServCurPos, ulValInt, urValInt, hOffset); 
  vServCurPos = moveServo(vServo, vServCurPos, llValInt, lrValInt, vOffset);

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

//the parseAndExecuteCommands function
void parseAndExecuteCommands()
{
    //Load all serial data into the readLine variable.
  while (Serial.available()) {
    delay(25);  //delay to allow buffer to fill 
    if (Serial.available() >0) {
      char c = Serial.read();  //gets one byte from serial buffer
      readLine += c; //makes the string line
    }
  }

  //Check if the line has data.
  if (readLine != "")
  {
    //Check if the line has a command
    if(checkForCommand(readLine))
    {
      //Horizontal Servo Offset
      if (readLine.indexOf("INCREASE-HOFFSET") >= 0)
      {
        hOffset = hOffset + 1;
        //hServCurPos = moveServo(hServo, hServCurPos, ulValInt, urValInt, hOffset);
        Serial.print("Horizontal servo offset is now: ");
        Serial.println(hOffset);
        readLine = "";
      }
      else if (readLine.indexOf("DECREASE-HOFFSET") >= 0)
      {
        hOffset = hOffset - 1;
        //hServCurPos = moveServo(hServo, hServCurPos, ulValInt, urValInt, hOffset);
        Serial.print("Horizontal servo is now: ");
        Serial.println(hOffset);
        readLine = "";
      }

      //Vertical Servo Offset
      else if (readLine.indexOf("INCREASE-VOFFSET") >= 0)
      {
        vOffset = vOffset + 1;
        //vServCurPos = moveServo(vServo, vServCurPos, llValInt, lrValInt, vOffset);
        Serial.print("Vertical Servo Offset is now: ");
        Serial.println(vOffset);
        readLine = "";
      }
      else if (readLine.indexOf("DECREASE-VOFFSET") >= 0)
      {
        vOffset = vOffset - 1;
        //vServCurPos = moveServo(vServo, vServCurPos, llValInt, lrValInt, vOffset);
        Serial.print("Vertical servo offset is now: ");
        Serial.println(vOffset);
        readLine = "";
      }

      //Upper Left Sensor
      else if (readLine.indexOf("INCREASE-ULOFFSET") >= 0)
      {
        ulOffset = ulOffset + 1;
        Serial.print("Upper left sensor offset is now: ");
        Serial.println(ulOffset);
        readLine = "";
      }
      else if (readLine.indexOf("DECREASE-ULOFFSET") >= 0)
      {
        ulOffset = ulOffset - 1;
        Serial.print("Upper left sensor is now: ");
        Serial.println(ulOffset);
        readLine = "";
      }

      //Upper Right Sensor
      else if (readLine.indexOf("INCREASE-UROFFSET") >= 0)
      {
        urOffset = urOffset + 1;
        Serial.print("Upper right sensor offset is now: ");
        Serial.println(urOffset);
        readLine = "";
      }
      else if (readLine.indexOf("DECREASE-UROFFSET") >= 0)
      {
        urOffset = urOffset - 1;
        Serial.print("Upper right sensor is now: ");
        Serial.println(urOffset);
        readLine = "";
      }
      
      //Lower Left Sensor
      else if (readLine.indexOf("INCREASE-LLOFFSET") >= 0)
      {
        llOffset = llOffset + 1;
        Serial.print("Lower left sensor offset is now: ");
        Serial.println(llOffset);
        readLine = "";
      }
      else if (readLine.indexOf("DECREASE-LLOFFSET") >= 0)
      {
        llOffset = llOffset - 1;
        Serial.print("Lower left sensor is now: ");
        Serial.println(llOffset);
        readLine = "";
      }

      //Lower Right Sensor
      else if (readLine.indexOf("INCREASE-LROFFSET") >= 0)
      {
        lrOffset = lrOffset + 1;
        Serial.print("Lower right sensor offset is now: ");
        Serial.println(lrOffset);
        readLine = "";
      }
      else if (readLine.indexOf("DECREASE-LROFFSET") >= 0)
      {
        lrOffset = lrOffset - 1;
        Serial.print("Lower right sensor is now: ");
        Serial.println(lrOffset);
        readLine = "";
      }

      //Manual Override
      else if (readLine.indexOf("ENABLE-MANUALMODE") >= 0)
      {
        manualMode = true;
        Serial.println("Manual mode is: on");
        readLine = "";
      }
      else if (readLine.indexOf("DISABLE-MANUALMODE") >= 0)
      {
        manualMode = false;
        Serial.println("Manual mode is: off");
        readLine = "";
      }
      
      //Transmitting of Values over Serial Port
      else if (readLine.indexOf("ENABLE-TRANSMITVALUES") >= 0)
      {
        transmitValues = true;
        Serial.println("Transmission of values: on");
        readLine = "";
      }
      else if (readLine.indexOf("DISABLE-TRANSMITVALUES") >= 0)
      {
        transmitValues = false;
        Serial.println("Transmission of values: off");
        readLine = "";
      }      

      //Show all current calibration data
      else if (readLine.indexOf("SHOW-CALIBRATION") >= 0)
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
        readLine = "";
      }

      //Show the help menu
      else if (readLine.indexOf("SHOW-HELP") >= 0)
      {
        displayWelcomeMessage();
        readLine = "";
      }

      //Handle bad data
      else
      {
        transmitValues = false;
        //Serial.println("");
        //Serial.println("Transmission of values stopped!");
        //Serial.println("");
        displayWelcomeMessage();
        readLine = "";
      }
    }
    else
    {
      transmitValues = false;
      //Serial.println("");
      //Serial.println("Transmission of values stopped.!");
      //Serial.println("");
      displayWelcomeMessage();
      readLine = "";
    }
  } 
}

//the moveServo function
int moveServo(Servo s, int sPos, int val1, int val2, int offset)
{
  if ((val1 > val2) and ((sPos + 1) <= 135) and (abs(val1 - val2) > 10))
  {
    sPos = sPos + 1;
    s.write(sPos + offset);
    return(sPos);
  }
  else if ((val1 < val2) and ((sPos - 1)  >= 35) and (abs(val1 - val2) > 10))
  {
    sPos = sPos - 1;
    s.write(sPos + offset);
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
    s = " " + s;
  } while (s.length() < 3);
  return s;
}

void printNexLine()
{
  //Convert the sensor values and servo positions to padded numbers in string format
  ulValStr = ConvertAndPad(ulValInt + ulOffset);
  urValStr = ConvertAndPad(urValInt + urOffset);
  llValStr = ConvertAndPad(llValInt + llOffset);
  lrValStr = ConvertAndPad(lrValInt + lrOffset);
  hServCurPosStr = ConvertAndPad(map((hServCurPos + hOffset), 34, 136, 0, 100));
  vServCurPosStr = ConvertAndPad(map((vServCurPos + vOffset), 34, 136, 0, 100));
  ulOffsetStr = ConvertAndPad(ulOffset);
  urOffsetStr = ConvertAndPad(urOffset);
  llOffsetStr = ConvertAndPad(llOffset);
  lrOffsetStr = ConvertAndPad(lrOffset);
  hServoOffset = ConvertAndPad(hOffset);
  vServoOffset = ConvertAndPad(vOffset);
  
  //assemble all the values into a single line
  writeLine = "UL=" + ulValStr + ";";
    writeLine += "UR=" + urValStr + ";";
    writeLine += "LL=" + llValStr + ";";
    writeLine += "LR=" + lrValStr + ";";
    writeLine += "HSRV=" + hServCurPosStr + ";";
    writeLine += "VSRV=" + vServCurPosStr + ";";
    writeLine += "ULO=" + ulOffsetStr + ";";
    writeLine += "URO=" + urOffsetStr + ";";
    writeLine += "LLO=" + llOffsetStr + ";";
    writeLine += "LRO=" + lrOffsetStr + ";";
    writeLine += "HOFF=" + hServoOffset + ";";
    writeLine += "VOFF=" + vServoOffset + ";";
  
  //write the assembled message to the serial port
  Serial.println(writeLine);

  //May want to split this off into a function later...
  if (lcdInterval <=4)
  {
    lcdInterval ++;
  }
  else
  {
    lcdInterval = 0;
    lcd.clear();
    lcd.print(writeLine);
  }

}

bool checkForCommand(String testLine)
{
  if((testLine.indexOf("-")) and (testLine.indexOf(";") > 0))
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
  Serial.println("INCREASE-HOFFSET");
  Serial.println("DECREASE-HOFFSET");
  Serial.println("INCREASE-VOFFSET");
  Serial.println("DECREASE-VOFFSET");
  Serial.println("INCREASE-ULOFFSET");
  Serial.println("DECREASE-ULOFFSET");
  Serial.println("INCREASE-UROFFSET");
  Serial.println("DECREASE-UROFFSET");
  Serial.println("INCREASE-LLOFFSET");
  Serial.println("DECREASE-LLOFFSET");
  Serial.println("INCREASE-LROFFSET");
  Serial.println("DECREASE-LROFFSET");
  Serial.println("ENABLE-MANUALMODE");
  Serial.println("DISABLE-MANUALMODE");
  Serial.println("ENABLE-TRANSMITVALUES");
  Serial.println("DISABLE-TRANSMITVALUES");  
  Serial.println("SHOW-CALIBRATION");
  Serial.println("SHOW-HELP");
}
