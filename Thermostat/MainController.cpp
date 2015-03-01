//
// MainController.cpp
// C++ code
// ----------------------------------
// Developed with embedXcode
// http://embedXcode.weebly.com
//
// Project 		MainController
//
// Created by 	Jakub Wozniak, 24/10/14 4:44 pm
// 				Jakub Wozniak
//
// Copyright	© Jakub Wozniak, 2014
// License   	<#license#>
//
// See 			MainController.h and ReadMe.txt for references
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
#include "MainController.h"
#include "Time.h"
#include "Events.h"
#include "TempRange.h"
#include "DailySettings.h"
#include "Constants.h"
// Code

#include <DS1307RTC.h>  // a basic DS1307 library that returns time as a time_t


MainController::MainController(ControllerModel* modelPtr) {
    this->model = modelPtr;
    pinMode(THERMOSTAT_PIN, OUTPUT);
    pinMode(MOVE_SENSOR_PIN, INPUT);
    pinMode(MOVE_SENSOR_LED_PIN, OUTPUT);
    
    this->modeButton = createButton(MODE_BUTTON_PIN);
    this->forceOnButton  = createButton(FORCE_ON_BUTTON_PIN);
    this->forceOffButton = createButton(FORCE_OFF_BUTTON_PIN);
    
    this->dht = new DHT(DHTPIN, DHTTYPE);
    this->dht->begin();
}

Button* MainController::createButton(byte pin) {
    Button* button = new Button();
    button->assign(pin);
    button->setMode(OneShotTimer);
    button->setTimer(1000);
    button->turnOnPullUp();
    return button;
}


OnReadStateEvent MainController::readState() {
    OnReadStateEvent event;
    event.on = model->isThermostatOn();
    event.forced = model->isThermostatForcedCommand();
    event.presenceMode = !model->isCalendarMode();
    event.remoteTimeout = model->isRemoteCommandTimeout();
    event.tempTimeout = model->isRemoteTemperatureTimeout();
    event.lastLocalTemp = model->getLastLocalTemperatureReading();
    event.meanLocalTemp = model->getLocalMeanTemperature();
    if(!model->isRemoteTemperatureTimeout()) {
        event.lastRemoteTemp = model->getLastRemoteTemperatureReading();
        event.meanRemoteTemp = model->getRemoteMeanTemperature();
    } else {
        event.lastRemoteTemp = NAN;
        event.meanRemoteTemp = NAN;
    }
    event.refTemp = model->getRefTemperature();
    event.lastLocalHumidity = model->getLastLocalHumidityReading();
    event.localMovementDetectorState = model->getLocalMovementDetectorState();
    event.presenceDetected = model->isPresenceDetected();
    event.time = now();
    return event;
}

float MainController::readInternalTemperature() {
    
    unsigned int wADC;
    float t;
    
    // The internal temperature has to be used
    // with the internal reference of 1.1V.
    // Channel 8 can not be selected with
    // the analogRead function yet.
    
    // Set the internal reference and mux.
    ADMUX = (_BV(REFS1) | _BV(REFS0) | _BV(MUX3));
    ADCSRA |= _BV(ADEN);  // enable the ADC
    
    delay(20);            // wait for voltages to become stable.
    
    ADCSRA |= _BV(ADSC);  // Start the ADC
    
    // Detect end-of-conversion
    while (bit_is_set(ADCSRA,ADSC));
    
    // Reading register "ADCW" takes care of how to read ADCL and ADCH.
    wADC = ADCW;
    
    // The offset of 324.31 could be wrong. It is just an indication.
    t = (wADC - 324.31 ) / 1.22;
    
    // The returned temperature is in degrees Celcius.
    return (t);
    
    
    
}

void MainController::readDHTSensorTemperatureAndHumidity() {
    float h = dht->readHumidity();
    float t = dht->readTemperature();
    
    // check if returns are valid, if they are NaN (not a number) then something went wrong!
    if (!isnan(t))  {
        model->addLocalTemperatureReading(t+LOCAL_TEMP_CORRECTION);
    }
    
    if(!isnan(h)) {
        model->addLocalHumidityReading(h);
    }
}

void MainController::onTimeEvent(long time) {
    model->resetRemoteCommandTimeout();
    RTC.set(time);   // set the RTC and the system time to the received value
    setTime(time);
}

void MainController::onTemperatureEvent(AddTemperatureReadingEvent& event) {
    if(event.thermometerNumber == 1) {
        float temp = event.value;
        float mean = model->getRemoteMeanTemperature();
        bool remoteTimeout = model->isRemoteTemperatureTimeout();
        if((!remoteTimeout && !isnan(mean) && fabsf(mean-temp) < 8.0) ||
           ( remoteTimeout || isnan(mean) )
           ) {
            //we only add if the temperature makes sense and we read from thermometer number 1 (this is currently our only remote).
            model->addRemoteTemperatureReading(event.value);
        }
        if(temp > 40) {
            //Fire!!!???
            //Once we have a buzzer we can use it!!!
        }
    }
};

void MainController::onDailySettingDefaultTemperatureEvent(AddDailyDefaultTemperatureEvent& event) {
    model->resetRemoteCommandTimeout();
    model->setDailyDefaultTemperature(event.setNumber,event.weekDay, event.temperature);
}


void MainController::onDailyRangeSettingEvent(AddDailyRangeSettingEvent& event) {
    model->resetRemoteCommandTimeout();
    TempRange range(event.range.hourFrom, event.range.minuteFrom, event.range.hourTo, event.range.minuteTo, event.range.temperature);
    model->setRangeValues(event.setNumber, event.weekDay, event.rangeIndex, range);
    
};

void MainController::onThermostatModePresenceEvent() {
    model->setPresenceMode();
}

void MainController::onThermostatModeCalendarEvent() {
    model->setCalendarMode();
}


void MainController::processSensors() {
    
    if(model->isLocalTemperatureTimeout()) {
        readDHTSensorTemperatureAndHumidity();
    }
    
    checkModeButton();
    checkForceOnButton();
    checkForceOffButton();
    checkMoveSensor();
}


void MainController::checkMoveSensor() {
    if(digitalRead(MOVE_SENSOR_PIN) == HIGH) {
        digitalWrite(MOVE_SENSOR_LED_PIN, HIGH);
        this->model->setPresenceDetected();
        this->model->setLocalMovementDetectorState(true);
    } else {
        this->model->setLocalMovementDetectorState(false);
        digitalWrite(MOVE_SENSOR_LED_PIN, LOW);
    }
}


void MainController::checkModeButton() {
    if(this->modeButton->check() == ON) {
        Serial.println(F("Button mode is pressed"));
        if(model->isCalendarMode()) {
            model->setPresenceMode();
        } else {
            model->setCalendarMode();
        }
    }
}

void MainController::checkForceOnButton() {
    if(this->forceOnButton->check() == ON) {
        Serial.println(F("Button force on is pressed"));
        this->ThermostatOnCommand();
    }
}

void MainController::checkForceOffButton() {
    if(this->forceOffButton->check() == ON) {
        Serial.println(F("Button force off is pressed"));
        this->ThermostatOffCommand();
    }
}

/*
 Controls the Thermostat using the model
 */
void MainController::controlThermostat() {
    
    if( !model->isThermostatForcedCommand()) {
        float referenceTemp = model->getRefTemperature();
        if(referenceTemp == NAN) {
            Serial.println(F("Cannot read reference temperature, 18.0 used as default"));
            referenceTemp = 18.0;
        }
        float currentMeanTemp = NAN;
        
        if(!model->isRemoteTemperatureTimeout()) {
            currentMeanTemp = model->getRemoteMeanTemperature();
        } else {
            currentMeanTemp = model->getLocalMeanTemperature();
        }
        if(currentMeanTemp==NAN) {
            Serial.println(F("Cannot read mean temperature, 18.0 used as default"));
            currentMeanTemp = 18.0;
        }
        if(currentMeanTemp < referenceTemp) {
          model->setThermostatOn();
        } else {
          model->setThermostatOff();
        }
    }
    
    if(model->isThermostatOn()) {
        digitalWrite(THERMOSTAT_PIN, HIGH);
    } else {
        digitalWrite(THERMOSTAT_PIN, LOW);
    }
    
    
};


void MainController::onRemotePingEvent() {
    model->resetRemoteCommandTimeout();
}

void MainController::onThermostatPresenceTemperaturesEvent(float comfortTemp, float defaultTemp, float nightTemp) {
    model->resetRemoteCommandTimeout();
    model->setPresenceTemperatures(comfortTemp, defaultTemp, nightTemp);
};


void MainController::onPresenceDetectedEvent() {
    model->resetRemoteCommandTimeout();
    model->setPresenceDetected();
}

void MainController::onThermostatOnEvent() {
    model->resetRemoteCommandTimeout();
    this->ThermostatOnCommand();
    
}

void MainController::ThermostatOnCommand() {
    if(model->isThermostatForcedCommand()) {
        model->resetThermostatForcedCommand();
    } else {
        model->setThermostatOnForcedCommand();
    }
}

void MainController::ThermostatOffCommand() {
    if(model->isThermostatForcedCommand()) {
        model->resetThermostatForcedCommand();
    } else {
        model->setThermostatOffForcedCommand();
    }

}

void MainController::onThermostatOffEvent() {
    model->resetRemoteCommandTimeout();
    this->ThermostatOffCommand();
}

void MainController::onThermostatOnOffResetEvent() {
    model->resetRemoteCommandTimeout();
    model->resetThermostatForcedCommand();
}


