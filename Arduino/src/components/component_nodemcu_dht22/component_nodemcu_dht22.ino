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
#define ERROR_READING_TEMPERATURE               1
#define ERROR_READING_HUMIDITY                  2


// Global variables for the DHT22 sensor
DHT_Unified myDht (DHT_PIN, DHT_TYPE);
unsigned int delayMS;


void printSensorDetails (DHT_Unified myDht)
{
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
}

int getAndPrintAirTemperature (DHT_Unified myDht, String &stringAirTemperature)
{
  sensors_event_t myEvent;  
  
  myDht.temperature().getEvent(&myEvent);
  if (isnan(myEvent.temperature))
  {
    Serial.println ("Error reading temperature!");
    return ERROR_READING_TEMPERATURE;
  }
  
  float floatAirTemperature = (float) myEvent.temperature;
  stringAirTemperature = String (floatAirTemperature, 3);
  Serial.print ("Temperature: ");
  Serial.print (stringAirTemperature);
  Serial.println (" *C");
  return OK;
}

int getAndPrintAirHumidity (DHT_Unified myDht, String &stringAirHumidity)
{
  sensors_event_t myEvent;  

  myDht.humidity().getEvent(&myEvent);
  if (isnan(myEvent.relative_humidity))
  {
    Serial.println ("Error reading humidity!");
    return ERROR_READING_HUMIDITY;
  }
  
  float floatAirHumidity = (float) myEvent.relative_humidity;
  stringAirHumidity = String (floatAirHumidity, 3);
  Serial.print ("Humidity: ");
  Serial.print (stringAirHumidity);
  Serial.println ("%");
  return OK;
}


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
  // The values are being converted to strings in order to facilitate handling later
  String stringAirTemperature;
  String stringAirHumidity;
  unsigned long currentMillis = millis ();

  if (currentMillis - previousMillis >= delayMS)
  {
    previousMillis = currentMillis;
    getAndPrintAirTemperature (myDht, stringAirTemperature);
    getAndPrintAirHumidity (myDht, stringAirHumidity);  
    digitalWrite (BUILT_IN_LED, !digitalRead (BUILT_IN_LED));
  } 
}
