
  Thermostat for the iHome project
  Project
  ----------------------------------
  Developed with embedXcode

  Project BoilerController
  Created by Jakub Wozniak on 23/10/14
  Copyright © 2014 Jakub Wozniak
  License GPL 3.0


Needs additional libs to compile, please check the Makefile for references (APP_LIBS_LIST, USER_LIBS_LIST)



AVRDUDE command for AVR dragon programmer chip upload:
/Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/avrdude -patmega328p -C/Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/etc/avrdude.conf -c dragon_isp  -P usb -Uflash:w:Builds/embeddedcomputing.hex:i -v





Current in mA:

LCD: 60mA
DHT: ~2mA
Relay: 60mA
TinyRTC: ???, assuming 100mA
XBee: 80mA
PIR: 1mA

Total: ~300mA












  embedXcode
  embedXcode+
  ----------------------------------
  Embedded Computing on Xcode
  Copyright © Rei VILO, 2010-2014
  All rights reserved
  http://embedXcode.weebly.com

