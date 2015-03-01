//
// DisplayController.cpp
// C++ code
// ----------------------------------
// Developed with embedXcode
// http://embedXcode.weebly.com
//
// Project 		ThermostatController
//
// Created by 	Jakub Wozniak, 26/10/14 10:39 am
// 				Jakub Wozniak
//
// Copyright	© Jakub Wozniak, 2014
// License   	<#license#>
//
// See 			DisplayController.h and ReadMe.txt for references
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

#include "DisplayController.h"
#include "Time.h"
// Code

DisplayController::DisplayController (ControllerModel * mptr, byte displayTime, rgb_lcd& ref): lcd(ref) {
    
    setupLcd();
    model = mptr;
    screenDisplayTime = displayTime;
    screenStartTime = now();
    
    
    screens[0] = &DisplayController::displayScreen0;
    screens[1] = &DisplayController::displayScreen1;
    screens[2] = &DisplayController::displayScreen2;
    //How much time this screen is diplayed in seconds
    currentScreen = 0;
    
}

// set up the LCD's number of columns and rows:
void DisplayController::setupLcd() {
    lcd.begin(16, 2);
    lcd.setRGB(0, 255, 0);
    lcd.noBlink();
}




void DisplayController::printDigits(Print* printer, int digits){
    // utility function for clock display leading 0
    if(digits < 10)
        printer->print('0');
    printer->print(digits);
}

void DisplayController::digitalClockDisplay(Print * printer){
    // digital clock display of the time
    printDigits(printer,hour());
    printer->print(F(":"));
    printDigits(printer,minute());
    printer->print(F(":"));
    printDigits(printer,second());
    lcd.setCursor(0, 1);
    printDigits(printer,day());
    printer->print(F("/"));
    printDigits(printer,month());
    printer->print(F("/"));
    printer->print(year());
}

void DisplayController::displayScreen0() {
    
    lcd.setCursor(0, 0);
    //Serial.println(F(temperature);
    lcd.print(F("M:"));
    
    if(model->isRemoteTemperatureTimeout()) {
        lcd.print(model->getLocalMeanTemperature());
    } else {
        lcd.print(model->getRemoteMeanTemperature());
    }
    lcd.print(F(" TR:"));
    
    
    if(model->isRemoteTemperatureTimeout()) {
        lcd.print(F("N/A"));
    }else {
        lcd.print(model->getLastRemoteTemperatureReading());
    }
    lcd.setCursor(0, 1);
    
    lcd.print(F("R:"));
    lcd.print(model->getRefTemperature());
    
    lcd.print(F(" TL:"));
    lcd.print(model->getLastLocalTemperatureReading());
    
}

//int idx = 0;
//void DisplayController::displayScreen0() {
//    lcd.setCursor(0, 0);
//    lcd.print(F("Test1"));
//    lcd.setCursor(0, 1);
//    lcd.print(F("Test2 "));
//    lcd.print(idx++);
//}

void DisplayController::displayScreen1(){
    lcd.setCursor(0, 0);
    if(timeStatus()!= timeNotSet){
        digitalClockDisplay(&lcd);
    } else {
        lcd.print(F("Time not set"));
    }
}

void DisplayController::displayScreen2(){
    lcd.setCursor(0, 0);
    if(model->isCalendarMode()){
        lcd.print(F("CMode"));
    } else {
        lcd.print(F("PMode:"));
        lcd.print(model->isPresenceDetected());
    }
    if(model->isRemoteCommandTimeout()) {
        lcd.print(F(" COM-ERR"));
    }
    if(model->isThermostatForcedCommand()) {
        lcd.setCursor(0, 1);
        lcd.print(F("Forced: "));
        lcd.print(model->getRemainingForcedCommandSeconds());
        lcd.print(F("sec"));
    }
}


void DisplayController::display() {
    
    unsigned long timeNow = now();
    
    if (timeNow - screenDisplayTime >= screenStartTime) {
        //swap displays
        currentScreen = (currentScreen+1)%3;
        screenStartTime = timeNow;
        Serial.print(F("Changing screen to: "));
        Serial.println(currentScreen);
        lcd.clear();
        (this->*(screens[currentScreen]))();
    }
    
    (this->*(screens[currentScreen]))();

    if(model->isThermostatOn() && !model->isThermostatForcedCommand()) {
        lcd.setRGB(180, 50, 50);
    } else  if( !model->isThermostatOn() && !model->isThermostatForcedCommand()) {
        lcd.setRGB(25, 150, 240);
    } else if(model->isThermostatOn() && model->isThermostatForcedCommand()) {
        lcd.setRGB(255, 0, 0);
    } else if( !model->isThermostatOn() && model->isThermostatForcedCommand()) {
        lcd.setRGB(0, 0, 255);
    }
}

