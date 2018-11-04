#include <Servo.h>

Servo hServo; 
Servo vServo; 

int hServCurPos = 90;
int vServCurPos = 90;

int ulPin = 0;
int urPin = 1;
int llPin = 2;
int lrPin = 3;
int ulValInt;
int urValInt;
int llValInt;
int lrValInt;
String ulValStr;
String urValStr;
String llValStr;
String lrValStr;
String hServCurPosStr;
String vServCurPosStr;
String nextLine;


void setup()
{
 Serial.begin(9600);
 hServo.attach(9);
 hServo.write(hServCurPos);
 delay(1000);
 vServo.attach(10);
 vServo.write(vServCurPos);
 delay(1000);
}
 
void loop()
{
  ulValInt = map(analogRead(ulPin), 0, 1024, 0, 100);
  urValInt = map(analogRead(urPin), 0, 1024, 0, 100);
  llValInt = map(analogRead(llPin), 0, 1024, 0, 100);
  lrValInt = map(analogRead(lrPin), 0, 1024, 0, 100);
  hServCurPosStr = map(hServCurPos, 0, 180, 0, 100);
  vServCurPosStr = map(vServCurPos, 0, 180, 0, 100);

  ulValStr = String(ulValInt);
  do {
    ulValStr = "0" + ulValStr;
  } while (ulValStr.length() < 3);
  
  urValStr = String(urValInt);
    do {
    urValStr = "0" + urValStr;
  } while (urValStr.length() < 3);
  
  llValStr = String(llValInt);
    do {
    llValStr = "0" + llValStr;
  } while (llValStr.length() < 3);
  
  lrValStr = String(lrValInt);
    do {
    lrValStr = "0" + lrValStr;
  } while (lrValStr.length() < 3);

  hServCurPosStr = String(hServCurPosStr);
  do {
    hServCurPosStr = "0" + hServCurPosStr;
  } while (hServCurPosStr.length() < 3);

  vServCurPosStr = String(vServCurPosStr);
  do {
    vServCurPosStr = "0" + vServCurPosStr;
  } while (vServCurPosStr.length() < 3);
  
  nextLine = 
    "UL=" + ulValStr + ";" +
    "UR=" + urValStr + ";" +
    "LL=" + llValStr + ";" +
    "LR=" + lrValStr + ";" +
    "hSrv=" + hServCurPosStr + ";" +
    "lSrv=" + vServCurPosStr + ";";
  Serial.println(nextLine);
  
    if (ulValInt < urValInt)
    {
      if ((urValInt - ulValInt) > 25 )
      {
        if (hServCurPos < 135)
          {
            hServCurPos = hServCurPos + 3;
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
          hServCurPos = hServCurPos - 3;
          hServo.write(hServCurPos);
        }
      }
    }




    if (llValInt < lrValInt)
    {
      if ((lrValInt - llValInt) > 25 )
      {
        if (vServCurPos < 135)
          {
            vServCurPos = vServCurPos + 3;
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
          vServCurPos = vServCurPos - 3;
          vServo.write(vServCurPos);
        }
      }
    }

  delay(25);
}
