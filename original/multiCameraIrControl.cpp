/*******************************************
 *
 * Name.......:  cameraIrControl Library
 * Description:  A powerful Library to control easy various cameras via IR. Please check the project page and leave a comment.
 * Author.....:  Sebastian Setz
 * Version....:  1.9
 * Date.......:  2013-02-12
 * Project....:  http://sebastian.setz.name/arduino/my-libraries/multiCameraIrControl
 * Contact....:  http://Sebastian.Setz.name
 * License....:  This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.
 *               To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/3.0/ or send a letter to
 *               Creative Commons, 171 Second Street, Suite 300, San Francisco, California, 94105, USA.
 * Keywords...:  arduino, library, camera, ir, control, canon, nikon, olympus, minolta, sony, pentax, interval, timelapse
 * History....:  2010-12-08 V1.0 - release
 *               2010-12-09 V1.1 
 *               2010-12-16 V1.2
 *               2011-01-01 V1.3
 *               2011-01-04 V1.4 - making Sony function work, thank you Walter.
 *               2011-01-25 V1.5 - making Olympus work, thank you Steve Stav.
 *               2011-12-05 V1.6 - adding Olympus zoom, thank you again Steve! Refresh keywords.txt; Arduino 1.0 compatible
 *               2011-12-07 V1.7 - repairing Canon function - thanks to Viktor
 *               2013-02-11 V1.8 - adding toggleFocus for Pentax - thanks to Marcel Normann
 * 								   adding toggleVideo for Sony - thanks to InfinitR
 *               	    V1.9 - adding CanonWLDC100 support - thanks to ImaRH
 *
 ********************************************/

#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include "multiCameraIrControl.h"

void wait(unsigned int time){
  unsigned long start = micros();
  while(micros()-start<=time){
  }
}

void high(unsigned int time, int freq, int pinLED){
  int pause = (1000/freq/2)-4;
  unsigned long start = micros();
  while(micros()-start<=time){
    digitalWrite(pinLED,HIGH);
    delayMicroseconds(pause);
    digitalWrite(pinLED,LOW);
    delayMicroseconds(pause);
  }
}

Nikon::Nikon(int pin)
{
  pinMode(pin, OUTPUT);
  _pin = pin;
  _freq = 40;
}

void Nikon::shutterNow()
{
  high(2000,_freq,_pin);
  wait(27830);
  high(390,_freq,_pin);
  wait(1580);
  high(410,_freq,_pin);
  wait(3580);
  high(400,_freq,_pin);
}

Pentax::Pentax(int pin)
{
  pinMode(pin, OUTPUT);
  _pin = pin;
  _freq = 38;
}

void Pentax::shutterNow()
{
  high(13000,_freq,_pin);
  wait(3000);
  for (int i=0;i<7;i++){
    high(1000,_freq,_pin);
    wait(1000);
  };
}

void Pentax::toggleFocus()
{
  high(13000,_freq,_pin);
  wait(3000);
  for (int i=0;i<5;i++){
    high(1000,_freq,_pin);
    wait(1000);
  };
  high(1000,_freq,_pin);
  wait(3000);
  high(1000,_freq,_pin);
}

Olympus::Olympus(int pin)
{
  pinMode(pin, OUTPUT);
  _pin = pin;
  _freq = 40;
}

void Olympus::shutterNow()
{
  bool _seq[] = {
    0,1,1,0,0,0,0,1,1,1,0,1,1,1,0,0,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1    };
  high(8972,_freq,_pin);
  wait(4384);
  high(624,_freq,_pin);
  for (int i=0;i<sizeof(_seq);i++){
    if (_seq[i]==0){
      wait(488);
      high(600,_freq,_pin);
    }
    else{
      wait(1600);
      high(600,_freq,_pin);
    }
  };
}

void Olympus::zoomin(unsigned int pct)
{
  bool _seq[] = {
    0,1,1,0,0,0,0,1,1,1,0,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1    };
  high(9000,_freq,_pin);
  wait(4500);
  high(500,_freq,_pin);
  for (int i=0;i<sizeof(_seq);i++){
    if (_seq[i]==0){
      wait(500);
      high(500,_freq,_pin);
    }
    else{
      wait(1500);
      high(500,_freq,_pin);
    }
  };
  wait(40000);
  if (pct>100) pct = 100;
  pct = (pct*52)/100 + 1;
  for (int i=1; i<pct; i++)
  {
    high(9000,_freq,_pin);
    wait(2000);
    high(500,_freq,_pin);
    wait(96000);
  }

}

void Olympus::zoomout(unsigned int pct)
{
  bool _seq[] = 
             {0,1,1,0,0,0,0,1,1,1,0,1,1,1,0,0,0,1,0,0,0,0,0,0,1,0,1,1,1,1,1,1 };
  high(9000,_freq,_pin);           // 
  wait(4500);
  high(500,_freq,_pin);
  for (int i=0;i<sizeof(_seq);i++){
    if (_seq[i]==0){
      wait(500);
      high(500,_freq,_pin);
    }
    else{
      wait(1500);
      high(500,_freq,_pin);
    }
  };
  wait(40000);
  if (pct>100) pct = 100;
  pct = (pct*70)/100 + 1;
  for (int i=1; i<pct; i++)
  {
    high(9000,_freq,_pin);
    wait(2000);
    high(500,_freq,_pin);
    wait(96000);
  }
}

Minolta::Minolta(int pin)
{
  pinMode(pin, OUTPUT);
  _pin = pin;
  _freq = 38;
}

void Minolta::shutterNow()
{
  bool _seq[] = {
    0,0,1,0,1,1,0,0,0,1,0,1,0,0,1,1,1,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1    };
  high(3750,_freq,_pin);
  wait(1890);
  for (int i=0;i<sizeof(_seq);i++){
    if (_seq[i]==0){
      high(456,_freq,_pin);
      wait(487);
    }
    else{
      high(456,_freq,_pin);
      wait(1430);
    }
  };
}

void Minolta::shutterDelayed()
{
  bool _seqDelayed[] = {
    0,0,1,0,1,1,0,0,0,1,0,1,0,0,1,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1    };
  high(3750,_freq,_pin);
  wait(1890);
  for (int i=0;i<sizeof(_seqDelayed);i++){
    if (_seqDelayed[i]==0){
      high(456,_freq,_pin);
      wait(487);
    }
    else{
      high(456,_freq,_pin);
      wait(1430);
    }
  };
}

Sony::Sony(int pin)
{
  pinMode(pin, OUTPUT);
  _pin = pin;
  _freq = 40;
}

void Sony::shutterNow()
{
  bool _seq[] = {
    1,0,1,1,0,1,0,0,1,0,1,1,1,0,0,0,1,1,1,1    };
  for (int j=0;j<3;j++) {
    high(2320,_freq,_pin);
    wait(650);
    for (int i=0;i<sizeof(_seq);i++){
      if (_seq[i]==0){
        high(575,_freq,_pin);
        wait(650);
      }
      else{
        high(1175,_freq,_pin);
        wait(650);
      }
    }
    wait(10000);
  }
}

void Sony::shutterDelayed()
{
  bool _seqDelayed[] = {
    1,1,1,0,1,1,0,0,1,0,1,1,1,0,0,0,1,1,1,1    };
  for (int j=0;j<3;j++) {
    high(2320,_freq,_pin);
    wait(650);
    for (int i=0;i<sizeof(_seqDelayed);i++){
      if (_seqDelayed[i]==0){
        high(575,_freq,_pin);
        wait(650);
      }
      else{
        high(1175,_freq,_pin);
        wait(650);
      }
    }
    wait(10000);
  }
}

void Sony::toggleVideo()
{
  bool _seqToggle[] = {
    0,0,0,1,0,0,1,0,1,0,1,1,1,0,0,0,1,1,1,1    };
  for (int j=0;j<3;j++) {
    high(2320,_freq,_pin);
    wait(650);
    for (int i=0;i<sizeof(_seqToggle);i++){
      if (_seqToggle[i]==0){
        high(575,_freq,_pin);
        wait(650);
      }
      else{
        high(1175,_freq,_pin);
        wait(650);
      }
    }
    wait(10000);
  }
}

Canon::Canon(int pin)
{
  pinMode(pin, OUTPUT);
  _pin = pin;
  _freq = 33;
}

void Canon::shutterNow()
{
 for(int i=0; i<16; i++) { 
    digitalWrite(_pin, HIGH);
    delayMicroseconds(11);
    digitalWrite(_pin, LOW);
    delayMicroseconds(11);
  } 
  delayMicroseconds(7330); 
  for(int i=0; i<16; i++) { 
    digitalWrite(_pin, HIGH);
    delayMicroseconds(11);
    digitalWrite(_pin, LOW);
    delayMicroseconds(11);
  }
}

void Canon::shutterDelayed()
{
  for(int i=0; i<16; i++) { 
    digitalWrite(_pin, HIGH);
    delayMicroseconds(11);
    digitalWrite(_pin, LOW);
    delayMicroseconds(11);
  } 
  delayMicroseconds(5360); 
  for(int i=0; i<16; i++) { 
    digitalWrite(_pin, HIGH);
    delayMicroseconds(11);
    digitalWrite(_pin, LOW);
    delayMicroseconds(11);
  }
}

CanonWLDC100::CanonWLDC100(int pin)
{
  pinMode(pin, OUTPUT);
  _pin = pin;
  _freq = 38;
}
 
void CanonWLDC100::shutterNow()
{
  bool _seq[] = {0,1,0,1,0,0,1,1,1,0,0,0,1,1,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1};
  high(9042,_freq,_pin);
  wait(4379);
 
  for (int i=0;i<sizeof(_seq);i++){
    if (_seq[i]==0){
      high(612,_freq,_pin);
      wait(512);
    }
    else{
      high(612,_freq,_pin);
      wait(1621);
    }
  };
    high(599,_freq,_pin);
}
