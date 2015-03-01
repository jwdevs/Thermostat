//
//  BoilerStateListener.h
//  BoilerController
//
//  Created by Jakub Wozniak on 07/02/15.
//  Copyright (c) 2015 Jakub Wozniak. All rights reserved.
//

#ifndef ThermostatStateListener_h
#define ThermostatStateListener_h
#include "Events.h"
class ThermostatStateListener {
public:
    virtual void onThermostatStateChange(bool isOn) = 0;
};
#endif
