/*
 * Author: Kaylani Bochie
 * 
 * Prints sensor details. Reads temperature and humidity from DHT22 and prints on the serial monitor. That's it.
 */


// Other directives
#define SERIAL_BAUD_RATE                          115200
#define OK                                        0

// Other global variables
unsigned long previousMillis = 0;

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


/*  DHT22 pinout:
 *  
 *  Pin 1 (VCC) -> Vcc
 *  Pin 2 (DATA) -> DHT_PIN
 *  Pin 3 (NC) -> FLOATING  
 *  Pin 4 (GND) -> Gnd
 *  10K resistor between DHT_PIN and Vcc
 *
 *
 *    --------------
 *      ||||||||||
 *    ||||||||||||||
 *    ||||||||||||||
 *      ||||||||||
 *    ||||||||||||||    
 *    --------------
 *    |   |   |   |
 *    |   |   |   |
 *    |   |   |   |
 *    VCC     NC
 *        DATA    GND
 */
 

// Directives for the DHT22 sensor
#include <Adafruit_Sensor.h>
#include <DHT_U.h>
#include <DHT.h>
#include <dht22Functions.h>
#define DHT_TYPE                                DHT22
#define DHT_PIN                                 D1


// Global variables for the DHT22 sensor
DHT_Unified myDht (DHT_PIN, DHT_TYPE);
unsigned int delayMS;

void setup ()
{
  Serial.begin (SERIAL_BAUD_RATE);
  pinMode (BUILT_IN_LED, OUTPUT);
  myDht.begin ();
  printSensorDetails (myDht);
  // Set delay between sensor readings based on sensor details.
  sensor_t sensor;
  delayMS = sensor.min_delay / 1000;
}

void loop ()
{
  String stringAirTemperature;
  String stringAirHumidity;
  unsigned long currentMillis = millis ();

  if (currentMillis - previousMillis >= delayMS)
  {
    previousMillis = currentMillis;
    if (!getAirTemperature (myDht, stringAirTemperature))
    {
      Serial.print ("Air temperature: ");
      Serial.println (stringAirTemperature);
    }
    if (!getAirHumidity (myDht, stringAirHumidity))
    {
      Serial.print ("Air humidity: ");
      Serial.println (stringAirHumidity);
    }

    digitalWrite (BUILT_IN_LED, !digitalRead (BUILT_IN_LED));
  } 
}
