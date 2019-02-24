/*
 * Author: Kaylani Bochie
 * 
 * Publishes a message in the same format as node_mult. Used to debug node-red parser.
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
#include <globalFunctions.h>
#define DHT_TYPE                                DHT22
#define DHT_PIN                                 D1


// Global variables for the DHT22 sensor
DHT_Unified myDht (DHT_PIN, DHT_TYPE);
unsigned int delayMS;

// Directives for Wi-Fi on the esp8266
#include <ESP8266WiFi.h>
#define NETWORK_SSID                              "NETWORK_SSID"
#define NETWORK_PASSWORD                          "NETWORK_PASSWORD"

// Directives for the MQQT client
#include <PubSubClient.h>
#define MQTT_SERVER                 "192.168.25.10"
//#define MQTT_SERVER                 "146.164.69.139"
#define MQTT_PORT                   1883 // standard port
#define MQTT_TEMPERATURE_TOPIC      "gta/mult/temp"
#define MQTT_HUMIDITY_TOPIC         "gta/mult/hum"
#define MQTT_INFLUX_TOPIC           "gta/mult/influx"
#define MQTT_USERNAME               "pi"
#define MQTT_PASSWORD               "gta"
#define CLIENT_ID                   "could_be_anything"
#define MQTT_DELAY                  5000 //ms

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

  // DHT SENSOR SETUP
  myDht.begin ();
  printSensorDetails (myDht);
  // Set delay between sensor readings based on sensor details.
  sensor_t sensor;
  delayMS = sensor.min_delay / 1000;

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
  String stringAirTemperature;
  String stringAirHumidity;
  String thereIsMovement;
  unsigned long currentMillis = millis ();

  if (currentMillis - previousMillis >= MQTT_DELAY)
  {
    if (WiFi.status () != WL_CONNECTED)
      Serial.println ("WIFI DISCONNECTED");
    else      
      Serial.println ("WIFI CONNECTED");
    previousMillis = currentMillis;
    if (0 == 0) // No need to check if the reading was performed correctly
    {
      stringAirTemperature = (String) random (20, 40);
      stringAirHumidity = (String) random (0, 100);
      thereIsMovement = "NO";
      String influxMessage = stringAirTemperature + ";" + stringAirHumidity + ";" + thereIsMovement;
      Serial.print ("Air temperature: ");
      Serial.println (stringAirTemperature);
      Serial.print ("Air humidity: ");
      Serial.println (stringAirHumidity);
      Serial.print ("There is movement: ");
      Serial.println (thereIsMovement);
      publishToTopic (myClient, MQTT_TEMPERATURE_TOPIC, stringAirTemperature.c_str (), CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD);
      publishToTopic (myClient, MQTT_HUMIDITY_TOPIC, stringAirHumidity.c_str (), CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD);
      publishToTopic (myClient, MQTT_INFLUX_TOPIC, influxMessage.c_str (), CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD);
    }

    digitalWrite (BUILT_IN_LED, !digitalRead (BUILT_IN_LED));
  } 
}
