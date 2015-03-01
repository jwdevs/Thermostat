//
// File			CommunicationController.h
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
// Date			24/10/14 7:14 pm
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

#include <SoftwareSerial.h>
#include "RingBuffer.h"
#include "MainController.h"
#include "Events.h"
#include "Constants.h"
#ifndef CommunicationController_h
#define CommunicationController_h

#include "MovementListener.h"
#include "ThermostatStateListener.h"

class CommunicationController: public MovementListener, public ThermostatStateListener  {
    SoftwareSerial* xbee;
    MainController * mainController;
    bool error;
    long lastSendTime{0};
    
    bool previousMovementDetectorState{false};
    void setupXbee();
    void processMessage();
    void readStartToken();
    bool readField(char* inData);
    bool readIntField(int & val);
    bool readFloatField(float& val);
    bool readLongField(long& val);
    bool readRangeValues(AddDailyRangeSettingEvent& dailySetting);
    bool readRange(TemperatureRangeValue& range);
   
    //messages
    bool readTimeMessage();
    bool readTempSensorMessage();
    bool readDailyDefaultTemperatureSettingMessage();
    bool readDailyRangeSettingMessage();
    bool readThermostatPresenceTemperaturesMessage();
    bool readPresenceDetectedMessage();
    
    //messages triggered just by type
    
    void readThermostatOnMessage();
    void readThermostatOffMessage();
    void readThermostatOnOffResetMessage();
    
    void readThermostatInRemoteMessage();
    void readThermostatInLocalMessage();
    
    void readThermostatModePresence();
    void readThermostatModeCalendar();
    
    
    
    void readRemotePingMessage();
    
    
    void sendThermostatStateMessage(OnReadStateEvent event);
    void sendTemperatureMessage(OnReadStateEvent event);
    void sendHumidityMessage(OnReadStateEvent event);
    void sendPresenceDetectionMessage();
    void sendThermostatSwitchMessage(bool state);
public:
    CommunicationController(MainController* controller) ;
    void readMessages();
    void sendMessages();
    
    void onMovementDetected();
    void onThermostatStateChange(bool isOn);
};


#endif
