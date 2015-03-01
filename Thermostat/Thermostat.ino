
//
// Description of the project
// Developed with [embedXcode](http://embedXcode.weebly.com)
//
// Author	 	Jakub Wozniak
// 				Jakub Wozniak
//
// Date			21/10/14 11:08 pm
// Version		1.0.0
//
// Copyright	© Jakub Wozniak, 2014
// License		GPL 3.0
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


#include "MainController.h"
#include "DisplayController.h"
#include <Wire.h>

#include <stdlib.h>
#include "DataModel.h"
#include "CommunicationController.h"
#include <SoftwareSerial.h>
#include <Time.h>
#include <DS1307RTC.h>  // a basic DS1307 library that returns time as a time_t
#include <EEPROM.h>
#include "ApplicationMonitor.h"
Watchdog::CApplicationMonitor ApplicationMonitor;
//It has to be on the stack, not on the heap for some reason?
//Maybe the stack is too small to accomodate it?
//If on the heap it causes the program to crash when Arduino is rebooted.
rgb_lcd lcd;

ControllerModel * model;
CommunicationController* communicationController ;
MainController* controller;
DisplayController * displayController;



int freeRam () {
    extern int __heap_start, *__brkval;
    int v;
    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

long time = 0;

void setup() {
    //clear the crash reports data
    //eeprom_write_byte((uint8_t*)500,0xff);
    
    Serial.begin(9600); // set up the serial port
    
    Serial.println(F("Setup start"));
    
    
    setSyncProvider(RTC.get);   // the function to get the time from the RTC
    
    
    Serial.println(F("Setting up controllers"));

    model = new ControllerModel();

    Serial.println(F("Model ok"));

    controller = new MainController(model);
    
    Serial.println(F("Controller ok"));

    communicationController = new CommunicationController(controller);
    
    model->setThermostatStateListener(communicationController);
    model->setMovementListener(communicationController);
    
    Serial.println(F("Comm ok"));

    displayController = new DisplayController(model, 3, lcd);
    
    Serial.println(F("Display ok"));

    //read model from memory
    //TODO
    time = now();
    
    
    ApplicationMonitor.Dump(Serial);
    ApplicationMonitor.EnableWatchdog(Watchdog::CApplicationMonitor::Timeout_4s);
    
}


void loop() {
    ApplicationMonitor.IAmAlive();

    //Serial.println(F("Begin loop"));

    //read remote messages
    communicationController->readMessages();
    
    
    //read other sensors (internal temperature, motion sensors, etc).
    controller->processSensors();

    //control the boiler (set on/off based on temperature readings).
    controller->controlThermostat();
    //Serial.println(F("After controlling boiler"));
    //display current information on the lcd screen.
    displayController->display();
    //Serial.println(F("After display processed"));
    
    //model->printSerial();
    
    //send remote messages (send messages about the meanTemps, current state of the boiler relay)
    communicationController->sendMessages();
    
    if(now() - time > 10) {
        Serial.print(F("Free ram:"));
        Serial.println(freeRam());
        
        time = now();
        //while(true);
    }
    //delay(900);    // wait 1 seconds
}




