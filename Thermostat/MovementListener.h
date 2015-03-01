//
//  MovemementChangeListener.h
//  BoilerController
//
//  Created by Jakub Wozniak on 07/02/15.
//  Copyright (c) 2015 Jakub Wozniak. All rights reserved.
//

#ifndef __BoilerController__MovemementChangeListener__
#define __BoilerController__MovemementChangeListener__

#include "Events.h"
class MovementListener {
public:
    virtual void onMovementDetected() = 0;
};

#endif /* defined(__BoilerController__MovemementChangeListener__) */
