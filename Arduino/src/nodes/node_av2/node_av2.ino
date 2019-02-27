/*
 * Author: Kaylani Bochie
 * 
 * Node: av2
 * Components:
 *  DHT22
 *  Door Sensor
 *  MFRC522
 *
 * Note: The RFID Tag won't be read if the door is open.
 * This behavior is a bug, however that restriction makes (some) sense
 * for access control.
 */


// Other directives
#include <globalFunctions.h> // Useful functions
#include <gtaConfig.h> // Important macros
#define SERIAL_BAUD_RATE                          115200
#define OK                                        0

// Other global variables
unsigned long previousMillis = 0;

/* Circuit for the MFRC522:

  Typical pin layout used:
   ----------------------------------------------------------------------------------------------------
               MFRC522       Arduino        NodeMCU    Arduino   Arduino    Arduino          Arduino
               Reader/PCD    Uno/101        ESP8266    Mega      Nano v3    Leonardo/Micro   Pro Micro
   Signal      Pin           Pin            Pin        Pin       Pin        Pin              Pin
   ----------------------------------------------------------------------------------------------------
   RST/Reset   RST           9              D3 [1]      5         D9         RESET/ICSP-5     RST
   SPI SS      SDA(SS) [3]   10             D8 [2]      53        D10        10               10
   SPI MOSI    MOSI          11 / ICSP-4    D7          51        D11        ICSP-4           16
   SPI MISO    MISO          12 / ICSP-1    D6          50        D12        ICSP-1           14
   SPI SCK     SCK           13 / ICSP-3    D5          52        D13        ICSP-3           15

    [1] (1, 2) Configurable, typically defined as RST_PIN in sketch/program.
    [2] (1, 2) Configurable, typically defined as SS_PIN in sketch/program.
    [3] The SDA pin might be labeled SS on some/older MFRC522 boards
*/
// Directives for the MFRC522
#include <SPI.h>
#include <MFRC522.h>
#define RESET_PIN                             D3
#define SLAVE_SELECT_PIN                      D8

// Global variables for the MFRC522
MFRC522 myMfrc522 (SLAVE_SELECT_PIN, RESET_PIN);
// This object will hold the ID that will be read

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



// Global variables for the DHT22 sensor
DHT_Unified myDht (DHT_PIN, DHT_TYPE);
//unsigned int delayMS; // Won't be used in this sketch
String stringAirTemperature = DHT_ERROR_STRING;
String stringAirHumidity = DHT_ERROR_STRING;
// These are global because if we detect a change in the door,
// the node will publish the information and this allows it
// to send the previous values

// Gloval variables for the reed switch
#define DOOR_SENSOR_PIN                       D2

// Directives for Wi-Fi on the esp8266
#include <ESP8266WiFi.h>

// Directives for the MQQT client
#include <PubSubClient.h>
#define MQTT_TEMPERATURE_TOPIC      AV2_MQTT_TEMPERATURE_TOPIC
#define MQTT_HUMIDITY_TOPIC         AV2_MQTT_HUMIDITY_TOPIC
#define MQTT_DOOR_TOPIC             AV2_MQTT_DOOR_TOPIC
#define MQTT_RFID_TOPIC             AV2_MQTT_RFID_TOPIC
#define MQTT_INFLUX_TOPIC           AV2_MQTT_INFLUX_TOPIC
#define CLIENT_ID                   AV2_CLIENT_ID

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

  // MFRC522 SETUP
  SPI.begin ();
  myMfrc522.PCD_Init ();

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
  String stringRfidTag = "NO_ID";
  unsigned long currentMillis = millis ();

  // if (time goes by) or (the door was opened) or (rfid tag is present)
  if ( (currentMillis - previousMillis >= MQTT_DELAY) || (digitalRead (DOOR_SENSOR_PIN) == LOW) || (myMfrc522.PICC_IsNewCardPresent ()) )
  {
    previousMillis = currentMillis;

    Serial.println ("--------------------------------------");
    if (myMfrc522.PICC_ReadCardSerial ())
    {
      byteArrayToString (myMfrc522.uid.uidByte, myMfrc522.uid.size, stringRfidTag);
      Serial.print ("RFID: ");
      Serial.println (stringRfidTag);
    }

    // If there is an error on the reading, the strings
    // will keep their previous values
    getAirTemperature (myDht, stringAirTemperature);
    getAirHumidity (myDht, stringAirHumidity);
    if (digitalRead (DOOR_SENSOR_PIN) == LOW)
      stringDoorSensor = "OPEN";

    Serial.print ("Air temperature: ");
    Serial.println (stringAirTemperature);
    Serial.print ("Air humidity: ");
    Serial.println (stringAirHumidity);
    Serial.print ("The door is: ");
    Serial.println (stringDoorSensor);
    String influxMessage = stringAirTemperature + ";" + stringAirHumidity + ";" + stringDoorSensor + ";" + stringRfidTag;
    Serial.print ("Inlux message: ");
    Serial.println (influxMessage);
  
    if (!stringAirTemperature.equals (DHT_ERROR_STRING)) 
      publishToTopic (myClient, MQTT_TEMPERATURE_TOPIC, stringAirTemperature.c_str (), CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD);
    if (!stringAirHumidity.equals (DHT_ERROR_STRING)) 
      publishToTopic (myClient, MQTT_HUMIDITY_TOPIC, stringAirHumidity.c_str (), CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD);
    publishToTopic (myClient, MQTT_DOOR_TOPIC, stringDoorSensor.c_str (), CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD);
    publishToTopic (myClient, MQTT_RFID_TOPIC, stringRfidTag.c_str (), CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD);
    publishToTopic (myClient, MQTT_INFLUX_TOPIC, influxMessage.c_str (), CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD);

    Serial.println ("--------------------------------------");
    // The condition was triggered by an event, not by time
    if ( (!stringRfidTag.equals ("NO_ID")) || (!stringDoorSensor.equals ("CLOSED")) )
      delay (EVENT_DELAY);
    digitalWrite (BUILT_IN_LED, !digitalRead (BUILT_IN_LED));
  } 
}
