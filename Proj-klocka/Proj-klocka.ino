/*
* Name: clock and temp project
* Author: Axel Börjeson
* Date: 2024-10-11
* Description: This project uses a ds3231 to measure time and displays the time to an 1306 oled display,
* Further, it measures temprature with a analog temprature module and displays a mapped value to a 9g-servo-motor
*/

// Include Libraries
#include "U8glib.h"
#include <RTClib.h>
#include <Wire.h>
#include <Servo.h>

// Init constants
const int tmpPin = A1;
const int servoPin = 9;  // Pinnen som servon är kopplad till
const int motorPin = A0;
const int ledredPin = 5;
const int ledgreenPin = 6;
const int ledbluePin = 7;
// Init global variables
int Vo;
float R1 = 10000;  // Value of R1 on board
float logR2, R2, T;
float c1 = 0.001129148, c2 = 0.000234125, c3 = 0.0000000876741;  // Steinhart-Hart coefficients for thermistor
float threshold = 23;


// construct objects
RTC_DS3231 rtc;
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);
Servo myServo;

void setup() {
  // init communication
  u8g.setFont(u8g_font_unifont);
  Serial.begin(9600);
  Wire.begin();
  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // Init Hardware
  myServo.attach(servoPin);
  pinMode(tmpPin, INPUT);
  pinMode(motorPin, OUTPUT);
  pinMode(ledredPin, OUTPUT);
  pinMode(ledgreenPin, OUTPUT);
  pinMode(ledbluePin, OUTPUT);

}

void loop() {
  DateTime now = rtc.now();
  String timeStr = getTime();
  oledWrite(timeStr);

  //float temp = getTemp();
  servoWrite(now.second());
  flaktmotor();
  setLEDColor(getTemp());
  delay(1000);
}


/*
*This function reads time from an ds3231 module and package the time as a String
*Parameters: Void
*Returns: time in hh:mm:ss as String
*/
String getTime() {
  DateTime now = rtc.now();
  char buf[9];
  snprintf(buf, sizeof(buf), "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
  return String(buf);
}


/*
* This function reads an analog pin connected to an analog temprature sensor and calculates the corresponding temp
*Parameters: Void
*Returns: temprature as float
*/
float getTemp() {

  Vo = analogRead(tmpPin);


  R2 = R1 * (1023.0 / (float)Vo - 1.0);  // Calculate resistance of thermistor
  logR2 = log(R2);
  T = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));  // Temperature in Kelvin
  T = T - 273.15;                                              // Convert Kelvin to Celsius
  Serial.println(T);
  return T;  // Return temperature in Celsius
}


/*
* This function takes a string and draws it to an oled display
*Parameters: - text: String to write to display
*Returns: void
*/
void oledWrite(String text) {
  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_unifont);
    u8g.drawStr(0, 20, text.c_str());
  } while (u8g.nextPage());
}


/*
* takes a time value and maps it to corresppnding degree on a servo
*Parameters: - value: tid
*Returns: void
*/
void servoWrite(int seconds) { 
  int angle = map(getTemp(), 10, 30, 0, 170);
  myServo.write(angle);  // Set servo to the mapped angle
}

/*
* Controls the fan motor based on a temperature threshold.
*Parameters: Temp
* Returns: void
*/
void flaktmotor() {
  if (getTemp() > threshold) {
    digitalWrite(motorPin, HIGH);  // Turn motor on
  } else {
    digitalWrite(motorPin, LOW);   // Turn motor off
  }
}

/*
* This function controls an RGB LED to change color gradually based on temperature.
* Parameters: - Temp
* Returns: void
*/
void setLEDColor(float temp) {
  int redValue = 0, greenValue = 0;

  if (temp <= threshold - 2) {
    // Below threshold - 2: LED glows green
    greenValue = 255;
  } else if (temp >= threshold + 2) {
    // Above threshold + 2: LED glows red
    redValue = 255;
    greenValue = 0;
  } else if (temp > threshold - 2 && temp < threshold) {
    // Between threshold - 2 and threshold: Gradual change from green to yellow
    float ratio = (temp - (threshold - 2)) / 2.0;
    greenValue = 255;
    redValue = int(255 * ratio);
  } else if (temp >= threshold && temp < threshold + 2) {
    // Between threshold and threshold + 2: Gradual change from yellow to red
    float ratio = (temp - threshold) / 2.0;
    redValue = 255;
    greenValue = int(255 * (1 - ratio));
  }

  analogWrite(ledredPin, redValue);
  analogWrite(ledgreenPin, greenValue);
  analogWrite(ledbluePin, 0);  // Only use red and green for these colors
}