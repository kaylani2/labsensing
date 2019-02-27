/*
 * Author: Kaylani Bochie
 * 
 */


// Other directives
#include <globalFunctions.h> // Useful functions
#include <gtaConfig.h> // Important macros
#define SERIAL_BAUD_RATE                          115200
#define OK                                        0

// Other global variables
unsigned long previousMillis = 0;

// Directives for the DHT22 sensor
#include <Adafruit_Sensor.h>
#include <DHT_U.h>
#include <DHT.h>
#define DHT_TYPE                                DHT22
#define DHT_PIN                                 D1

// Global variables for the DHT22 sensor
DHT_Unified myDht (DHT_PIN, DHT_TYPE);
String stringAirTemperature = "0";
String stringAirHumidity = "0";
// These are global because if we detect a change in the door,
// the node will publish the information and this allows it
// to send the previous values

// Gloval variables for the PIR motion sensor
#define MOTION_SENSOR_PIN                       D2

// Directives for Wi-Fi on the esp8266
#include <ESP8266WiFi.h>

// Directives for the MQQT client
#include <PubSubClient.h>
#define MQTT_TEMPERATURE_TOPIC      MULT_MQTT_TEMPERATURE_TOPIC
#define MQTT_HUMIDITY_TOPIC         MULT_MQTT_HUMIDITY_TOPIC
#define MQTT_MOVEMENT_TOPIC         MULT_MQTT_MOVEMENT_TOPIC
#define MQTT_INFLUX_TOPIC           MULT_MQTT_INFLUX_TOPIC
#define CLIENT_ID                   MULT_CLIENT_ID

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
  delay (500);
  
  WiFi.begin (networkSSID, networkPassword);
  while (WiFi.status () != WL_CONNECTED)
  {
    delay (500);
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

  // PIR SENSOR SETUP
  pinMode (MOTION_SENSOR_PIN, INPUT);

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
  String stringMotionSensor = "NO";
  unsigned long currentMillis = millis ();

  if (currentMillis - previousMillis >= MQTT_DELAY)
  {
    previousMillis = currentMillis;

    Serial.println ("--------------------------------------");

    // If there is an error on the reading, the strings
    // will keep their previous values
    getAirTemperature (myDht, stringAirTemperature);
    getAirHumidity (myDht, stringAirHumidity);
    if (digitalRead (MOTION_SENSOR_PIN) == HIGH)
      stringMotionSensor = "YES";

    Serial.print ("Air temperature: ");
    Serial.println (stringAirTemperature);
    Serial.print ("Air humidity: ");
    Serial.println (stringAirHumidity);
    Serial.print ("There is movement: ");
    Serial.println (stringMotionSensor);
    String influxMessage = stringAirTemperature + ";" + stringAirHumidity + ";" + stringMotionSensor;
    Serial.print ("Inlux message: ");
    Serial.println (influxMessage);
    
    if (!stringAirTemperature.equals (DHT_ERROR_STRING)) 
      publishToTopic (myClient, MQTT_TEMPERATURE_TOPIC, stringAirTemperature.c_str (), CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD);
    if (!stringAirHumidity.equals (DHT_ERROR_STRING)) 
      publishToTopic (myClient, MQTT_HUMIDITY_TOPIC, stringAirHumidity.c_str (), CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD);
    publishToTopic (myClient, MQTT_MOVEMENT_TOPIC, stringMotionSensor.c_str (), CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD);
    publishToTopic (myClient, MQTT_INFLUX_TOPIC, influxMessage.c_str (), CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD);

    Serial.println ("--------------------------------------");
    digitalWrite (BUILT_IN_LED, !digitalRead (BUILT_IN_LED));
  } 
}
