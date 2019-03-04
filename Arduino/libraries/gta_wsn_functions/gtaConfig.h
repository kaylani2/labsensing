/*
 * Author: Kaylani Bochie
 *
 */

#ifndef GTA_CONFIG_H
#define GTA_CONFIG_H

#define EVENT_DELAY                                2000 //ms
#define TEMPERATURE_DIFFERENCE_THRESHOLD           1 // degree

// Directives for the NodeMCU board
#define D0                                         16
#define D1                                         5
#define D2                                         4
#define D3                                         0
#define D4                                         2
#define D5                                         14
#define D6                                         12
#define D7                                         13
#define D8                                         15
#define D9                                         3
#define D10                                        1
#define BUILT_IN_LED                               D4

#define DHT_ERROR_STRING                           "0"


// Easily change the address of the server and network use by each node
#define MQTT_SERVER                                "192.168.25.2"
#define MQTT_PORT                                  1883 // standard port
#define NETWORK_SSID                               "NETWORK_SSID"
#define NETWORK_PASSWORD                           "NETWORK_PASSWORD"
#define MQTT_USERNAME                              "USERNAME"
#define MQTT_PASSWORD                              "PASSWORD"
#define MQTT_DELAY                                 2000 // ms

// node_av1
#define AV1_MQTT_TEMPERATURE_TOPIC                 "gta/av1/temp"
#define AV1_MQTT_HUMIDITY_TOPIC                    "gta/av1/hum"
#define AV1_MQTT_DOOR_TOPIC                        "gta/av1/door"
#define AV1_MQTT_FIRE_TOPIC                        "gta/av1/fire"
#define AV1_MQTT_INFLUX_TOPIC                      "gta/av1/influx"
#define AV1_CLIENT_ID                              "gta-av1"

// node_av2
#define AV2_MQTT_TEMPERATURE_TOPIC                 "gta/av2/temp"
#define AV2_MQTT_HUMIDITY_TOPIC                    "gta/av2/hum"
#define AV2_MQTT_DOOR_TOPIC                        "gta/av2/door"
#define AV2_MQTT_RFID_TOPIC                        "gta/av2/rfid"
#define AV2_MQTT_INFLUX_TOPIC                      "gta/av2/influx"
#define AV2_CLIENT_ID                              "gta-av2"

// node_av2_asynchronous
#define AV2_ASYNC_MQTT_TEMPERATURE_TOPIC           "gta/av2async/temp"
#define AV2_ASYNC_MQTT_HUMIDITY_TOPIC              "gta/av2async/hum"
#define AV2_ASYNC_MQTT_DOOR_TOPIC                  "gta/av2async/door"
#define AV2_ASYNC_MQTT_RFID_TOPIC                  "gta/av2async/rfid"
#define AV2_ASYNC_MQTT_INFLUX_TOPIC                "gta/av2async/influx"
#define AV2_ASYNC_CLIENT_ID                        "gta-av2async"

// node_meet
#define MEET_MQTT_TEMPERATURE_TOPIC                "gta/meet/temp"
#define MEET_MQTT_HUMIDITY_TOPIC                   "gta/meet/hum"
#define MEET_MQTT_MOVEMENT_TOPIC                   "gta/meet/mov"
#define MEET_MQTT_INFLUX_TOPIC                     "gta/meet/influx"
#define MEET_CLIENT_ID                             "gta-meet"

// node_mult
#define MULT_MQTT_TEMPERATURE_TOPIC                "gta/mult/temp"
#define MULT_MQTT_HUMIDITY_TOPIC                   "gta/mult/hum"
#define MULT_MQTT_MOVEMENT_TOPIC                   "gta/mult/mov"
#define MULT_MQTT_INFLUX_TOPIC                     "gta/mult/influx"
#define MULT_CLIENT_ID                             "gta-mult"

// node_prof
#define prof_MQTT_TEMPERATURE_TOPIC                "gta/prof/temp"
#define prof_MQTT_HUMIDITY_TOPIC                   "gta/prof/hum"
#define prof_MQTT_MOVEMENT_TOPIC                   "gta/prof/mov"
#define prof_MQTT_INFLUX_TOPIC                     "gta/prof/influx"
#define prof_CLIENT_ID                             "gta-prof"

#endif
