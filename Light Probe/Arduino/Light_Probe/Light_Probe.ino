//Includes
#include <Servo.h>

//Declase the servo's
Servo hServo; 
Servo vServo; 

//Set the servo pins
int hServCurPos = 90;
int vServCurPos = 90;

//Set the photoresister pins
int ulPin = 0;
int urPin = 1;
int llPin = 2;
int lrPin = 3;

//declare the sensor values
int ulValInt;
int urValInt;
int llValInt;
int lrValInt;

//Declare the string variables of the sensor values and servo positions
//These are used for serial communication
String ulValStr;
String urValStr;
String llValStr;
String lrValStr;
String hServCurPosStr;
String vServCurPosStr;

//Declare the string variable used to send serial data
String nextLine;

void setup()
//the Setup() function runs once at arduino boot time
{
	//Start serial communication
	Serial.begin(9600);

	//Initialize the horizontal servo
	hServo.attach(9);
	hServo.write(hServCurPos);
	//Delay 1 second to allow the servo to center
	delay(1000);

	//Initialize the vertical servo
	vServo.attach(10);
	vServo.write(vServCurPos);
	delay(1000);
}
 
void loop()
//the loop() function runs in a loop forever
{
	//The C# input expects ranges between 0 and 100, so we map values
	ulValInt = map(analogRead(ulPin), 0, 1024, 0, 100);
	urValInt = map(analogRead(urPin), 0, 1024, 0, 100);
	llValInt = map(analogRead(llPin), 0, 1024, 0, 100);
	lrValInt = map(analogRead(lrPin), 0, 1024, 0, 100);
	hServCurPosStr = map(hServCurPos, 0, 180, 0, 100);
	vServCurPosStr = map(vServCurPos, 0, 180, 0, 100);

	//Convert the sensor values and servo positions to padded numbers in string format
	ulValStr = ConvertAndPad(ulValInt);
	urValStr = ConvertAndPad(urValInt);
	llValStr = ConvertAndPad(llValInt);
	lrValStr = ConvertAndPad(lrValInt);
	hServCurPosStr = ConvertAndPad(hServCurPos);
	vServCurPosStr = ConvertAndPad(vServCurPos);
  
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
  
	//Move the horizontal servo
  Serial.println(hServCurPos + CalculateNextMove(ulValInt, urValInt));
  hServCurPos = hServCurPos + CalculateNextMove(ulValInt, urValInt);
  if (CheckServoConstraints(hServCurPos))
  {
	  hServo.write(hServCurPos);
  }

	//Move the vertical servo
  Serial.println(vServCurPos + CalculateNextMove(llValInt, lrValInt));
  vServCurPos = vServCurPos + CalculateNextMove(llValInt, lrValInt);
	if (CheckServoConstraints(vServCurPos))
	{
	  vServo.write(vServCurPos + CalculateNextMove(llValInt, lrValInt));
	}

	//Pause execution for a moment so the servo can move
	delay(500);
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

int CalculateNextMove(int s1, int s2)
//Calculate how much and what direction the servo should move
//s1 represents either the left or lower servo, s2 is right or upper
{
	int multiplier; //Determines which way the servo will turn.
	int difference; //The difference between s1 and s2.
	int magnitude; //How much to move the servo.
	int result; //This is the final result and is returned.
  String temp;

	//Set the multiplier. -1 is counterclockwise, 0 is nothing, 1 is clockwise
	if (s1 < s2)
	{
		multiplier = 1;
	}	
	else if (s1 > s2)
	{
		multiplier = -1;
	}
	else
	{
		multiplier = 0;
	}


	//Calculate the difference between sensors.  This will be a positive integer.
	difference = s1 - s2;
	//Make the difference positive if it isn't already
	if(difference < 0)
	{
		difference = difference * -1;
	}


	//Calculate magnitude of the next move
	if (difference < 10)
	{
		magnitude = 2;
	} 
	else if ((difference >= 10) and (difference < 20))
	{
		magnitude = 5;
	}
	else if (difference >= 20)
	{
		magnitude = 10;
	}


	//Calculate the result.
	result = magnitude * multiplier;


	//Return the result
	return result;
} 

bool CheckServoConstraints(int i)
//Checks if a given angle is within what the servo can actually handle
//Returns true if the angle is between 45 and 135
{
  if ((i > 45) and (i < 135))
  {
    return true;
  }
  else
  {
    return false;
  }
}
