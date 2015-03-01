//
//  Constants.h
//  ThermostatController
//
//  Created by Jakub Wozniak on 28/12/14.
//  Copyright (c) 2014 Jakub Wozniak. All rights reserved.
//

#ifndef ThermostatController_Constants_h
#define ThermostatController_Constants_h

#define WEEKLY_SETTINGS 1
//30 minutes
#define ON_OFF_FORCE_TIME 1800
//10 minutes
#define PRESENCE_RESET_TIME 600
//3 minutes
#define REMOTE_TEMP_TIMEOUT_SEC 180
//10 minutes
#define REMOTE_COMMAND_TIMEOUT_SEC 600
// 30 seconds local temp readout
#define LOCAL_TEMP_READING_TIME_SEC 30

#define MODE_PRESENCE 1
#define MODE_CALENDAR 2

#define START_NIGHT (23*60)
#define STOP_NIGHT (6*60)+30


#define MODE_BUTTON_PIN 2
#define FORCE_ON_BUTTON_PIN 3
#define FORCE_OFF_BUTTON_PIN 4


#define MOVE_SENSOR_PIN 10
#define MOVE_SENSOR_LED_PIN 9
#define THERMOSTAT_PIN 8 

//Sensor numbers
#define LOCAL_TEMP_SENSOR_NUMBER 0
#define LOCAL_HUM_SENSOR_NUMBER 0
#define LOCAL_THERMOSTAT_SWITCH_NUMBER 0
#define LOCAL_PRESENCE_SENSOR_NUMBER 0

//calibration correction with respect to the remote thermometer.
#define LOCAL_TEMP_CORRECTION 0.0

#define RANGES_SIZE 5

//For communication protocol

#define START '{'
#define END '}'
#define FS ';'

//Codes for the messages {CODE;MESSAGE} from the sensors
#define TEMPERATURE_MESSAGE 1
#define HUMIDITY_MESSAGE 2
#define PRESENCE_DETECTION_MESSAGE 3

#define THERMOSTAT_SWITCH_MESSAGE 10
//...Other sensors...

//Thermostat controller settings messages
#define TIME_MESSAGE 101
#define DAILY_RANGE_SETTING_MESSAGE 102
#define DAILY_DEFAULT_TEMP_MESSAGE 103
#define THERMOSTAT_ON_MESSAGE 104
#define THERMOSTAT_OFF_MESSAGE 105
#define THERMOSTAT_ON_OFF_RESET_MESSAGE 106
#define THERMOSTAT_MODE_PRESENCE_MESSAGE 107
#define THERMOSTAT_MODE_CALENDAR_MESSAGE 108
#define THERMOSTAT_PRESENCE_TEMPS_MESSAGE 109
#define REMOTE_PING_MESSAGE 110
#define STATE_MESSAGE 200


#define SEND_INTERVAL_SEC 30

#define INVALID_TEMP -999
//Eeprom addresses
#define BASE_ADDRESS (byte*)100
//For Grove DHT sensor
//#define DHTPIN A0 // what pin we're connected to
#define DHTPIN 14
// DHT 22 (AM2302)
#define DHTTYPE DHT22



#endif
