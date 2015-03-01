//
// File			DataModel.h
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
// Date			24/10/14 4:36 pm
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



#ifndef DataModel_h
#define DataModel_h

#include "RingBuffer.h"
#include "WeeklySettings.h"
#include "DailySettings.h"
#include "Constants.h"
#include "MovementListener.h"
#include "ThermostatStateListener.h"

/**
 * Contains the setting of desired temperatures thoughout the day.
 **/


class ControllerModel {
    WeeklySettings weeklySettings[WEEKLY_SETTINGS];
    RingBuffer<float, byte, 40> remoteTemperatureList;
    RingBuffer<float, byte, 40> localTemperatureList;
    MovementListener * movementListener;
    ThermostatStateListener * thermostatStateListener;
    
    bool thermostatOn{false};
    byte weeklySettingNumber{0};
    long timeThermostatForceCommand{0};
    byte mode{MODE_PRESENCE};
    
    bool localMovementDetector{false};
    
    float humidity{0.0};
    
    long lastRemoteTemperatureTime{0};
    long lastLocalTemperatureTime{0};
    
    long lastRemoteControllerPingTime{0};
    
    float comfortTemp{20};
    float lowTemp{17};
    float nightTemp{17};
    
    long lastPresenceDetectionTime{0};
    
    float getPresenceModeRefTemperature(byte day, byte hour, byte minute);
    float getCalendarModeRefTemperature(byte day, byte hour, byte minute);
    
    void addTemperatureReading(float temp, RingBuffer<float, byte, 40> * buffer);
    float getMeanTemperature(RingBuffer<float, byte, 40> * buffer);
    
    void setThermostatState(bool value);
public:
    ControllerModel();
    
    void setThermostatStateListener(ThermostatStateListener * listener);
    void setMovementListener(MovementListener * listener);
    
    void setLocalMovementDetectorState(bool value);
    bool getLocalMovementDetectorState();
    
    
    void  setPresenceTemperatures(float comfortTemp, float defaultTemp, float nightTemp);
    void  setWeeklySettingNumber(int number);
    void  setRangeValues(int weeklySettingsNumber, int dayNumber,  int rangeNum, TempRange & range);
    void  setDailyDefaultTemperature(int weeklySettingsNumber, int dayNumber,float temp);
    void  printSerial() ;

    void  addLocalTemperatureReading(float temperature);
    void  addRemoteTemperatureReading(float temperature);
    void  addLocalHumidityReading(float humidity);
    
    float getRefTemperature();
    float getLocalMeanTemperature();
    float getRemoteMeanTemperature();
    
    float getLastRemoteTemperatureReading();
    float getLastLocalTemperatureReading();
    float getLastLocalHumidityReading();
    
    void  setThermostatOn();
    void  setThermostatOff();
    bool  isThermostatOn();
    
    void  setThermostatOnForcedCommand();
    void  setThermostatOffForcedCommand();
    void  resetThermostatForcedCommand();
    bool  isThermostatForcedCommand();
    int   getRemainingForcedCommandSeconds();
    
    void resetRemoteCommandTimeout();
    
    bool isRemoteTemperatureTimeout();
    bool isLocalTemperatureTimeout();
    
    bool isRemoteCommandTimeout();
    
    void setPresenceDetected();
    bool isPresenceDetected();
    void setPresenceMode();
    
    void setCalendarMode();
    bool isCalendarMode();
    
    
    
};


#endif
