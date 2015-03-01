//
// BoilerController.cpp 
// C++ code
// ----------------------------------
// Developed with embedXcode 
// http://embedXcode.weebly.com
//
// Project 		BoilerController
//
// Created by 	Jakub Wozniak, 24/10/14 4:44 pm
// 				Jakub Wozniak
//
// Copyright	© Jakub Wozniak, 2014
// License   	<#license#>
//
// See 			BoilerController.h and ReadMe.txt for references
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
#include "BoilerController.h"

// Code
void BoilerController::processMessages(){
    float tempInC =  this->commController->readTempSensor();
    if (tempInC != INVALID_TEMP)
        model->addTemperatureReading(tempInC);
    } else {
        Serial.println("Invalid temp received");
    }

};
/*
 Controls the boiler using the model
 */
void BoilerController::controlBoiler() {
    
    //get time from TimeController?
    
    float referenceTemp = this->model->getTemperature(1, 10, 10);
    float currentMeanTemp = this->model->getMeanTemperature();
    
    if(currentMeanTemp < referenceTemp) {
        this.boilerOn = true;
        digitalWrite(8, HIGH);
    } else {
        this.boilerOn = false;
        digitalWrite(8, LOW);
    }
}

void BoilerController::isBoilerOn() {
    return this.boilerOn;
}

