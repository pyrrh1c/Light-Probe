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

//the Setup() function runs once at arduino boot time
void setup()
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
 
//the loop() function runs in a loop forever
void loop()
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
  
	//move the horizontal servo
    if (ulValInt < urValInt)
    {
      if ((urValInt - ulValInt) > 25 )
      {
        if (hServCurPos < 135)
          {
            hServCurPos = hServCurPos + 5;
            hServo.write(hServCurPos);
          }
      }
    }
    if (ulValInt > urValInt)
    {
      if ((ulValInt - urValInt) > 25 )
      {
        if (hServCurPos > 45)
        {
          hServCurPos = hServCurPos - 5;
          hServo.write(hServCurPos);
        }
      }
    }

	//Move the vertical servo
    if (llValInt < lrValInt)
    {
      if ((lrValInt - llValInt) > 25 )
      {
        if (vServCurPos < 135)
          {
            vServCurPos = vServCurPos + 5;
            vServo.write(vServCurPos);
          }
      }
    }
    if (llValInt > lrValInt)
    {
      if ((llValInt - lrValInt) > 25 )
      {
        if (vServCurPos > 45)
        {
          vServCurPos = vServCurPos - 5;
          vServo.write(vServCurPos);
        }
      }
    }

	//Pause execution for a moment so the servo can move
	delay(50);
}

//this function takes an integer and produces a string with padded 0's
String ConvertAndPad (int i)
{
	String s = String(i);
	do
	{
		s = "0" + s;
	} while (s.length() < 3);
	return s;
}