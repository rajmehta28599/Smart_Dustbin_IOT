/* Code Written by Raj Mehta
  Web Site:https://github.com/rajmehta28599/Smart_Dustbin_IOT
*/

#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <SimpleTimer.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

/* ##############  Blynk Connection Setup  ############## */
char auth[] = "IFJmY6JKXItxvvW07kcT95S89VtmaXwn"; // Name of your WiFi SSID
char ssid[] = "jsk";
char pass[] = "rajbhaimehta"; // WiFi Password
char server[] = "blynk-cloud.com";
const int trigP = D4;  //D4 Or GPIO-2 of nodemcu
const int echoP = D3;  //D3 Or GPIO-0 of nodemcu

long duration;
int distance;


/*  virtual pins:
  # V0: MAP
  # V1: LCD screen in Advanced
*/

// Blynk LCD and map widgets
WidgetLCD lcd(V1);
WidgetMap myMap(V0);
String GPSLabel = "BLYNK"; //Labeling location on MAP

/* ##############  Timer  ############## */
SimpleTimer timer;

/* ##############  GPS PIN Setup  ############## */
static const int RXPin = 4, TXPin = 5;   // GPIO 4=D2(conneect Tx of GPS) and GPIO 5=D1(Connect Rx of GPS)
static const uint32_t GPSBaud = 9600; //if Baud rate 9600 didn't work in your case then use 4800

TinyGPSPlus gps;                             // The TinyGPS++ object
SoftwareSerial ss(RXPin, TXPin);     // Serial connection to the GPS module

/* ############################
   Starting Program
   ############################ */

void setup() {
  Serial.begin(9600);       // serial connection for debugging
  ss.begin(GPSBaud);
  //Connect Blynk
  Blynk.begin(auth, ssid, pass, server, 8080);
  Serial.println("Activating GPS");
  timer.setInterval(1000L, periodicUpdate);
  timer.setInterval(60 * 1000, reconnectBlynk);
  pinMode(trigP, OUTPUT);  // Sets the trigPin as an Output
  pinMode(echoP, INPUT);   // Sets the echoPin as an Input

}
void ultra()
{
  digitalWrite(trigP, LOW);   // Makes trigPin low
  delayMicroseconds(2);       // 2 micro second delay

  digitalWrite(trigP, HIGH);  // tigPin high
  delayMicroseconds(10);      // trigPin high for 10 micro seconds
  digitalWrite(trigP, LOW);   // trigPin low

  duration = pulseIn(echoP, HIGH);   //Read echo pin, time in microseconds
  distance = duration * 0.034 / 2;   //Calculating actual/real distance

  //Serial.print("Distance = ");        //Output distance on arduino serial monitor
  //Serial.println(distance);

}
//Show GPS lat and lng on LCD
void periodicUpdate() {
  String line1, line2;
  //LCD
  lcd.clear();
  if (gps.location.isValid() && (gps.location.age() < 3000)) {
    //position current
    line1 = String("lat: ") + String(gps.location.lat(), 6);
    line2 = String("lng: ") + String(gps.location.lng(), 6);
    lcd.print(0, 0, line1);
    lcd.print(1, 1, line2);
    //update location on map
    myMap.location(2, gps.location.lat(), gps.location.lng(), GPSLabel);
  } else {
    //position is lost
    lcd.print(0, 0, "GPS lost");
  }
}

void updateGPS() {
  //read data from GPS module
  while (ss.available() > 0) {
    gps.encode(ss.read());
  }
}

void reconnectBlynk() {
  if (!Blynk.connected()) {
    Serial.println("Lost connection");
    if (Blynk.connect()) Serial.println("Reconnected");
    else Serial.println("Not reconnected");
  }
}

void loop() {
  timer.run();
  if (Blynk.connected()) {
    Blynk.run(); if (distance > 15) {
    //  Blynk.notify("Dustbin is full");
    }
  }
  updateGPS();
//  ultra();

}
