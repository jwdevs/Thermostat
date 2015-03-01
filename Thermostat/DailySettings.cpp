//
// DailySettings.cpp 
// C++ code
// ----------------------------------
// Developed with embedXcode 
// http://embedXcode.weebly.com
//
// Project 		BoilerController
//
// Created by 	Jakub Wozniak, 31/10/14 6:54 pm
// 				Jakub Wozniak
//
// Copyright	© Jakub Wozniak, 2014
// License   	<#license#>
//
// See 			DailySettings.h and ReadMe.txt for references
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
#include "DailySettings.h"
// Code

DailySettings::DailySettings() {
    Serial.println(F("DailySettings::DailySettings()"));
    defaultTemp = 17.5;
    setValues(settings[0], 6, 50, 8, 30, 19.5);
    setValues(settings[1], 12, 15, 13, 30, 19);
    setValues(settings[2], 17, 45, 21, 30, 19.5);
    setValues(settings[3], -1, -1,-1,-1,NAN);
    setValues(settings[4], -1, -1,-1,-1,NAN);
}

void DailySettings::setValues(TempRange& range, int hourFrom, int minuteFrom, int hourTo, int minuteTo, float temp) {
    range.hourFrom = hourFrom;
    range.hourTo = hourTo;
    range.minuteFrom = minuteFrom;
    range.minuteTo = minuteTo;
    range.temperature = temp;
}

void DailySettings::setValues(int rangeNumber, int hourFrom, int minuteFrom, int hourTo, int minuteTo, float temp) {
    if(rangeNumber >= 0 && rangeNumber < RANGES_SIZE) {
        setValues(settings[rangeNumber], hourFrom, minuteFrom, hourTo, minuteTo, temp);
    } else {
        Serial.println(F("Invalid range number"));
    }
}

void DailySettings::setDefaultTemp(float temp) {
    defaultTemp  = temp;
}


float DailySettings::getRefTemperature(byte hour, byte minute) {
    //Serial.print(F("Temp for: "));
    //Serial.print(hour);
    //Serial.print(F("h"));
    //Serial.println(minute);
    int time = hour * 60 + minute;
    
    for (byte i = 0; i < RANGES_SIZE; ++i) {
        if(settings[i].temperature!=NAN) {
            int timeFrom = settings[i].hourFrom * 60 + settings[i].minuteFrom;
            int timeTo =settings[i].hourTo * 60 + settings[i].minuteTo;
            if (time >= timeFrom && time < timeTo) {
                //printSerial(&settings[i]);
                return settings[i].temperature;
            }
        }
    }
    //Serial.println(F("Returing default temperature"));
    return defaultTemp;
}

void DailySettings::printSerial(TempRange * range) {
    Serial.print(range->hourFrom);
    Serial.print(F("h"));
    Serial.print(range->minuteFrom);
    Serial.print(F( "-"));
    Serial.print(range->hourTo);
    Serial.print(F("h"));
    Serial.print(range->minuteTo);
    Serial.print(F(" Temp: "));
    Serial.println(range->temperature);
}

void DailySettings::printSerial() {
    
    Serial.print(F("Default temp: "));
    Serial.println(defaultTemp);
    Serial.println(F("Ranges: "));
    for (byte i = 0; i < RANGES_SIZE; i++) {
        printSerial(&settings[i]);
    }
    
}
