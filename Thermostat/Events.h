//
// File			AddTemperatureReadingEvent.h
// Header
//
// Details		<#details#>
//	
// Project		 BoilerController
// Developed with [embedXcode](http://embedXcode.weebly.com)
// 
// Author		Jakub Wozniak
// 				Jakub Wozniak
//
// Date			29/10/14 9:41 pm
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

#include "DailySettings.h"
#ifndef AddTemperatureReadingEvent_h
#define AddTemperatureReadingEvent_h




struct AddTemperatureReadingEvent {
    int thermometerNumber;
    float value;
};

struct TemperatureRangeValue {
    int hourFrom;
    int minuteFrom;
    int hourTo;
    int minuteTo;
    float temperature;
};

struct AddDailyRangeSettingEvent {
    int setNumber;
    int weekDay;
    int rangeIndex;
    TemperatureRangeValue range;
};

struct AddDailyDefaultTemperatureEvent {
    int setNumber;
    int weekDay;
    float temperature;
};

struct OnReadStateEvent {
    bool on;
    bool forced;
    bool remoteTimeout;
    bool tempTimeout;
    bool presenceMode;
    float refTemp;
    float lastLocalTemp;
    float meanLocalTemp;
    float lastRemoteTemp;
    float meanRemoteTemp;
    float lastLocalHumidity;
    bool localMovementDetectorState;
    bool presenceDetected;
    long time;
};


#endif
