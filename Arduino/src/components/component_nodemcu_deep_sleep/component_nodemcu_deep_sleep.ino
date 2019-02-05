/*
 * Author: Kaylani Bochie
 * 
 * Blinks the built-in led and enter deep sleep mode for a few seconds. That's it.
 * 
 */

// note: on waking up the esp8266 starts back from setup
// note: the bootloader prints rubbish on the serial monitor when exiting deep sleep mode

// Other directives
#define SERIAL_BAUD_RATE                        115200

// Directives for the NodeMCU board
#define D0                                        16
#define D1                                        5
#define D2                                        4
#define D3                                        0
#define D4                                        2
#define D5                                        14
#define D6                                        12
#define D7                                        13
#define D8                                        15
#define D9                                        3
#define D10                                       1
#define BUILT_IN_LED                              D4
#define INDICATOR_LED                             D4

// Directives for deep sleep
#define SLEEP_TIME                                5e6

void blinkLed (unsigned int timeInSeconds, unsigned int ledPin)
{
  for (unsigned int blinkCount = 0; blinkCount < timeInSeconds * 4; blinkCount++)
  {
    digitalWrite (ledPin, !digitalRead (ledPin));
    delay (250);
  }
}

void setup ()
{
  delay (100);
  Serial.begin (SERIAL_BAUD_RATE);
  pinMode (BUILT_IN_LED, OUTPUT);

  blinkLed (5, BUILT_IN_LED);
  Serial.println ();
  Serial.print ("Entering deep sleep mode for ");
  Serial.print (SLEEP_TIME);
  Serial.println (" microseconds.");
  ESP.deepSleep (SLEEP_TIME);

  //ESP.deepSleep (0);
  // /\ the device exits deep sleep mode when the RST pin detects a RISING EDGE

}

void loop()
{
}
