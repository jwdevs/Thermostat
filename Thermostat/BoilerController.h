//
// File			BoilerController.h
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
// Date			24/10/14 4:45 pm
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

#include "DataModel.h"
#include "CommunicationController.h"
#ifndef BoilerController_h
#define BoilerController_h

class BoilerController {
private:
    ControllerModel* model;
    CommunicationController * commController;
    
public:
    BoilerController(ControllerModel* modelPtr, CommunicationController* controller) {
        this->model = modelPtr;
        this->commController = controller;
    }
    /*
     Processes the messages from external sources.
     Saves the neccessary information to the model
     */
    void processMessages();
    /*
     Controls the boiler using the model
     */
    void controlBoiler();
    
    /**
     * Returns true is boiler is currently on. 
     */
    bool isBoilerOn();
    
};


#endif
