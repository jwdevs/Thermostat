//
// WeeklySettings.cpp 
// C++ code
// ----------------------------------
// Developed with embedXcode 
// http://embedXcode.weebly.com
//
// Project 		BoilerController
//
// Created by 	Jakub Wozniak, 31/10/14 6:53 pm
// 				Jakub Wozniak
//
// Copyright	© Jakub Wozniak, 2014
// License   	<#license#>
//
// See 			WeeklySettings.h and ReadMe.txt for references
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
#include "WeeklySettings.h"
// Code
//day from 1 (Sunday) to 7 (Saturday)
DailySettings& WeeklySettings::getDailySettings(int day) {
    return days[day-1];
}
