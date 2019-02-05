/*
 * Author: Kaylani Bochie
 * 
 * Prints sensor details. Reads temperature and humidity from DHT22 and prints on the serial monitor. That's it.
 */

// TODO: write a wrapper for the dht sensor
// note: no support for floats by printf-like functions, must use dtostrf () instead


// Other directives
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
#define DHT_TYPE                                DHT22
#define DHT_PIN                                 D1
#define DATA_STRING_LENGTH                      32

// Global variables for the DHT22 sensor
DHT_Unified myDht (DHT_PIN, DHT_TYPE);
unsigned int delayMS;


void setup ()
{
  Serial.begin (SERIAL_BAUD_RATE);

  pinMode (BUILT_IN_LED, OUTPUT);

  myDht.begin ();
  sensor_t sensor;
  myDht.temperature().getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.println("Temperature");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" *C");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" *C");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" *C");  
  Serial.println("------------------------------------");
  myDht.humidity().getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.println("Humidity");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println("%");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println("%");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println("%");  
  Serial.println("------------------------------------");
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;
}

void loop ()
{
  // The values are being converted to strings in order to facilitate handling later
  char stringAirTemperature [DATA_STRING_LENGTH];
  char stringAirHumidity [DATA_STRING_LENGTH];
  sensors_event_t myEvent;  
  
  myDht.temperature().getEvent(&myEvent);
  if (isnan(myEvent.temperature))
  {
    Serial.println ("Error reading temperature!");
  }
  else
  {
    float floatAirTemperature = (float) myEvent.temperature;
    dtostrf (floatAirTemperature, 1, 2, stringAirTemperature);
    Serial.print ("Temperature: ");
    Serial.print (stringAirTemperature);
    Serial.println (" *C");
  }
  
  myDht.humidity().getEvent(&myEvent);
  if (isnan(myEvent.relative_humidity))
  {
    Serial.println("Error reading humidity!");
  }
  else
  {
    float floatAirHumidity = (float) myEvent.relative_humidity;
    dtostrf (floatAirHumidity, 1, 2, stringAirHumidity);
    Serial.print ("Humidity: ");
    Serial.print (stringAirHumidity);
    Serial.println ("%");
  }
  

  
  digitalWrite (BUILT_IN_LED, !digitalRead (BUILT_IN_LED));
  delay (delayMS);
}
