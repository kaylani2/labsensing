/*
 * Author: Kaylani Bochie
 * 
 * Measures the distance between the sensor and an object. That's it.
 * 
 * Note: timers don't work well with non-avr microcontrollers
 * 
 */


#define SERIAL_BAUD_RATE                          115200

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


// Directives for the HC-SR04
#include <NewPing.h>
#define TRIGGER_PIN                               D3
#define ECHO_PIN                                  D4
#define MAXIMUM_DISTANCE                          400 

// Global variables for the HC-SR04
NewPing mySonar (TRIGGER_PIN, ECHO_PIN, MAXIMUM_DISTANCE);

void setup ()
{
  Serial.begin (SERIAL_BAUD_RATE);
}

void loop ()
{
  delay (50); // at least 29ms between pings 
  Serial.print ("Distance: ");
  Serial.print (mySonar.ping_cm ());
  Serial.println ("cm");
}
