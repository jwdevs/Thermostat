//
// CommunicationController.cpp 
// C++ code
// ----------------------------------
// Developed with embedXcode 
// http://embedXcode.weebly.com
//
// Project 		ThermostatController
//
// Created by 	Jakub Wozniak, 24/10/14 7:15 pm
// 				Jakub Wozniak
//
// Copyright	© Jakub Wozniak, 2014
// License   	<#license#>
//
// See 			CommunicationController.h and ReadMe.txt for references
//


// Core library - IDE-based
#if defined(WIRING) // Wiring specific
#include "Wiring.h"
#elif defined(MAPLE_IDE) // Maple specific
#include "WProgram.h"
#elif defined(MPIDE) // chipKIT specific
#include "WProgram.h"
#elif defined(DIGISPARK) // Digispark specific
#include "Arduino.h"
#elif defined(ENERGIA) // LaunchPad MSP430 G2 and F5529, Stellaris and Tiva, Experimeter Board FR5739 specific
#include "Energia.h"
#elif defined(MICRODUINO) // Microduino specific
#include "Arduino.h"
#elif defined(TEENSYDUINO) // Teensy specific
#include "Arduino.h"
#elif defined(ARDUINO) // Arduino 1.0 and 1.5 specific
#include "Arduino.h"
#else // error
#error Platform not defined
#endif // end IDE

#include "CommunicationController.h"
#include "DailySettings.h"
#include <stdlib.h>
#include <Time.h>
// Code



CommunicationController::CommunicationController(MainController* controller) {
    this->xbee = new SoftwareSerial(6, 7); // RX, TX
    this->mainController = controller;
    setupXbee();
    error = false;
}

void CommunicationController::setupXbee() {
    xbee->begin(9600);
}





void CommunicationController::readStartToken() {
    if(error) {
        return;
    }
    char c = '\0';
    while(xbee->available()) {
        c = xbee->read();
        if(c==START) {
            return;
        }
    };
    error = true;
}



bool CommunicationController::readIntField(int & ret) {
    char inData[10];
    //Serial.println(F("Int field"));
    //Serial.println(error);
    bool isEnd = readField(&inData[0]);
    //Serial.println(error);
    //delay(2000);
    if(!error) {
        ret = atoi(inData);
    }
    return isEnd;
}

bool CommunicationController::readLongField(long & ret) {
    char inData[20];
    bool isEnd = readField(&inData[0]);
    if(!error) {
        ret = atol(inData);
    }
    return isEnd;
}

bool CommunicationController::readFloatField(float & ret) {
    char inData[20];
    bool isEnd = readField(&inData[0]);
    if(!error) {
        ret = atof(inData);
        //Serial.print(F("Float value:"));
        //Serial.println(ret);
    }
    return isEnd;
}

bool CommunicationController::readField(char* inData) {
    if(error) {
        return false;
    }
    
    byte index = 0;
    char c = '\0';
    inData[0] = c;
    
    while(xbee->available()) {
        c = xbee->read();
        if(c != FS && c != END && c!=START) {
            inData[index++] = c;
            inData[index] = '\0';
        } else {
            break;
        }
    }
    //Serial.print(F("Field:"));
    //Serial.println(inData);
    //Serial.println(c);
    if(c != FS && c != END) {
        error = true;
        Serial.println(F("Error reading field"));
        Serial.println(inData);
    }
    
    if(c==END) {
        return true;
    } else {
        return false;
    }
}


/*
 
 struct OnReadStateEvent {
 bool on;
 bool remoteMode;
 bool forced;
 float refTemp;
 float lastTemp;
 float meanTemp;
 long time;
 };
 
 */
void CommunicationController::onMovementDetected() {
    sendPresenceDetectionMessage();
}

void CommunicationController::onThermostatStateChange(bool isOn) {
    sendThermostatSwitchMessage(isOn);
}


void CommunicationController::sendMessages() {
    long time = now();
    OnReadStateEvent state = this->mainController->readState();
    
    if(time - lastSendTime > SEND_INTERVAL_SEC) {
        Serial.println(F("Sending state"));
        sendThermostatStateMessage(state);
        sendTemperatureMessage(state);
        sendHumidityMessage(state);
        sendThermostatSwitchMessage(state.on);
        lastSendTime = time;
    }
}

void CommunicationController::sendPresenceDetectionMessage() {
    xbee->print(START);
    xbee->print(PRESENCE_DETECTION_MESSAGE);
    xbee->print(FS);
    xbee->print(LOCAL_PRESENCE_SENSOR_NUMBER);
    xbee->print(END);
}

void CommunicationController::sendThermostatSwitchMessage(bool state) {
    xbee->print(START);
    xbee->print(THERMOSTAT_SWITCH_MESSAGE);
    xbee->print(FS);
    xbee->print(LOCAL_THERMOSTAT_SWITCH_NUMBER);
    xbee->print(FS);
    xbee->print(state);
    xbee->print(END);
}

void CommunicationController::sendTemperatureMessage(OnReadStateEvent state) {
    xbee->print(START);
    xbee->print(TEMPERATURE_MESSAGE);
    xbee->print(FS);
    xbee->print(LOCAL_TEMP_SENSOR_NUMBER);
    xbee->print(FS);
    
    xbee->print(state.lastLocalTemp);
    xbee->print(END);
}

void CommunicationController::sendThermostatStateMessage(OnReadStateEvent state) {
    xbee->print(START);
    xbee->print(STATE_MESSAGE);
    xbee->print(FS);
    xbee->print(state.on);
    xbee->print(FS);
    xbee->print(state.forced);
    xbee->print(FS);
    xbee->print(state.presenceMode);
    xbee->print(FS);
    xbee->print(state.presenceDetected);
    xbee->print(FS);
    xbee->print(state.remoteTimeout);
    xbee->print(FS);
    xbee->print(state.tempTimeout);
    xbee->print(FS);
    xbee->print(state.refTemp);
    xbee->print(FS);
    xbee->print(state.lastRemoteTemp);
    xbee->print(FS);
    xbee->print(state.meanRemoteTemp);
    xbee->print(FS);
    xbee->print(state.lastLocalTemp);
    xbee->print(FS);
    xbee->print(state.meanLocalTemp);
    xbee->print(FS);
    
    xbee->print(state.time);
    xbee->print(END);

}

void CommunicationController::sendHumidityMessage(OnReadStateEvent state) {
    xbee->print(START);
    xbee->print(HUMIDITY_MESSAGE);
    xbee->print(FS);
    xbee->print(LOCAL_HUM_SENSOR_NUMBER);
    xbee->print(FS);
    xbee->print(state.lastLocalHumidity);
    xbee->print(END);
}

void CommunicationController::readMessages() {
    
    while(xbee->available()) {
        error = false;
        readStartToken();
        if(!error) {
            int messageType = 0;
            bool isEnd = false;
            isEnd = readIntField(messageType);
            if(error) {
                Serial.println(F("Error reading message type"));
                continue;
            }
            //Serial.print(F("Message type: "));
            //Serial.println(messageType);
            
            //delay(2000);
            switch (messageType) {
                case TEMPERATURE_MESSAGE:
                    isEnd = readTempSensorMessage();
                    break;
                case TIME_MESSAGE:
                    isEnd = readTimeMessage();
                    break;
                case DAILY_RANGE_SETTING_MESSAGE:
                    isEnd = readDailyRangeSettingMessage();
                    break;
                case DAILY_DEFAULT_TEMP_MESSAGE:
                    isEnd = readDailyDefaultTemperatureSettingMessage();
                    break;
                case THERMOSTAT_ON_MESSAGE:
                    readThermostatOnMessage();
                    break;
                case THERMOSTAT_OFF_MESSAGE:
                    readThermostatOffMessage();
                    break;
                case THERMOSTAT_ON_OFF_RESET_MESSAGE:
                    readThermostatOnOffResetMessage();
                    break;
                case THERMOSTAT_MODE_PRESENCE_MESSAGE:
                    readThermostatModePresence();
                    break;
                case THERMOSTAT_MODE_CALENDAR_MESSAGE:
                    readThermostatModeCalendar();
                    break;
                case THERMOSTAT_PRESENCE_TEMPS_MESSAGE:
                    isEnd = readThermostatPresenceTemperaturesMessage();
                    break;
                case PRESENCE_DETECTION_MESSAGE:
                    isEnd = readPresenceDetectedMessage();
                    break;
                case REMOTE_PING_MESSAGE:
                    readRemotePingMessage();
                    break;
                default:
                    Serial.print(F("Unknown message type: "));
                    Serial.println(messageType);
                    break;
            }
            if (!isEnd || error) {
                Serial.print(F("Error reading message: "));
                Serial.println(messageType);
            }
        } else {
            Serial.println(F("Error reading start of message"));
        }
    }
    //delay(2000);
}

/*
 {3;<PIR NR>}
 If received it means that the movement has been detected by the PIR detector.
*/
bool CommunicationController::readPresenceDetectedMessage() {
    int presenceDetectorNumber = 0;
    bool isEnd = readIntField(presenceDetectorNumber);
    if(isEnd && !error) {
        Serial.print(F("Received presence detection event from detector:"));
        Serial.println(presenceDetectorNumber);
        mainController->onPresenceDetectedEvent();
    }else {
       Serial.println(F("Error reading presence detected event"));
    }
    return isEnd;
}

void CommunicationController::readThermostatModePresence() {
    if(!error){
        mainController->onThermostatModePresenceEvent();
    } else {
        Serial.println(F("Error reading Thermostat mode presence event"));
    }
}
void CommunicationController::readThermostatModeCalendar(){
    if(!error){
        mainController->onThermostatModeCalendarEvent();
    } else {
        Serial.println(F("Error reading Thermostat mode calendar event"));
    }
}

void CommunicationController::readRemotePingMessage() {
    if(!error){
        mainController->onRemotePingEvent();
    } else {
        Serial.println(F("Error reading remote ping message"));
    }
}

void CommunicationController::readThermostatOnOffResetMessage() {
    if(!error){
        mainController->onThermostatOnOffResetEvent();
    } else {
        Serial.println(F("Error reading Thermostat on/off reset message"));
    }
}

void CommunicationController::readThermostatOnMessage() {
    if(!error){
        mainController->onThermostatOnEvent();
    } else {
        Serial.println(F("Error reading Thermostat on message"));
    }
}

void CommunicationController::readThermostatOffMessage(){
    if(!error){
        mainController->onThermostatOffEvent();
    } else {
        Serial.println(F("Error reading Thermostat off message"));
    }
};

/*
 Example (spaces for clarity):
 {4;0;7;17.5};
 */
bool CommunicationController::readDailyDefaultTemperatureSettingMessage() {
    AddDailyDefaultTemperatureEvent dailySetting;
    
    readIntField(dailySetting.setNumber);
    //Serial.println(dailySetting.setNumber);
    
    readIntField(dailySetting.weekDay);
    //Serial.println(dailySetting.weekDay);
    
    bool isEnd = readFloatField(dailySetting.temperature);
    if(isEnd && !error){
        mainController->onDailySettingDefaultTemperatureEvent(dailySetting);
    } else {
        Serial.println(F("Error reading daily default temp setting message"));
    }
    return isEnd;

}

/*
  Example (spaces for clarity):
 {3;1;7;1; 7;30; 11;0; 20.5}
 */
bool CommunicationController::readDailyRangeSettingMessage() {
    AddDailyRangeSettingEvent dailySetting;
    
    readIntField(dailySetting.setNumber);
    //Serial.println(dailySetting.setNumber);
    
    readIntField(dailySetting.weekDay);
    //Serial.println(dailySetting.weekDay);
    
    readIntField(dailySetting.rangeIndex);

    
    bool isEnd = readRangeValues(dailySetting);
    if(isEnd && !error) {
        mainController->onDailyRangeSettingEvent(dailySetting);
    } else {
        Serial.println(F("Error reading daily range setting message"));
    }
    return isEnd;
    
}
/*
 
 {12; comfort; default; night}
 */
bool CommunicationController::readThermostatPresenceTemperaturesMessage() {
    float comfortTemp, defaultTemp, nightTemp;
    readFloatField(comfortTemp);
    readFloatField(defaultTemp);
    
    bool isEnd = readFloatField(nightTemp);
    if(isEnd && !error) {
        mainController->onThermostatPresenceTemperaturesEvent(comfortTemp, defaultTemp, nightTemp);
    } else {
        Serial.println(F("Error reading presence temperatures setting message"));
    }
    return isEnd;
    

}

bool CommunicationController::readRangeValues(AddDailyRangeSettingEvent& dailySetting) {
    if(error) {
        return false;
    }
    
    bool isEnd = false;
    if(xbee->available()) {
        isEnd=readRange(dailySetting.range);
    }
    return isEnd;
}

bool CommunicationController::readRange(TemperatureRangeValue& range) {
    readIntField(range.hourFrom);
    readIntField(range.minuteFrom);
    readIntField(range.hourTo);
    readIntField(range.minuteTo);
    return readFloatField(range.temperature);
}


bool CommunicationController::readTimeMessage() {
    long time = 0;
    bool isEnd = readLongField(time);
    if(isEnd && !error) {
        Serial.println(F("Received time message"));
        Serial.println(time);
        mainController->onTimeEvent(time);
    } else {
        Serial.println(F("Error reading time message"));
    }
    return isEnd;
}

bool CommunicationController::readTempSensorMessage() {
    AddTemperatureReadingEvent event;
    readIntField(event.thermometerNumber);
    bool isEnd = readFloatField(event.value);
    if(isEnd && !error) {
        Serial.println(F("Received temperature message"));
        Serial.println(event.value);
        mainController->onTemperatureEvent(event);
    }else {
        Serial.println(F("Error reading temperature message"));
    }
    return isEnd;
}



