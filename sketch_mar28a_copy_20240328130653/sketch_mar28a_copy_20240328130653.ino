 /**
 * An Arduino Volt Meter Analog Clock based on a TinyRTC real time clock.
 *
 * For more info about this project, check out my blog for the full story
 * images, and the endresult of the project:
 * http://michaelteeuw.nl/post/174972004187/what-time-is-it-fathers-day
 *
 * LICENSE: Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation files
 * (the "Software"). For more info see LICENSE file.
 *
 * @category   CategoryName
 * @package    PackageName
 * @author     Michael Teeuw
 * @copyright  2018
 * @license    https://opensource.org/licenses/MIT
 * @link       http://michaelteeuw.nl
 */

#include <Arduino.h>
#include <Wire.h>
#include <FastLED.h>
#include "RTClib.h" // https://github.com/adafruit/RTClib

#define LED_PIN     6
#define NUM_LEDS    3
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define BRIGHTNESS  200

// Define the Pins we are using.
#define PIN_BUTTON_HOUR 7
#define PIN_BUTTON_MIN 8
#define PIN_METER_HOUR 9
#define PIN_METER_MIN 5
#define PIN_METER_SEC 3

// Define the minimum and maximum PWM values for all the meters.
// These values can be used to calibrate the meters.
#define MINIMUM_PWM_VALUE_HOUR 15
#define MAXIMUM_PWM_VALUE_HOUR 240
#define MINIMUM_PWM_VALUE_MIN 15
#define MAXIMUM_PWM_VALUE_MIN 254
#define MINIMUM_PWM_VALUE_SEC 1
#define MAXIMUM_PWM_VALUE_SEC 254


CRGB leds[NUM_LEDS];
unsigned long previousMillis = 0;  // will store last time LED was updated
unsigned long previousMillis1 = 0;  // will store last time LED was updated

// constants won't change:
const long interval = 100;  // interval at which to blink (milliseconds)
static int8_t s;
static int8_t m;
static int8_t h;
// Create the real time clock instance.
RTC_DS1307 rtc;

/**
 * Slowly animate one meter to the maximum value and back.
 * This animation is used in the start sequence. Not only does it look
 * cool. It's also a way to test the meters.
 *
 * @param uint8_t pin  The pin of the meter.
 * @param uint8_t min  The meter's minimum PWM value.
 * @param uint8_t max  The meter's maximum PWM value.
 
 */
void animateMeter(uint8_t pin, uint8_t min, uint8_t max) {
  uint8_t v;
  for (v = min; v <= max; v++) {
    analogWrite(pin, v);
    delay(2);
  }
  delay(100);
  for (v = max; v >= min; v--) {
    analogWrite(pin, v);
    delay(2);
  }
  delay(100);
}

/**
 * Run the startup sequence by animating all three meters sequentially.
 */
void startSequence() {
  animateMeter(PIN_METER_HOUR, MINIMUM_PWM_VALUE_HOUR, MAXIMUM_PWM_VALUE_HOUR);
  animateMeter(PIN_METER_MIN, MINIMUM_PWM_VALUE_MIN, MAXIMUM_PWM_VALUE_MIN);
  animateMeter(PIN_METER_SEC, MINIMUM_PWM_VALUE_SEC, MAXIMUM_PWM_VALUE_SEC);
}

/**
 * The method to update the time displayed on the the meter.
 */
void displayTime(uint8_t s,uint8_t m,uint8_t h) {
  // Retrieve the time from the real time clock.
  DateTime now = rtc.now();

  // Since the real time clock returns a 24 hour format,
  // we need to convert the hour value to 12 hour format.
  // For minutes and seconds we can just directly use the
  // properties on the 'now' object. No need to store those
  // in a temporary variable.
  // int8_t h = (now.hour() == 0 || now.hour() == 12) ? 12 : now.hour() % 12;

 

  // Convert the time values to PWM values by using map ...
  // and set that value as the PWM value using analogWrite.
  analogWrite(PIN_METER_HOUR, map(h, 1, 12, MINIMUM_PWM_VALUE_HOUR, MAXIMUM_PWM_VALUE_HOUR));
  analogWrite(PIN_METER_MIN, map(m, 0, 60, MINIMUM_PWM_VALUE_MIN, MAXIMUM_PWM_VALUE_MIN));
  analogWrite(PIN_METER_SEC, map(s, 0, 10, MINIMUM_PWM_VALUE_SEC, MAXIMUM_PWM_VALUE_SEC));

  // For debuging purposes, write the time to the
  // Serial port as well.
  Serial.print(h);
  Serial.print(":");
  Serial.print(now.minute());
  Serial.print(":");
  Serial.println(s);
}

/**
 * The setup method used by the Arduino.
 */
void setup () {
  // Initiate the serial port for debugging purposes.
  Serial.begin(9600);

  // Configure all the pin modes for all connected pins.
  pinMode(PIN_BUTTON_HOUR, INPUT_PULLUP);
  pinMode(PIN_BUTTON_MIN, INPUT_PULLUP);
  pinMode(PIN_METER_HOUR, OUTPUT);
  pinMode(PIN_METER_MIN, OUTPUT);
  pinMode(PIN_METER_SEC, OUTPUT);



  // Start the real time clock.
  rtc.begin();

  // If the real time clock is not running,
  // for example after the battery was replaced,
  // start it by setting the time.
  if (!rtc.isrunning()) {
    rtc.adjust(DateTime(2018, 6, 17, 0, 0, 0));
  }

  // Initiate the power-on self test by running the start sequence.
  startSequence();
   delay(1000);
    LEDS.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.clear();
    FastLED.show();
}

/**
 * The main runloop used by the Arduino.
 */
 void loop () {
//     // Update the meters by calling the `displayTime()` method.
  leds[0] = CRGB(255, 255, 255);
  leds[1] = CRGB(255, 255, 255);
  leds[2] = CRGB(255, 255, 255);
   unsigned long currentMillis = millis();  

   if (currentMillis - previousMillis1 >= interval) {
    // save the last time you blinked the LED
    previousMillis1 = currentMillis;

if (s>=10){
      s=0;
      h=h+1;
    }
    else {
      s=(s+1);
    }
    
    if (h>=24){
      h=0;
    }
    

  }

  displayTime(s,m,h);
    // Check if we pressed the button to adjust the hour ...
    // Repeat the following loop for as long as we hold this button.
    // while (!digitalRead(PIN_BUTTON_HOUR)) {
    //   // Grab the current time.
    //   DateTime now = rtc.now();

    //   // Set the RTC time by using the current time and increaing the hour by one.
    //   // By using the % 24, it will automaticly overflow to 0 when it reaches 24.
    //   // The seconds are simply reset.
    //   rtc.adjust(DateTime(now.year(), now.month(), now.day(), (now.hour() + 1) % 24, now.minute(), 0));

    //   // Update the display to reflect the changes.
    //   displayTime();

    //   // Wait half a second before we check again to see
    //   // if the button is still pressed.
    //   delay(500);
    // }

    // // Check if we pressed the button to adjust the minutes ...
    // // Repeat the following loop for as long as we hold this button.
    // while (!digitalRead(PIN_BUTTON_MIN)) {
    //   // Grab the current time.
    //   DateTime now = rtc.now();

    //   // Set the RTC time by using the current time and increaing the minutes by one.
    //   // By using the % 60, it will automaticly overflow to 0 when it reaches 60.
    //   // The seconds are simply reset.
    //   rtc.adjust(DateTime(now.year(), now.month(), now.day(), now.hour(), (now.minute() + 1) % 60, 0));

    //   // Update the display to reflect the changes.
    //   displayTime();

    //   // Wait a quarter of a second before we check again to see
    //   // if the button is still pressed.
   
    // }

    // Wait for 1 second before we start all over again.
FastLED.show();
}