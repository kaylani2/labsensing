/*
 * Author: Kaylani Bochie
 * 
 * Toggles the built-in led according to the opening/closing of a door sensor. That's it.
 */



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

// Directives for the door sensor
#define DOOR_SENSOR_PIN                           D3



void setup ()
{
  Serial.begin (SERIAL_BAUD_RATE);

  pinMode (BUILT_IN_LED, OUTPUT);
  pinMode (DOOR_SENSOR_PIN, INPUT_PULLUP);
}

void loop ()
{
  // The sensor used is normally closed
  if (digitalRead (DOOR_SENSOR_PIN) == HIGH) // Door was opened
  {
    Serial.println ("Door was opened.");
    delay (1000);
    while (digitalRead (DOOR_SENSOR_PIN) == HIGH)
      delay (2000);
    Serial.println ("Door was closed.");
    digitalWrite (BUILT_IN_LED, !digitalRead (BUILT_IN_LED));
  }  
}
