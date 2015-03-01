//
// File			TempRange.h
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
// Date			01/11/14 9:55 am
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

#ifndef TempRange_h
#define TempRange_h

class TempRange {
public:
    byte hourFrom;
    byte minuteFrom;
    byte hourTo;
    byte minuteTo ;
    float temperature;
    
    TempRange(): hourFrom(0),minuteFrom(0),hourTo(0),minuteTo(0),temperature(20.0){
        Serial.println(F("TempRange::TempRange()"));
    }
    
    TempRange(int hf, int mf, int ht, int mt, float temp) {
        hourFrom = hf;
        minuteFrom = mf;
        hourTo = ht;
        minuteTo = mt;
        temperature = temp;
    }
};

#endif
