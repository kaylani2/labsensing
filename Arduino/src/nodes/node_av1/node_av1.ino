/*
 * Author: Kaylani Bochie
 * 
 * Node: av1
 * Components:
 *  DHT22
 *  Door Sensor
 *  MQ-02
 *
 */

// TODO: CHECK IF SMOKE SENSOR OUTPUTS HIGH OR LOW WHEN THERE IS SMOKE
// CHANGE ON READ AND INPUT (OR USE AN INVERTER)

// Other directives
#define SERIAL_BAUD_RATE                          115200
#define OK                                        0
#define EVENT_DELAY                               2000 //ms
// The delay is there to ensure that the node won't publish
// a ton of messages whilst the door is open

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
#include <globalFunctions.h>
#define DHT_TYPE                                DHT22
#define DHT_PIN                                 D1


// Global variables for the DHT22 sensor
DHT_Unified myDht (DHT_PIN, DHT_TYPE);
//unsigned int delayMS; // Won't be used in this sketch
String stringAirTemperature = "0";
String stringAirHumidity = "0";
// These are global because if we detect a change in the door,
// the node will publish the information and this allows it
// to send the previous values

// Gloval variables for the reed switch
#define DOOR_SENSOR_PIN                       D2

// Gloval variables for the smoke sensor
#define SMOKE_SENSOR_PIN                      D6

// Directives for Wi-Fi on the esp8266
#include <ESP8266WiFi.h>
#define NETWORK_SSID                              "NETWORK_SSID"
#define NETWORK_PASSWORD                          "NETWORK_PASSWORD"

// Directives for the MQQT client
#include <PubSubClient.h>
#define MQTT_SERVER                 "192.168.25.10"
#define MQTT_PORT                   1883 // standard port
#define MQTT_TEMPERATURE_TOPIC      "gta/av1/temp"
#define MQTT_HUMIDITY_TOPIC         "gta/av1/hum"
#define MQTT_DOOR_TOPIC             "gta/av1/door"
#define MQTT_SMOKE_TOPIC            "gta/av1/smoke"
#define MQTT_INFLUX_TOPIC           "gta/av1/influx"
#define MQTT_USERNAME               "pi"
#define MQTT_PASSWORD               "gta"
#define CLIENT_ID                   "could_be_anything"
#define MQTT_DELAY                  5000 // ms

// Global variables for the PubSubClient
WiFiClient myWifiClient;
PubSubClient myClient (MQTT_SERVER, MQTT_PORT, myWifiClient);

/*
 * int makeWifiConnection (const char *, const char *)
 * 
 * Arguments:
 * const char * - String holding the SSID of the network that will be used to make a connection (I)
 * const char * - String holding the password of the network that will be used to make a connection (I)
 * 
 * Return values:
 * OK - The connection was established successfully.
 * 
 * Description:
 * This function receives the name and password of a Wi-Fi network and connects to it.
 * 
 */
int makeWifiConnection (const char *networkSSID, const char *networkPassword)
{
  Serial.println ();
  Serial.print ("Connecting to: ");
  Serial.println (networkSSID);
  Serial.println ();
  delay (1000);
  
  WiFi.begin (networkSSID, networkPassword);
  delay (500);
  while (WiFi.status () != WL_CONNECTED)
  {
    delay (1000);
    Serial.print (".");
  }
  Serial.println ();
  Serial.println ("WiFi connected");

  Serial.println ("Local IP: ");
  Serial.println (WiFi.localIP());

  delay (500);
  return OK;
}

void setup ()
{
  delay (2000);
  Serial.begin (SERIAL_BAUD_RATE);
  Serial.println ("Setup initiated.");
  pinMode (BUILT_IN_LED, OUTPUT);

  // DOOR SENSOR SETUP
  pinMode (DOOR_SENSOR_PIN, INPUT_PULLUP);

  // SMOKE SENSOR SETUP
  pinMode (SMOKE_SENSOR_PIN, INPUT_PULLUP);

  // DHT SENSOR SETUP
  myDht.begin ();
  printSensorDetails (myDht);
  // Set delay between sensor readings based on sensor details.
  //sensor_t sensor;
  //delayMS = sensor.min_delay / 1000;
  // No need to use this delay in this sketch

  // MQTT SETUP
  makeWifiConnection (NETWORK_SSID, NETWORK_PASSWORD);
  if (myClient.connect (CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD))
    Serial.println ("Connected to MQTT Broker!");
  else
    Serial.println ("Connection to MQTT Broker failed!");

  Serial.println ("Setup finished.");
}

void loop ()
{
  String stringDoorSensor = "CLOSED";
  String stringSmokeSensor = "OK";
  unsigned long currentMillis = millis ();

  // if (every few seconds) or (the door was opened) or (smoke is detected)
  if ( (currentMillis - previousMillis >= MQTT_DELAY) || (digitalRead (DOOR_SENSOR_PIN) == LOW) || (digitalRead (SMOKE_SENSOR_PIN) == LOW) ) 
  {
    previousMillis = currentMillis;

    Serial.println ("--------------------------------------");

    // If there is an error on the reading, the strings
    // will keep their previous values
    getAirTemperature (myDht, stringAirTemperature);
    getAirHumidity (myDht, stringAirHumidity);
    if (digitalRead (DOOR_SENSOR_PIN) == LOW)
      stringDoorSensor = "OPEN";
    if (digitalRead (SMOKE_SENSOR_PIN) == LOW)
      stringSmokeSensor = "SMOKE";

    Serial.print ("Air temperature: ");
    Serial.println (stringAirTemperature);
    Serial.print ("Air humidity: ");
    Serial.println (stringAirHumidity);
    Serial.print ("The door is: ");
    Serial.println (stringDoorSensor);
    String influxMessage = stringAirTemperature + ";" + stringAirHumidity + ";" + stringDoorSensor + ";" + stringSmokeSensor;
    Serial.print ("Inlux message: ");
    Serial.println (influxMessage);
    
    publishToTopic (myClient, MQTT_TEMPERATURE_TOPIC, stringAirTemperature.c_str (), CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD);
    publishToTopic (myClient, MQTT_HUMIDITY_TOPIC, stringAirHumidity.c_str (), CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD);
    publishToTopic (myClient, MQTT_DOOR_TOPIC, stringDoorSensor.c_str (), CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD);
    publishToTopic (myClient, MQTT_SMOKE_TOPIC, stringSmokeSensor.c_str (), CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD);
    publishToTopic (myClient, MQTT_INFLUX_TOPIC, influxMessage.c_str (), CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD);

    Serial.println ("--------------------------------------");
    // The condition was triggered by an event, not by time
    if ( (!stringSmokeSensor.equals ("OK")) || (!stringDoorSensor.equals ("CLOSED")) )
      delay (EVENT_DELAY);
    digitalWrite (BUILT_IN_LED, !digitalRead (BUILT_IN_LED));
  } 
}
