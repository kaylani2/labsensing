/*
 * Author: Kaylani Bochie
 * 
 * Connects to Wi-Fi network, subscribes to a topic in a local MQTT broker and calls a function
 * when a new message is received. That's it.
 *
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

// Directives for Wi-Fi on the esp8266
#include <ESP8266WiFi.h>
#define NETWORK_SSID                              "NETWORK_SSID"
#define NETWORK_PASSWORD                          "NETWORK_PASSWORD"

// Directives for the MQQT client
#include <PubSubClient.h>
#define MQTT_SERVER                 "xxx.xxx.xxx.xxx"
#define MQTT_PORT                   1883 // standard port
#define MQTT_TOPIC                  "MQTT_TOPIC"
#define MQTT_USERNAME               "MQTT_USERNAME"
#define MQTT_PASSWORD               "MQTT_PASSWORD"
#define CLIENT_ID                   "could_be_anything"
#define MQTT_MESSAGE                "any_string"

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


/*
 * void handleStringPayload (String, byte *, unsigned int)
 * 
 * Arguments:
 * String - Topic
 * byte * - Payload
 * unsigned int - Payload length
 * 
 * Return values:
 * 
 * 
 * Description:
 * This function is used by .setCallback () and is called when a new message arrives
 * to a topic subscribed by the client.
 * 
 */
void handleStringPayload (String topic, byte *payload, unsigned int payloadLength)
{
  Serial.print ("Topic: ");
  Serial.println (topic);
  String stringPayload;

  // If payload is a string
  for (unsigned int payloadIndex = 0; payloadIndex < payloadLength; payloadIndex++)
    stringPayload += (char) payload [payloadIndex];


  Serial.print ("Payload: ");
  Serial.println (stringPayload);

  // Include any logic to handle payload
}


void setup ()
{
  delay (3000); // Included to wait for the serial monitor
  Serial.begin (SERIAL_BAUD_RATE);
  Serial.println ("Setup initiated.");
  pinMode (BUILT_IN_LED, OUTPUT);
  makeWifiConnection (NETWORK_SSID, NETWORK_PASSWORD);
  
  myClient.setCallback (handleStringPayload);
  
  if (myClient.connect (CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD))
    Serial.println ("Connected to MQTT Broker.");
  else
    Serial.println ("Connection to MQTT Broker failed!");
  
  Serial.println ("Setup finished.");
}

void loop ()
{
  if (!myClient.connected ())
  {
    myClient.connect (CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD);
    myClient.subscribe (MQTT_TOPIC);
  }
  myClient.loop ();
  // /\ This should be called regularly to allow the client to process incoming messages and
  // maintain its connection to the server. (Extracted from the official documentation)
  

  //digitalWrite (BUILT_IN_LED, !digitalRead (BUILT_IN_LED));
  //delay (5000); // Testing to see if the subscription is delay tolerant. Kind of, not much
}
