/*
 * Author: Kaylani Bochie
 *
 * Global functions for the NodeMCU board.
 *
 */

#include "globalFunctions.h"

// Other directives
#define OK                                      0

// Directives for the DHT22 sensor
#define ERROR_READING_TEMPERATURE               1
#define ERROR_READING_HUMIDITY                  2
#define ERROR_FINDING_TEMPERATURE                -1

void publishToTopic (PubSubClient myClient, const char *mqttTopic, const char *mqttMessage, const char *clientId, const char *mqttUsername, const char *mqttPassword)
{
  if (myClient.connected ())
  {
    myClient.publish (mqttTopic, mqttMessage);
    Serial.println ("Message sent.");
  }
  else
  {
    Serial.println ("Lost connection. Trying again.");
    myClient.connect (clientId, mqttUsername, mqttPassword);
    delay (10); // This delay ensures that myClient.publish doesn't clash with the myClient.connect call
    if (myClient.publish (mqttTopic, mqttMessage))
      Serial.println ("Connection recovered. Message sent.");
    else
      Serial.println ("Failed to send message");
  }
}


/*
 * void printSensorDetails (DHT_Unified);
 *
 * Parameters:
 * DHT_Unified - Object sensor from which the details will be extracted. (I)
 *
 * Return codes:
 * 
 * Description:
 * Prints sensor details from a given DHT_Unified object. 
 *
 */
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

/*
 * int getAndPrintAirTemperature (DHT_Unified, String &);
 *
 * Parameters:
 * DHT_Unified - Object sensor that will be used to read the temperature. (I)
 * String & - String reference that will be used to return the temperature. (O)
 *
 * Return codes:
 * OK - The function was executed correctly.
 * ERROR_READING_TEMPERATURE - Problem with the reading. The output (second parameter) is not reliable.
 *
 * Description:
 * Reads the temperature using a DHT_Unified object. Prints the temperature on Serial Monitor.
 * Returns the temperature as a string using the second parameter.
 */
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

/*
 * int getAirTemperature (DHT_Unified, String &);
 *
 * Parameters:
 * DHT_Unified - Object sensor that will be used to read the temperature. (I)
 * String & - String reference that will be used to return the temperature. (O)
 *
 * Return codes:
 * OK - The function was executed correctly.
 * ERROR_READING_TEMPERATURE - Problem with the reading. The output (second parameter) is not reliable.
 *
 * Description:
 * Reads the temperature using a DHT_Unified object.
 * Returns the temperature as a string using the second parameter.
 */
int getAirTemperature (DHT_Unified myDht, String &stringAirTemperature)
{
  sensors_event_t myEvent;  
  
  myDht.temperature().getEvent(&myEvent);
  if (isnan(myEvent.temperature))
    return ERROR_READING_TEMPERATURE;
  
  float floatAirTemperature = (float) myEvent.temperature;
  stringAirTemperature = String (floatAirTemperature, 3);
  return OK;
}

/*
 * int getAndPrintAirHumidity (DHT_Unified, String &);
 *
 * Parameters:
 * DHT_Unified - Object sensor that will be used to read the humidity. (I)
 * String & - String reference that will be used to return the humidity. (O)
 *
 * Return codes:
 * OK - The function was executed correctly.
 * ERROR_READING_HUMIDITY - Problem with the reading. The output (second parameter) is not reliable.
 *
 * Description:
 * Reads the humidity using a DHT_Unified object. Prints the humidity on Serial Monitor.
 * Returns the humidity as a string using the second parameter.
 */
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

/*
 * int getAirHumidity (DHT_Unified, String &);
 *
 * Parameters:
 * DHT_Unified - Object sensor that will be used to read the humidity. (I)
 * String & - String reference that will be used to return the humidity. (O)
 *
 * Return codes:
 * OK - The function was executed correctly.
 * ERROR_READING_HUMIDITY - Problem with the reading. The output (second parameter) is not reliable.
 *
 * Description:
 * Reads the humidity using a DHT_Unified object.
 * Returns the humidity as a string using the second parameter.
 */
int getAirHumidity (DHT_Unified myDht, String &stringAirHumidity)
{
  sensors_event_t myEvent;  

  myDht.humidity().getEvent(&myEvent);
  if (isnan(myEvent.relative_humidity))
    return ERROR_READING_HUMIDITY;
  
  float floatAirHumidity = (float) myEvent.relative_humidity;
  stringAirHumidity = String (floatAirHumidity, 3);
  return OK;
}

/*
 * int findAndPrintAirTemperature (DHT_Unified, String &);
 *
 * Parameters:
 * DHT_Unified - Object sensor that will be used to read the temperature. (I)
 * String & - String reference that will be used to return the temperature. (O)
 * const int - Previous temperature will be returned if something goes wrong. (I)
 *
 * Return codes:
 *
 * Description:
 * Reads the temperature using a DHT_Unified object. Prints the temperature on Serial Monitor.
 * Returns the temperature as a string using the second parameter.
 * Returns the temperature as an integer using the return.
 */
int findAndPrintAirTemperature (DHT_Unified myDht, String &stringAirTemperature, const int previousTemperature)
{
  sensors_event_t myEvent;  
  
  myDht.temperature().getEvent(&myEvent);
  if (isnan(myEvent.temperature))
  {
    Serial.println ("Error reading temperature!");
    return previousTemperature;
  }
  
  float floatAirTemperature = (float) myEvent.temperature;
  stringAirTemperature = String (floatAirTemperature, 3);
  Serial.print ("Temperature: ");
  Serial.print (stringAirTemperature);
  Serial.println (" *C");

  return (int) floatAirTemperature;
}

void blinkRapidly (unsigned int ledPin)
{
  unsigned int numberOfTimes = 10;
  for (unsigned int counter = 0; counter < numberOfTimes; counter++)
  {
    digitalWrite (ledPin, !digitalRead (ledPin));
    delay (50);
  }
}

/*
 * void dumpByteArray (byte *, byte);
 * 
 * Arguments:
 * byte * - Pointer to the byte array
 * byte - Length of the array
 * 
 * Description:
 * Prints a generic hex array on the serial monitor
 */
void dumpByteArray (byte *byteArray, unsigned int bufferSize)
{
  for (unsigned char index = 0; index < bufferSize; index++)
  {
    Serial.print (byteArray [index] < 0x10 ? " 0" : " ");
    Serial.print (byteArray [index], HEX);
  }
  Serial.println ();
}

/*
 * int compareByteArrays (byte *, byte *, unsigned int);
 * 
 * Arguments:
 * byte * - First byte array (I)
 * byte * - Second byte array (I)
 * unsigned int - Length of the arrays (I)
 * 
 * Description:
 * Compare each byte of the arrays. If they have the same content, returns OK. Othewise an error code is returned.
 * 
 * Return codes:
 * OK - The arrays are the same.
 * DIFFERENT_BYTE_ARRAY - The arrays are not the same.
 */
int compareByteArrays (byte *firstByteArray, byte *secondByteArray, unsigned int byteArrayLength)
{
  for (unsigned char index = 0; index < byteArrayLength; index++)
  {
    if (firstByteArray [index] != secondByteArray [index])
      return DIFFERENT_BYTE_ARRAYS;
  }

  return OK;
}

void byteArrayToString (byte *byteArray, unsigned int byteArrayLength, String &outputString)
{
  outputString = "";
  for (unsigned char index = 0; index < byteArrayLength; index++)
  {
     outputString.concat (String (byteArray [index] < 0x10 ? ":0" : ":"));
     outputString.concat (String (byteArray [index], HEX));
  }
  outputString [0] = ' ';
  outputString.trim ();
  outputString.toUpperCase ();
}
