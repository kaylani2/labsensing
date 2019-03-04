/*
 * Author: Kaylani Bochie
 *
 */

#ifndef GLOBAL_FUNCTIONS_H
#define GLOBAL_FUNCTIONS_H


// Directives for the DHT22 sensor
#include <Adafruit_Sensor.h>
#include <DHT_U.h>
#include <DHT.h>

// Directives for the MQTT client
#include <PubSubClient.h>

// Directives for the MFRC522 RFID Reader
#include <SPI.h>
#include <MFRC522.h>
#define DIFFERENT_BYTE_ARRAYS                       1



void publishToTopic (PubSubClient, const char *, const char *, const char *, const char *, const char *);

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
void printSensorDetails (DHT_Unified);

/*
 * int getAndPrintAirTemperature (DHT_Unified, String &);
 *
 * Parameters:
 * DHT_Unified - Object sensor that will be used to read the temperature. (I)
 * String & - String reference that will be used to return the temperature. (O)
 *
 * Description:
 * Reads the temperature using a DHT_Unified object. Prints the temperature on Serial Monitor.
 * Returns the temperature as a string using the second parameter.
 */
int getAndPrintAirTemperature (DHT_Unified, String &);

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
int getAirTemperature (DHT_Unified, String &);

/*
 * int getAndPrintAirHumidity (DHT_Unified, String &);
 *
 * Parameters:
 * DHT_Unified - Object sensor that will be used to read the humidity. (I)
 * String & - String reference that will be used to return the humidity. (O)
 *
 * Description:
 * Reads the humidity using a DHT_Unified object. Prints the humidity on Serial Monitor.
 * Returns the humidity as a string using the second parameter.
 */
int getAndPrintAirHumidity (DHT_Unified, String &);

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
int getAirHumidity (DHT_Unified, String &);

/*
 * int findAndPrintAirTemperature (DHT_Unified, String &);
 *
 * Parameters:
 * DHT_Unified - Object sensor that will be used to read the temperature. (I)
 * String & - String reference that will be used to return the temperature. (O)
 *
 * Return codes:
 * const int - Previous temperature will be returned if something goes wrong. (I)
 *
 * Description:
 * Reads the temperature using a DHT_Unified object. Prints the temperature on Serial Monitor.
 * Returns the temperature as a string using the second parameter.
 * Returns the temperature as an integer using the return.
 */
int findAndPrintAirTemperature (DHT_Unified, String &, const int);


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
void dumpByteArray (byte *, unsigned int);

/*
 * int compareByteArrays (byte *, byte *, unsigned int);
 * 
 * Arguments:
 * byte * - First byte array
 * byte * - Second byte array
 * unsigned int - Length of the arrays
 * 
 * Description:
 * Compare each byte of the arrays. If they have the same content, returns OK. Othewise an error code is returned.
 * 
 * Return codes:
 * OK - The arrays are the same.
 * DIFFERENT_BYTE_ARRAY - The arrays are not the same.
 */
int compareByteArrays (byte *, byte *, unsigned int);

void byteArrayToString (byte *, unsigned int, String &);

void blinkRapidly (unsigned int);

#endif
