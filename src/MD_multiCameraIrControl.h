/*******************************************
 *
 * Name.......: cameraIrControl Library
 * Description: A powerful Library to control easy various cameras via IR. Please check the project page and leave a comment.
 * Author.....: Sebastian Setz
 * Version....: 2.0.0
 * Date.......: 2021-01-10
 * Project....: Original: http://sebastian.setz.name/arduino/my-libraries/multiCameraIrControl (defunct)
 *              New: http://github.com/MajicDesigns/MD_multiCameraIrControl
 * Contact....: http://Sebastian.Setz.name (defunct)
 * License....: This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.
 *              To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/3.0/ or send a letter to
 *              Creative Commons, 171 Second Street, Suite 300, San Francisco, California, 94105, USA.
 * Keywords...: arduino, library, camera, ir, control, canon, nikon, olympus, minolta, sony, pentax, interval, timelapse
 * History....: 2010-12-08 V1.0 - release
 *              2010-12-09 V1.1 
 *              2010-12-16 V1.2
 *              2011-01-01 V1.3
 *              2011-01-04 V1.4 - making Sony function work, thank you Walter.
 *              2011-01-25 V1.5 - making Olympus work, thank you Steve Stav.
 *              2011-12-05 V1.6 - adding Olympus zoom, thank you again Steve! Refresh keywords.txt; Arduino 1.0 compatible
 *              2011-12-07 V1.7 - repairing Canon function - thanks to Viktor
 *              2013-02-11 V1.8 - adding toggleFocus for Pentax - thanks to Marcel Normann
 *                                adding toggleVideo for Sony - thanks to InfinitR
 *                         V1.9 - adding CanonWLDC100 support - thanks to ImaRH
 *              2021-01-10 V2.0.0 - MajicDesigns library update
 *                                - eliminated Arduino compiler warnings
 *                                - created cCamera base class, refactored camera specific classes
 *                                - added begin() method to execute previous call to pinMode() in constructor
 *                                - added a 'dumb' default for shutterDelayed() (delay() and shutterNow())
 *                                - cleaned up invariants and rationalized data types
 *                                - converted _seq[] arrays to uint32_t bit fields to save RAM
 *                                - added signal() to consolidate high()/wait() sequence
 *                                - checked all IR signals output on Arduino Uno against original lib 
 *                                  output using AnalysIR; modified timing delays to match original output.
 *              2023-10-29 V2.1.0 - Fixed reported timing issues using wait() in high() and signal() instead of delayMicroseconds()
 *
 ********************************************/
#pragma once

#ifndef multiCameraIrControl_h    
#define multiCameraIrControl_h    // retained in case legacy code checks this
#endif

#include "Arduino.h"

// Base class ---------------------
class cCamera
{
  public:
    cCamera(uint8_t pin, uint8_t freq) : _pin(pin), _freq(freq) {}
    
    // This MUST be implemented all camera specific classes
    virtual void shutterNow(void) = 0;

    // These will override if not implemented in camera specific classes
    virtual void begin(void) { pinMode(_pin, OUTPUT); }
    virtual void shutterDelayed(void) { delay(SHUTTER_DELAY); shutterNow(); }

  protected:
    uint8_t _pin;   // LED output pin
    uint8_t _freq;  // in kHz (eg, 33, 38, 40)

    void wait(uint32_t time);      // Implements a very long microsecond wait
    void high(uint32_t time);      // modulated IR output at _freq
    void signal(uint32_t timeH, uint32_t timeW);  // wrapper for a high() followed by wait()
    
  private:
    static const uint16_t SHUTTER_DELAY = 2000; // time for base class delayed shutter in ms
};

// Derived classes -----------------
class Nikon: public cCamera
{
  public:
    Nikon(uint8_t pin): cCamera(pin, 40) {}

    void shutterNow(void);
};

class Canon : public cCamera
{
  public:
    Canon(uint8_t pin): cCamera(pin, 33) {}

    void shutterNow(void);
    void shutterDelayed(void);
};

class CanonWLDC100: public cCamera
{
  public:
    CanonWLDC100(uint8_t pin): cCamera(pin, 38) {}

    void shutterNow(void);
};
 
class Pentax: public cCamera
{
  public:
    Pentax(uint8_t pin): cCamera(pin, 38) {}

    void shutterNow(void);
    void toggleFocus(void);
};

class Olympus: public cCamera
{
  public:
    Olympus(uint8_t pin): cCamera(pin, 40) {}

    void shutterNow(void);
    void zoomIn(uint16_t pct);
    void zoomOut(uint16_t pct);
};

class Minolta: public cCamera
{
  public:
    Minolta(uint8_t pin): cCamera(pin, 38) {}

    void shutterNow(void);
    void shutterDelayed(void);
};

class Sony: public cCamera
{
  public:
    Sony(uint8_t pin): cCamera(pin, 40) {}

    void shutterNow(void);
    void shutterDelayed(void);
    void toggleVideo(void);
};

