//
// File			ThermostatController.h
// Header
//
// Details		<#details#>
//	
// Project		 ThermostatController
// Developed with [embedXcode](http://embedXcode.weebly.com)
// 
// Author		Jakub Wozniak
// 				Jakub Wozniak
//
// Date			24/10/14 4:45 pm
// Version		<#version#>
// 
// Copyright	© Jakub Wozniak, 2014
// License    <#license#>
//
// See			ReadMe.txt for references
//


// Core library for code-sense - IDE-based
#if defined(WIRING) // Wiring specific
#include "Wiring.h"
#elif defined(MAPLE_IDE) // Maple specific
#include "WProgram.h"
#elif defined(MPIDE) // chipKIT specific
#include "WProgram.h"
#elif defined(DIGISPARK) // Digispark specific
#include "Arduino.h"
#elif defined(ENERGIA) // LaunchPad specific
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
#include "Events.h"
#include "buttons.h"

#include "DHT.h"


#ifndef MainController_h
#define MainController_h


class MainController {
    ControllerModel* model;
    //Temperature & humidity sensor
    //DHT dht(DHTPIN, DHTTYPE);
    DHT* dht;
    //buttons
    Button* modeButton;
    Button* forceOnButton;
    Button* forceOffButton;
    
    float readInternalTemperature();
    void readDHTSensorTemperatureAndHumidity();
    Button* createButton(byte pin);
    void checkForceOnButton();
    void checkForceOffButton();
    void checkModeButton();
    void checkMoveSensor();
    void ThermostatOffCommand();
    void ThermostatOnCommand();
public:
    MainController(ControllerModel* modelPtr);
        /*
     Controls the Thermostat using the model
     */
    void controlThermostat();
    void processSensors();
    
    void onTimeEvent(long time);
    void onTemperatureEvent(AddTemperatureReadingEvent& event);
    void onDailyRangeSettingEvent(AddDailyRangeSettingEvent& event);
    void onDailySettingDefaultTemperatureEvent(AddDailyDefaultTemperatureEvent& event);
    void onThermostatOnEvent();
    void onThermostatOffEvent();
    void onThermostatOnOffResetEvent();
    void onThermostatInRemoteEvent();
    void onThermostatInLocalEvent();
    void onThermostatModePresenceEvent();
    void onThermostatModeCalendarEvent();
    void onThermostatPresenceTemperaturesEvent(float comfortTemp, float defaultTemp, float nightTemp);
    void onPresenceDetectedEvent();
    void onRemotePingEvent();
    
    OnReadStateEvent readState();
    
    
};


#endif
