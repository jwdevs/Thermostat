//
// File			DailySettings.h
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
// Date			31/10/14 6:54 pm
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

#include "TempRange.h"
#include "Constants.h"
#ifndef DailySettings_h
#define DailySettings_h



class DailySettings {
    //if nothing else is set;
    float defaultTemp;
    TempRange settings[RANGES_SIZE];
    
    void setValues(TempRange& range, int hourFrom, int minuteFrom, int hourTo, int minuteTo, float temp);
    
public:
    DailySettings();
    
    
    void setDefaultTemp(float temp) ;
    float getRefTemperature(byte hour, byte minute) ;
    void setValues(int rangeNumber, int hourFrom, int minuteFrom, int hourTo, int minuteTo, float temp);
    void printSerial(TempRange * range) ;
    void printSerial() ;
};



#endif
