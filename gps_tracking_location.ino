/* Code Written by Raj Mehta
-  Web Site:https://github.com/rajmehta28599/Smart_Dustbin_IOT
*/

#include <SoftwareSerial.h>
#include <TinyGPS.h>
TinyGPS gps;
SoftwareSerial ss(4,5);// GPIO 4=D2(conneect Tx of GPS) and GPIO 5=D1(Connect Rx of GPS)
void setup()
{
  Serial.begin(9600);
  ss.begin(9600);
  Serial.println("Welcome To Fuel Tank Guard... :) "); 
  Serial.print("Simple TinyGPS library v. "); 
  Serial.println(TinyGPS::library_version());
  Serial.println();
}
void loop()
{
  bool newData = false;
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (ss.available())
    {
      char c = ss.read();

      if (gps.encode(c))
        newData = true;
    }
  }
  if (newData)
  {
    float flat, flon;
    //unsigned long age;
    gps.f_get_position(&flat, &flon);//, &age);
    Serial.print("LAT=");
    Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    Serial.print(" LON=");
    Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    Serial.println();
  }
}
