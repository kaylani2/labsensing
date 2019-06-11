/*
 * Author: Kaylani Bochie
 * 
 * Node: threshold_batching
 * Components:
 *  DHT22
 *  Door Sensor
 *
 */


// Other directives
#include <globalFunctions.h> // Useful functions
#include <gtaConfig.h> // Important macros
#define SERIAL_BAUD_RATE                          115200
#define OK                                        0

// Other global variables
unsigned long previousMillis = 0;
unsigned long batchIndex = 0;

// Directives for the DHT22 sensor
#include <Adafruit_Sensor.h>
#include <DHT_U.h>
#include <DHT.h>
#define DHT_TYPE                                DHT22
#define DHT_PIN                                 D1

// Global variables for the DHT22 sensor
DHT_Unified myDht (DHT_PIN, DHT_TYPE);
String stringAirTemperature = DHT_ERROR_STRING;
String stringAirHumidity = DHT_ERROR_STRING;
String stringAirTemperatureFinal = "";
String stringAirHumidityFinal = "";

// Gloval variables for the reed switch
#define DOOR_SENSOR_PIN                       D2

// Directives for Wi-Fi on the esp8266
#include <ESP8266WiFi.h>

// Directives for the MQQT client
#include <PubSubClient.h>
#define MQTT_TEMPERATURE_TOPIC      THRESHOLD_BATCHING_MQTT_TEMPERATURE_TOPIC
#define MQTT_HUMIDITY_TOPIC         THRESHOLD_BATCHING_MQTT_HUMIDITY_TOPIC
#define MQTT_INFLUX_TOPIC           THRESHOLD_BATCHING_MQTT_INFLUX_TOPIC
#define CLIENT_ID                   THRESHOLD_BATCHING_CLIENT_ID

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
  delay (3000);
}

void loop ()
{
  String stringDoorSensor = "CLOSED";
  unsigned long currentMillis = millis ();
  float temperatureDifference = 0; // see abs () documentation

  // if (time goes by)
  if ( ((currentMillis - previousMillis) >= (BATCH_DELAY * batchIndex)) )
  {
    batchIndex++;
    Serial.println ("--------------------------------------");

    // If there is an error on the reading, the strings
    // will keep their previous values
    String stringPreviousAirTemperature = stringAirTemperature; 
    findAndPrintAirTemperature (myDht, stringAirTemperature, stringPreviousAirTemperature.toInt ());
    temperatureDifference = stringAirTemperature.toFloat () - stringPreviousAirTemperature.toFloat ();
/*
    Serial.print ("Previous temperature: ");
    Serial.println (stringPreviousAirTemperature.toFloat ());
    Serial.print ("Current temperature: ");
    Serial.println (stringAirTemperature.toFloat ());
    Serial.print ("Temperature difference: ");
    Serial.println ((temperatureDifference));
*/
    if (temperatureDifference < 0)
      temperatureDifference = - temperatureDifference;
    // Fill batch
    if (temperatureDifference >= TEMPERATURE_DIFFERENCE_THRESHOLD)
    {
      Serial.println ("Measure included in the batch.");
      if (stringAirTemperatureFinal.length () == 0)
        stringAirTemperatureFinal = stringAirTemperature;
      else
        stringAirTemperatureFinal = stringAirTemperatureFinal + "," + stringAirTemperature;
    }
    else
      Serial.println ("Measure rejected.");

    Serial.print ("Air temperature: ");
    Serial.println (stringAirTemperature);

    digitalWrite (BUILT_IN_LED, !digitalRead (BUILT_IN_LED));
  } 

  // Send batch
  if ( ((currentMillis - previousMillis) >= BATCH_DELAY * BATCH_SIZE))
  {
    batchIndex = 1;
    previousMillis = currentMillis;
    String influxMessage = stringAirTemperatureFinal;
    Serial.print ("Influx message: ");
    Serial.println (influxMessage);
    publishToTopic (myClient, MQTT_INFLUX_TOPIC, influxMessage.c_str (), CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD);
    stringAirTemperatureFinal = "";
  }
}
