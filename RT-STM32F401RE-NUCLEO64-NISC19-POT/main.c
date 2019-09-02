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




/*===========================================================================*/
/* ADC related code                                                          */
/*===========================================================================*/
/*
 * In this demo we want to use a single channel to sample voltage across
 * the potentiometer.
 */
#define MY_NUM_CH                                              1
#define MY_SAMPLING_NUMBER                                    10

static adcsample_t sample_buff[MY_NUM_CH * MY_SAMPLING_NUMBER];

/*
 * ADC conversion group.
 * Mode:        Linear buffer, 10 samples of 1 channel, SW triggered.
 * Channels:    IN0.
 */
static const ADCConversionGroup my_conversion_group = {
  FALSE,                            /* NOT CIRCULAR */
  MY_NUM_CH,                        /* NUMB OF CH */
  NULL,                             /* NO ADC CALLBACK */
  NULL,                             /* NO ADC ERROR CALLBACK */
  0,                                /* CR1 */
  ADC_CR2_SWSTART,                  /* CR2 */
  0,                                /* SMPR1 */
  ADC_SMPR2_SMP_AN0(ADC_SAMPLE_144),/* SMPR2 */
  0,                                /* HTR */
  0,                                /* LTR */
  ADC_SQR1_NUM_CH(MY_NUM_CH),       /* SQR1 */
  0,                                /* SQR2 */
  ADC_SQR3_SQ1_N(ADC_CHANNEL_IN0)   /* SQR3 */
};





/*===========================================================================*/
/*  end of definitions  from demo 18                                         */
/*===========================================================================*/


/*===========================================================================*/
/*  thread from   from demo 18                                         */
/*===========================================================================*/
static THD_WORKING_AREA(waThd3, 512);
static THD_FUNCTION(Thd3, arg) {
  unsigned ii;
(void) arg;
  chRegSetThreadName("ADC handler");

  /*
   * Setting up analog inputs used by the demo.
   */
  palSetPadMode(GPIOA, 0, PAL_MODE_INPUT_ANALOG);

  /*
   * Activates the ADC1 driver.
   */
  adcStart(&ADCD1, NULL);
  while(!chThdShouldTerminateX()) {
                  adcConvert(&ADCD1, &my_conversion_group, sample_buff, MY_SAMPLING_NUMBER);

                  /* Making mean of sampled values. Note that samples refers to OTA and OTB
                     but since they we are looking for Rcm (common mode) we can make a simple
                     mean */
                  mean = 0;
                  for (ii = 0; ii < MY_NUM_CH * MY_SAMPLING_NUMBER; ii++) {
                    mean += sample_buff[ii];
                  }
                  mean /= MY_NUM_CH * MY_SAMPLING_NUMBER;
                  lastvalue = ((float)mean) * 100.0 / 4095.0;
                  flag = TRUE;

                  chThdSleepMilliseconds(100);
     }
  chThdExit(0);

}



/*===========================================================================*/
/*  end of thread from   from demo 18                                        */
/*===========================================================================*/



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
    thread_t *tp = chThdCreateStatic(waThd2, sizeof(waThd2), NORMALPRIO + 1,Thd2,NULL);


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
           if (flag2 == false){
                       if(!strcmp("LED",testvar)&& argc<=3){
                                   if(!strcmp("BLUE",testvar1)  ){
                                          if(!strcmp("ON",testvar2)){
                                            palClearLine(LINE_LED_RGB_BLUE);
                                            }
                                          else {
                                             palSetLine(LINE_LED_RGB_BLUE);
                                            }

                                      }
                                   else if (!strcmp("RED",testvar1) ){
                                           if(!strcmp("ON",testvar2)){
                                              palClearLine(LINE_LED_RGB_RED);
                                              }
                                           else {
                                              palSetLine(LINE_LED_RGB_RED);
                                              }
                                       }
                                   else if (!strcmp("GREEN",testvar1) ) {
                                            if(!strcmp("ON",testvar2)){
                                              palClearLine(LINE_LED_RGB_GREEN);
                                             }
                                            else {
                                              palSetLine(LINE_LED_RGB_GREEN);
                                             }
                                        }
                                   else
                                     {chprintf(chp, "wrong command \r\n" );}
                       }
                      else if (!strcmp("POT",testvar) ){
                               if(!strcmp("ON",testvar1)){
                                   chprintf(chp, "Last value: %.2f\r\n", lastvalue);
                                   }
                                 else {
                                       chprintf(chp, "pot sampling off\r\n" );
                                      }
                       }

                      else if (!strcmp("PWM",testvar)  ){
                                  if(!strcmp("BLUE",testvar1)  ){
                                            chprintf(chp, "Last value: %.2f\r\n", lastvalue);
                                     }
                                  else if (!strcmp("RED",testvar1) ){
                                           chprintf(chp, "Last value: %.2f\r\n", lastvalue);
                                      }
                                  else if (!strcmp("GREEN",testvar1) ) {
                                           chprintf(chp, "Last value: %.2f\r\n", lastvalue);

                                       }
                                  else
                                    {chprintf(chp, "wrong command \r\n" );}


                         }
                      else{
                        chprintf(chp, "boh" );
                      }

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

 chThdCreateStatic(waThd3, sizeof(waThd3), NORMALPRIO + 1,Thd3,NULL);
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
