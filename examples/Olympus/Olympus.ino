/*******************************************
 *
 * Name.......:  multiCameraIrControl Library - Olympus Example
 * Description:  A small example sketch how to use multiCameraIrControl library. Please check the project page for wiring and leave a comment.
 * Author.....:  Sebastian Setz
 * Version....:  1.3
 * Date.......:  2011-01-28
 * Project....:  http://sebastian.setz.name/arduino/my-libraries/multi-Camera-IR-Control
 * Contact....:  http://Sebastian.Setz.name
 * License....:  This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.
 *               To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/3.0/ or send a letter to
 *               Creative Commons, 171 Second Street, Suite 300, San Francisco, California, 94105, USA.
 * Keywords...:  arduino, library, camera, ir, control, canon, nikon, olympus, minolta, sony, pentax, interval, timelapse
 * History....:  2010-12-08 V1.0 - release
 *               2011-01-04 V1.1
 *               2011-01-25 V1.2 - changing pin number, because the wiring at the homepage was different
 *               2011-01-28 V1.3 - adding zoom to the example
 *
 ********************************************/

#include <MD_multiCameraIrControl.h>

Olympus E5(5);

void setup(void)
{
  E5.begin();
}

void loop(void)
{
  E5.shutterNow();
  delay(1000);
  E5.zoomIn(10);
  delay(1000);
  E5.zoomOut(10);
  delay(3000);
}




