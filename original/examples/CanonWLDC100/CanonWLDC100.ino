/*******************************************
 *
 * Name.......:  multiCameraIrControl Library - CanonWLDC100 Example
 * Description:  A small example sketch how to use multiCameraIrControl library. Please check the project page for wiring and leave a comment.
 * Author.....:  Sebastian Setz
 * Version....:  1.0
 * Date.......:  2013-02-12
 * Project....:  http://sebastian.setz.name/arduino/my-libraries/multi-Camera-IR-Control
 * Contact....:  http://Sebastian.Setz.name
 * License....:  This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.
 *               To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/3.0/ or send a letter to
 *               Creative Commons, 171 Second Street, Suite 300, San Francisco, California, 94105, USA.
 * Keywords...:  arduino, library, camera, ir, control, canon, nikon, olympus, minolta, sony, pentax, interval, timelapse
 * History....:  2013-02-12 V1.0 - release
 *
 ********************************************/

#include <multiCameraIrControl.h>

CanonWLDC100 D3(9);

void setup(){
}

void loop(){
  D3.shutterNow();
  delay(5000);
  D3.shutterDelayed();
  delay(5000);
}



