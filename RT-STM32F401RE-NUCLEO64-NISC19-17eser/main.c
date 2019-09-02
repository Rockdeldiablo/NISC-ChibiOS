/*
    PLAY Embedded demos - Copyright (C) 2014...2019 Rocco Marco Guglielmi

    This file is part of PLAY Embedded demos.

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/
 
#include "ch.h"
#include "hal.h"
#include "shell.h"
#include "chprintf.h"
#include <string.h>




/*===========================================================================*/
/* Command line related.                                                     */
/*===========================================================================*/
#define SHELL_WA_SIZE   THD_WORKING_AREA_SIZE(2048)
#define LINE_LED_RGB_RED                 PAL_LINE(GPIOC, 0U)
#define LINE_LED_RGB_GREEN               PAL_LINE(GPIOC, 1U)
#define LINE_LED_RGB_BLUE                PAL_LINE(GPIOC, 2U)


/*===========================================================================*/
/* definitions  from demo 18                                                   */
/*===========================================================================*/
static int32_t mean;
static bool flag = FALSE;
static bool flag2 = FALSE;
static float lastvalue;



static THD_WORKING_AREA(waThd2, 256);
static THD_FUNCTION(Thd2, arg){
 (void) arg;
  chRegSetThreadName("Thread 2");
  palSetLine(LINE_LED_RGB_RED);
  palSetLine(LINE_LED_RGB_GREEN);
  palSetLine(LINE_LED_RGB_BLUE);
  while (!chThdShouldTerminateX()){
                 palClearLine(LINE_LED_RGB_GREEN);
                 chThdSleepSeconds(6);
                 palClearLine(LINE_LED_RGB_RED);
                 chThdSleepSeconds(3);
                 palSetLine(LINE_LED_RGB_GREEN);
                 chThdSleepSeconds(6);
                 palSetLine(LINE_LED_RGB_RED);
                 palClearLine(LINE_LED_RGB_GREEN);
  }
  chThdExit(0);
}




/* Can be measured using dd if=/dev/xxxx of=/dev/null bs=512 count=10000.*/
static void cmd_write(BaseSequentialStream *chp, int argc, char *argv[]) {
  static uint8_t buf[] =
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef";

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: write\r\n");
    return;
  }

  while (chnGetTimeout((BaseChannel *)chp, TIME_IMMEDIATE) == Q_TIMEOUT) {
    /* Writing in channel mode.*/
    chnWrite(&SD2, buf, sizeof buf - 1);
  }
  chprintf(chp, "\r\n\n stopped\r\n");
}


static void cmd_semaforo(BaseSequentialStream *chp, int argc, char *argv[]) {
  char *testvar = argv[0];
  flag2=true;
    thread_t *tp = chThdCreateStatic(waThd2, sizeof(waThd2), NORMALPRIO + 1,Thd2, NULL);


        if(!strcmp("ON",testvar) ){


        }

        else if (!strcmp("OFF",testvar)) {

          chThdTerminate(tp);
          flag2=false;
          palSetLine(LINE_LED_RGB_GREEN);
          palSetLine(LINE_LED_RGB_RED);
          palSetLine(LINE_LED_RGB_BLUE);

        }



  }



static void cmd_test(BaseSequentialStream *chp, int argc, char *argv[]) {
 char *testvar = argv[0];
 char *testvar1 = argv[1];
 char *testvar2 = argv[2];
  int led[2]= {0,0};
           if (flag2 == false){
                       if(!strcmp("LED",testvar)&& argc<=3){
                                   if(!strcmp("BLUE",testvar1)  ){
                                          if(!strcmp("ON",testvar2)){
                                            led[1]=1;
                                            led[2]=1;
                                            palClearLine(LINE_LED_RGB_BLUE);


                                            }
                                          else {
                                            led[1]=1;
                                            led[2]=0;
                                             palSetLine(LINE_LED_RGB_BLUE);
                                            }

                                      }
                                   else if (!strcmp("RED",testvar1) ){
                                           if(!strcmp("ON",testvar2)){
                                              led[1]=2;
                                              led[2]=1;
                                              palClearLine(LINE_LED_RGB_RED);
                                              }
                                           else {
                                              led[1]=2;
                                              led[2]=0;
                                              palSetLine(LINE_LED_RGB_RED);
                                              }
                                       }
                                   else if (!strcmp("GREEN",testvar1) ) {
                                            if(!strcmp("ON",testvar2)){
                                              led[1]=3;
                                              led[2]=1;
                                              palClearLine(LINE_LED_RGB_GREEN);
                                             }
                                            else {
                                              led[1]=3;
                                              led[2]=0;
                                              palSetLine(LINE_LED_RGB_GREEN);
                                             }
                                        }
                                   else
                                     {chprintf(chp, "wrong command \r\n" );}
                       }
                 /*      else if (!strcmp("POT",testvar)&& argc<=1  ){
                                 if(!strcmp("ON",testvar2)){
                                       //something
                                   }
                                 else {
                                       flag=false;
                                       chThdTerminate(tp3);

                                      }


                       }
                       else if (!strcmp("PWM",testvar)&& argc<=2){
                                       //dosomething

                       }*/

          }

         else if (flag2 == true) {
              chprintf(chp, "TEST doesn't work when SEMAFORO is active" );
         }



}

static const ShellCommand commands[] = {
  {"write", cmd_write},
  {"TEST", cmd_test},
  {"SEMAFORO", cmd_semaforo},
  {NULL, NULL}
};

static const ShellConfig shell_cfg1 = {
  (BaseSequentialStream *)&SD2,
  commands
};

static THD_WORKING_AREA(waThd1, 256);
static THD_FUNCTION(Thd1, arg){
  (void) arg;
  chRegSetThreadName(" dummy Thread 1");
  while(TRUE){
    palToggleLine(LINE_LED_GREEN);
    chThdSleepMilliseconds(500);
  }
}



/*
 * Application entry point.
 */
int main(void) {
  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  /*
   * Shell manager initialization.
   */
  shellInit();

  /*
  *
  *
  *
  */

  palSetLineMode(LINE_LED_RGB_RED, PAL_MODE_OUTPUT_PUSHPULL);
  palSetLineMode(LINE_LED_RGB_GREEN, PAL_MODE_OUTPUT_PUSHPULL);
  palSetLineMode(LINE_LED_RGB_BLUE, PAL_MODE_OUTPUT_PUSHPULL);

  palSetLine(LINE_LED_RGB_RED);
  palSetLine(LINE_LED_RGB_GREEN);
  palSetLine(LINE_LED_RGB_BLUE);

  chThdCreateStatic(waThd1, sizeof(waThd1), NORMALPRIO + 1,Thd1, NULL);
  sdStart(&SD2, NULL);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and check the button state.
   */
  while (true) {
    thread_t *shelltp = chThdCreateFromHeap(NULL, SHELL_WA_SIZE,
                                            "shell", NORMALPRIO + 1,
                                            shellThread, (void *)&shell_cfg1);
    chThdWait(shelltp);               /* Waiting termination.             */


  }
}
