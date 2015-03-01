//
// File			DisplayController.h
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
// Date			26/10/14 10:39 am
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

#include "rgb_lcd.h"
#include "DataModel.h"

#ifndef DisplayController_h
#define DisplayController_h

class DisplayController {
//private fields:
    rgb_lcd & lcd;
    ControllerModel * model;
    //How much time this screen is diplayed in seconds
    byte screenDisplayTime;
    byte currentScreen;
    unsigned long screenStartTime;
    
//private methods:
    void setupLcd();
    void printDigits(Print* printer, int digits);
    void digitalClockDisplay(Print * printer);
    void displayInfoOnLcd();
    
    void displayScreen0();
    void displayScreen1();
    void displayScreen2();
    
    
    void (DisplayController::*screens[3])();
    
public:
    DisplayController(ControllerModel * mptr, byte displayTime, rgb_lcd& ref);
    void display();
};

#endif
