/* 
* Company
* Name of Project :
* Version:
* Compartible Hardware:
* Date Created: 
* Last Modified:
*/

//************************************************************LIBRARIES******************************************************************//
#include <Arduino.h>

//************************************************************DEFINTIONS*****************************************************************//
#define RANDOM_NUM 13
//*************************************************************OBJECTS*******************************************************************//

//DUAL CORE
static portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

static TaskHandle_t Core0;
static TaskHandle_t Core1;

//INTERRUPTS
hw_timer_t *timer = NULL;

//*******************************************************FUNCTION DEFINITIONS************************************************************//

//DUAL CORE
void core0Task(void *pvParameters);
void core1Task(void *pvParameters);

//***********************************************************INTERRUPTS******************************************************************//

//SOFTWARE
void IRAM_ATTR softwareInterrupt()
{
  portENTER_CRITICAL_ISR(&mux);
  //code goes here
  portEXIT_CRITICAL_ISR(&mux);
}

//HARDWARE
void IRAM_ATTR hardwareInterrupt()
{
  portENTER_CRITICAL_ISR(&mux);
  //code goes here
  portEXIT_CRITICAL_ISR(&mux);
}

//**************************************************************SETUP********************************************************************//
void setup()
{
  //SERIAL
  Serial.begin(921600);

  //SOFTWARE INTERRUPTS:

  // timer = timerBegin(0, 80, true);
  // timerAttachInterrupt(timer, &softwareInterrupt, true);
  // timerAlarmWrite(timer, 1000000, true);
  // timerAlarmEnable(timer); //Initialize Timer

  // -- HARDWARE INTERRUPTS: Uncomment to use

  //pinMode(interruptPin, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(interruptPin), hardwareInterrupt, FALLING);

  // -- DUAL CORE SETUP
  xTaskCreatePinnedToCore(
      core0Task, /* Task function. */
      "Task0",   /* name of task. */
      10000,     /* Stack size of task in Memory */
      NULL,      /* parameter of the task */
      1,         /* priority of the task: 0 to 25, 25 being the highest*/
      &Core0,    /* Task handle to keep track of created task */
      0);        /* pin task to core 0 */
  delay(500);

  xTaskCreatePinnedToCore(
      core1Task, /* Task function. */
      "Task1",   /* name of task. */
      10000,     /* Stack size of task */
      NULL,      /* parameter of the task */
      1,         /* priority of the task */
      &Core1,    /* Task handle to keep track of created task */
      1);        /* pin task to core 1 */
  delay(500);
}

//**************************************************************CORE 0*******************************************************************//
void core0Task(void *pvParameters)
{
  for (;;)
  {
    /*

      -- HOW TO USE DUAL CORE FUNCTIONALITY --

      There are three requirements you must meet when writting dual core CPU with shared resources:
        * Shared Variables          -- Memory
        * Shared Hardware Resources -- Hardware
    
        Copy this code where needed

          portENTER_CRITICAL(&mux);
          -- // Shared Memory or Variables 
          portEXIT_CRITICAL(&mux);

        If not used, your program will randomly crash, if both cpu's are trying to access the same
        recourse at once.
    
     Mateo

    */

    Serial.print("Code Running on Core: ");
    +Serial.println(xPortGetCoreID());
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}
//**************************************************************CORE 1*******************************************************************//
void core1Task(void *pvParameters)
{
  for (;;)
  {
    /*

      -- HOW TO USE DUAL CORE FUNCTIONALITY --

      There are three requirements you must meet when writting dual core CPU with shared resources:
        * Shared Variables          -- Memory
        * Shared Hardware Resources -- Hardware
    
        Copy this code where needed

          portENTER_CRITICAL(&mux);
          -- // Shared Memory or Variables 
          portEXIT_CRITICAL(&mux);

        If not used, your program will randomly crash, if both cpu's are trying to access the same
        recourse at once.
    
     Mateo

    */

    Serial.print("Code Running on Core: ");
    +Serial.println(xPortGetCoreID());
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

//***************************************************************LOOP********************************************************************//
void loop()
{
  yield();
}
//*******************************************************FUNCTION DECLARATIONS************************************************************//