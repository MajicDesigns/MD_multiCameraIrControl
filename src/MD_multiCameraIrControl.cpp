/*******************************************
 *
 * Name.......:  cameraIrControl Library
 * Description:  A powerful Library to control easy various cameras via IR. Please check the project page and leave a comment.
 * Author.....:  Sebastian Setz
 * Project....:  http://sebastian.setz.name/arduino/my-libraries/multiCameraIrControl
 * Contact....:  http://Sebastian.Setz.name
 * License....:  This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.
 *               To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/3.0/ or send a letter to
 *               Creative Commons, 171 Second Street, Suite 300, San Francisco, California, 94105, USA.
 * 
 * See main header file for additional details
 * 
 * Possible Issues:
 *  - Originally time parameter to wait() was unsigned int. The Olympus::zoomOut method uses a wait(96000) which will behave 
 *    differently with the larger integer. This will need to be tested with a camera as it is unclear what was intended or if
 *    it was ever tested properly.
 ********************************************/

#include "MD_multiCameraIrControl.h"

#define ARRAY_SIZE(a)  (sizeof(a)/sizeof((a)[0]))
const uint8_t BITFIELD_SIZE = 32;

void cCamera::wait(uint32_t time)
// Implements a very long microsecond wait
{
  uint32_t timeStart = micros();

  do { /* nothing */ } while (micros() - timeStart < time);
}

void cCamera::high(uint32_t time)
{
  uint32_t pause = (1000 / _freq / 2) - 4;
  uint32_t start = micros();

  while (micros() - start <= time)
  {
    digitalWrite(_pin, HIGH);
    delayMicroseconds(pause);
    digitalWrite(_pin, LOW);
    delayMicroseconds(pause);
  }
}

inline void cCamera::signal(uint32_t timeH, uint32_t timeW)
{
  high(timeH);
  delayMicroseconds(timeW);
}

// == NIKON ==

void Nikon::shutterNow(void)
{
  signal(2000, 27830);
  signal( 390, 1580);
  signal( 410, 3580);
  high(400);
}

// == PENTAX ==

void Pentax::shutterNow(void)
{
  signal(13000, 3000);
  for (uint8_t i = 0; i < 7; i++)
    signal(1000, 1000);
}

void Pentax::toggleFocus(void)
{
  signal(13000, 3000);

  for (uint8_t i = 0; i < 5; i++)
    signal(1000, 1000);

  signal(1000, 3000);
  high(1000);
}

// == OLYMPUS ==

void Olympus::shutterNow(void)
{
  uint32_t seq[] = { 0b01100001110111001000000001111111 };
  uint8_t seqValid[] = { 32 };    // number of valid bits from MSB for each seq[i]

  signal(8972, 4384);
  high(600);

  for (uint8_t j = 0; j < ARRAY_SIZE(seq); j++)   // for each 32 bit field
  {
    for (uint8_t i = 0; i < seqValid[j]; i++)     // for each bit in the field
    {
      wait(seq[j] & (1L << (BITFIELD_SIZE - i - 1)) ? 1600 : 488);
      high(580);
    }
  }
}

void Olympus::zoomIn(uint16_t pct)
{
  uint32_t seq[] = { 0b01100001110111001100000000111111 };
  uint8_t seqValid[] = { 32 };    // number of valid bits from MSB for each seq[i]

  signal(9000, 4500);
  high(500);

  for (uint8_t j = 0; j < ARRAY_SIZE(seq); j++)   // for each 32 bit field
  {
    for (uint8_t i = 0; i < seqValid[j]; i++)    // for each bit in the field
    {
      wait(seq[j] & (1L << (BITFIELD_SIZE - i - 1)) ? 1500 : 500);
      high(500);
    }
  }
  wait(40000);

  if (pct > 100) pct = 100;
  pct = (pct * 52) / 100 + 1;

  for (uint8_t i = 1; i < pct; i++)
  {
    signal(9000,  2000);
    signal( 500, 96000);
  }
}

void Olympus::zoomOut(uint16_t pct)
{
  uint32_t seq[] = { 0b01100001110111000100000010111111 };
  uint8_t seqValid[] = { 32 };    // number of valid bits from MSB for each seq[i]

  signal(9000, 4500);
  high(500);

  for (uint8_t j = 0; j < ARRAY_SIZE(seq); j++)   // for each 32 bit field
  {
    for (uint8_t i = 0; i < seqValid[j]; i++)    // for each bit in the field
    {
      wait(seq[j] & (1L << (BITFIELD_SIZE - i - 1)) ? 1500 : 500);
      high(500);
    }
  }
  wait(40000);

  if (pct > 100) pct = 100;
  pct = (pct * 70) / 100 + 1;

  for (uint8_t i = 1; i < pct; i++)
  {
    signal(9000, 2000);
    signal(500, 96000);
  }
}

// == MINOLTA ==

void Minolta::shutterNow(void)
{
  uint32_t seq[] = { 0b00101100010100111000001010000000, 0b10000000000000000000000000000000 };
  uint8_t seqValid[] = { 32, 1 };    // number of valid bits from MSB for each seq[i]

  signal(3750, 1890);

  for (uint8_t j = 0; j < ARRAY_SIZE(seq); j++)   // for each 32 bit field
  {
    for (uint8_t i = 0; i < seqValid[j]; i++)    // for each bit in the field
      signal(456, seq[j] & (1L << (BITFIELD_SIZE - i - 1)) ? 1350 : 450);
  }
}

void Minolta::shutterDelayed(void)
{
  uint32_t seq[] = { 0b00101100010100111000001000000000, 0b10000000000000000000000000000000 };
  uint8_t seqValid[] = { 32, 1 };    // number of valid bits from MSB for each seq[i]

  signal(3750,1890);

  for (uint8_t j = 0; j < ARRAY_SIZE(seq); j++)   // for each 32 bit field
  {
    for (uint8_t i = 0; i < seqValid[j]; i++)    // for each bit in the field
      signal(456, seq[j] & (1L << (BITFIELD_SIZE - i - 1)) ? 1350 : 450);
  }
}

// == SONY ==

void Sony::shutterNow(void)
{
  uint32_t seq[] = { 0b10110100101110001111000000000000 };
  uint8_t seqValid[] = { 20 };    // number of valid bits from MSB for each seq[i]

  for (uint8_t k = 0; k < 3; k++) 
  {
    signal(2320, 650);

    for (uint8_t j = 0; j < ARRAY_SIZE(seq); j++)   // for each 32 bit field
    {
      for (uint8_t i = 0; i < seqValid[j]; i++)    // for each bit in the field
        signal(seq[j] & (1L << (BITFIELD_SIZE - i - 1)) ? 1100 : 600, 650);
    }
    wait(10000);
  }
}

void Sony::shutterDelayed(void)
{
  uint32_t seq[] = { 0b11101100101110001111000000000000 };
  uint8_t seqValid[] = { 20 };    // number of valid bits from MSB for each seq[i]

  for (uint8_t k = 0; k < 3; k++)
  {
    signal(2320, 650);

    for (uint8_t j = 0; j < ARRAY_SIZE(seq); j++)   // for each 32 bit field
    {
      for (uint8_t i = 0; i < seqValid[j]; i++)    // for each bit in the field
        signal(seq[j] & (1L << (BITFIELD_SIZE - i - 1)) ? 1100 : 600, 650);
    }
    wait(10000);
  }
}

void Sony::toggleVideo(void)
{
  uint32_t seq[] = { 0b00010010101110001111000000000000 };
  uint8_t seqValid[] = { 20 };    // number of valid bits from MSB for each seq[i]

  for (uint8_t k = 0; k < 3; k++) 
  {
    signal(2320, 650);

    for (uint8_t j = 0; j < ARRAY_SIZE(seq); j++)  // for each 32 bit field
    {
      for (uint8_t i = 0; i < seqValid[j]; i++)    // for each bit in the field
        signal(seq[j] & (1L << (BITFIELD_SIZE - i - 1)) ? 1100 : 600, 650);
    }
    wait(10000);
  }
}

// == CANON ==

void Canon::shutterNow(void)
{
 for(uint8_t i = 0; i < 16; i++) 
 { 
   digitalWrite(_pin, HIGH);
   delayMicroseconds(11);
   digitalWrite(_pin, LOW);
   delayMicroseconds(11);
 } 

 delayMicroseconds(7330); 

 for(uint8_t i = 0; i < 16; i++) 
 { 
   digitalWrite(_pin, HIGH);
   delayMicroseconds(11);
   digitalWrite(_pin, LOW);
   delayMicroseconds(11);
 }
}

void Canon::shutterDelayed(void)
{
  for (uint8_t i = 0; i < 16; i++) 
  { 
    digitalWrite(_pin, HIGH);
    delayMicroseconds(11);
    digitalWrite(_pin, LOW);
    delayMicroseconds(11);
  } 

  delayMicroseconds(5360); 

  for (uint8_t i = 0; i < 16; i++) 
  { 
    digitalWrite(_pin, HIGH);
    delayMicroseconds(11);
    digitalWrite(_pin, LOW);
    delayMicroseconds(11);
  }
}

// == CANONWLDC100 ==

void CanonWLDC100::shutterNow(void)
{
  uint32_t seq[] = { 0b01010011100011011111100000000111 };
  uint8_t seqValid[] = { 32 };    // number of valid bits from MSB for each seq[i]

  signal(9042, 4379);
 
  for (uint8_t j = 0; j < ARRAY_SIZE(seq); j++)  // for each 32 bit field
  {
    for (uint8_t i = 0; i < seqValid[j]; i++)    // for each bit in the field
    {
      signal(600, seq[j] & (1L << (BITFIELD_SIZE - i - 1)) ? 1610 : 500);
    }
  }
  high(599);
}
