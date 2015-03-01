//
// DataModel.cpp 
// C++ code
// ----------------------------------
// Developed with embedXcode 
// http://embedXcode.weebly.com
//
// Project 		ThermostatController
//
// Created by 	Jakub Wozniak, 24/10/14 4:38 pm
// 				Jakub Wozniak
//
// Copyright	© Jakub Wozniak, 2014
// License   	<#license#>
//
// See 			DataModel.h and ReadMe.txt for references
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


#include "DataModel.h"
#include "DailySettings.h"
#include "WeeklySettings.h"
#include "Time.h"
#include "RingBuffer.h"
#include <avr/eeprom.h>
#include "MovementListener.h"
#include "ThermostatStateListener.h"
// Code


ControllerModel::ControllerModel() {
    Serial.println(F("ControllerModel::ControllerModel()"));
    
    byte* adr = BASE_ADDRESS;
    byte isSet = eeprom_read_byte(adr++);
    if(isSet == 1) {
        Serial.println(F("Reading presence temperatures from eeprom"));
        float* adrFloat = (float*)adr;
        this->comfortTemp = eeprom_read_float(adrFloat++);
        this->nightTemp = eeprom_read_float(adrFloat++);
        this->lowTemp = eeprom_read_float(adrFloat);
    }
    
}

void ControllerModel::setMovementListener(MovementListener * listener) {
    this->movementListener = listener;
}

void ControllerModel::setThermostatStateListener(ThermostatStateListener * listener) {
    this->thermostatStateListener = listener;
}

void ControllerModel::setLocalMovementDetectorState(bool value) {
    bool lastState = localMovementDetector;
    this->localMovementDetector = value;
    if(lastState != localMovementDetector && localMovementDetector == true) {
        this->movementListener->onMovementDetected();
    }
}

bool ControllerModel::getLocalMovementDetectorState() {
    return this->localMovementDetector;
}

void ControllerModel::setPresenceTemperatures(float comfortTemp, float defaultTemp, float nightTemp) {
    this->comfortTemp = comfortTemp;
    this->nightTemp = nightTemp;
    this->lowTemp = defaultTemp;
    byte* adr = BASE_ADDRESS;
    eeprom_write_byte(adr++,1);
    float* adrFloat = (float*)adr;
    eeprom_write_float(adrFloat++,this->comfortTemp);
    eeprom_write_float(adrFloat++,this->nightTemp);
    eeprom_write_float(adrFloat,this->lowTemp);
}

void ControllerModel::setPresenceDetected(){
    lastPresenceDetectionTime = now();
};

void ControllerModel::setCalendarMode(){
    mode = MODE_CALENDAR;
};

void ControllerModel::setPresenceMode(){
    mode = MODE_PRESENCE;
};



bool ControllerModel::isCalendarMode() {
    return mode == MODE_CALENDAR;
}

bool ControllerModel::isRemoteCommandTimeout() {
    return now() - lastRemoteControllerPingTime > REMOTE_COMMAND_TIMEOUT_SEC;
}

void ControllerModel::resetRemoteCommandTimeout() {
    lastRemoteControllerPingTime = now();
}


bool ControllerModel::isRemoteTemperatureTimeout() {
    return now() - lastRemoteTemperatureTime > REMOTE_TEMP_TIMEOUT_SEC;
}

bool ControllerModel::isLocalTemperatureTimeout() {
    return now() - lastLocalTemperatureTime > LOCAL_TEMP_READING_TIME_SEC;
}

float ControllerModel::getPresenceModeRefTemperature(byte day, byte hour, byte minute) {
    long time = hour * 60 + minute;
    
    if( time > START_NIGHT || time < STOP_NIGHT ) {
        return nightTemp;
    } else if( isPresenceDetected() ) {
        //presence detected at the house
        return comfortTemp;
    } else {
        return lowTemp;
    }
}

float ControllerModel::getCalendarModeRefTemperature(byte day, byte hour, byte minute) {
    return this->weeklySettings[weeklySettingNumber].getDailySettings(day).getRefTemperature(hour, minute);
};

float ControllerModel::getRefTemperature() {
    //get time from TimeController
    byte h = hour();
    byte m = minute();
    //Time.h: Sunday is 1, Monday 2, etc.
    byte wday =  weekday();
    return mode==MODE_CALENDAR?getCalendarModeRefTemperature(wday, h, m):getPresenceModeRefTemperature(wday,h,m);
}

bool ControllerModel::isPresenceDetected() {
    return now() - lastPresenceDetectionTime < PRESENCE_RESET_TIME;
}


void ControllerModel::printSerial() {
    Serial.println(F(">>>>>>> Weekly settings"));
    for(byte i = 1; i <= 7; ++i) {
        Serial.print(F("--------- Setting for day: "));
        Serial.println(i);
        this->weeklySettings[weeklySettingNumber].getDailySettings(i).printSerial();
    }
}


void ControllerModel::addTemperatureReading(float temperature, RingBuffer<float, byte, 40> * buffer) {
    if(buffer->isFull()) {
        float temp = 0;
        buffer->poll(temp);
    }
    buffer->offer(temperature);
}

void ControllerModel::addLocalTemperatureReading(float temperature) {
    this->lastLocalTemperatureTime = now();
    addTemperatureReading(temperature, &this->localTemperatureList);
}

void ControllerModel::addRemoteTemperatureReading(float temperature) {
    this->lastRemoteTemperatureTime = now();
    addTemperatureReading(temperature, &this->remoteTemperatureList);
}

void  ControllerModel::setDailyDefaultTemperature(int weeklySettingsIndex, int dayNumber,float temp) {
    if (weeklySettingsIndex >= 0 && weeklySettingsIndex < WEEKLY_SETTINGS && dayNumber >= 1 && dayNumber <= 7) {
        this->weeklySettings[weeklySettingsIndex].getDailySettings(dayNumber).setDefaultTemp(temp);
    }
}


void ControllerModel::setRangeValues(int weeklySettingsIndex, int dayNumber, int rangeNum, TempRange & range){
    if (weeklySettingsIndex >= 0 && weeklySettingsIndex < WEEKLY_SETTINGS && dayNumber >= 1 && dayNumber <= 7 && rangeNum >= 0 && rangeNum < RANGES_SIZE) {
        this->weeklySettings[weeklySettingsIndex].getDailySettings(dayNumber).setValues(rangeNum,  range.hourFrom, range.minuteFrom, range.hourTo, range.minuteTo, range.temperature);
    } else {
        Serial.println(F("Invalid range settings"));
    }
    
}

void  ControllerModel::resetThermostatForcedCommand() {
    timeThermostatForceCommand = 0;
}

void  ControllerModel::setThermostatOnForcedCommand() {
    timeThermostatForceCommand = now();
    this->setThermostatOn();
};

void  ControllerModel::setThermostatOffForcedCommand() {
    timeThermostatForceCommand = now();
    this->setThermostatOff();
};



int ControllerModel::getRemainingForcedCommandSeconds() {
    if(!isThermostatForcedCommand()) {
        return 0;
    } else {
        return ON_OFF_FORCE_TIME - (now() - timeThermostatForceCommand);
    }
}


bool ControllerModel::isThermostatForcedCommand() {
    return (now() - timeThermostatForceCommand) < ON_OFF_FORCE_TIME;
}

float ControllerModel::getLastLocalHumidityReading() {
    return humidity;
}

void ControllerModel::addLocalHumidityReading(float h) {
    this->humidity = h;
}

float ControllerModel::getMeanTemperature(RingBuffer<float, byte, 40> * buffer) {
    float sum = 0;
    if( !buffer->isEmpty() ) {
        for (byte i = 0; i < buffer->size(); ++i){
            sum += (*buffer)[i];
        }
        return sum / buffer->size();
    } else {
        return NAN;
    }
}

float ControllerModel::getLocalMeanTemperature() {
    return getMeanTemperature(&this->localTemperatureList);
}

float ControllerModel::getRemoteMeanTemperature() {
    return getMeanTemperature(&this->remoteTemperatureList);
}

float ControllerModel::getLastLocalTemperatureReading() {
    if(localTemperatureList.size() > 0) {
        return localTemperatureList[localTemperatureList.size()-1];
    }
    return NAN;
}

float ControllerModel::getLastRemoteTemperatureReading() {
    if( remoteTemperatureList.size() > 0 ){
        return remoteTemperatureList[remoteTemperatureList.size()-1];
    }
    return NAN;
}



void ControllerModel::setThermostatState(bool value) {
    bool lastState = thermostatOn;
    thermostatOn = value;
    if(lastState != thermostatOn) {
        this->thermostatStateListener->onThermostatStateChange(thermostatOn);
    }
}

void ControllerModel::setThermostatOn(){
    setThermostatState(true);
}

void ControllerModel::setThermostatOff(){
    setThermostatState(false);
}

bool ControllerModel::isThermostatOn() {
    return this->thermostatOn;
}
