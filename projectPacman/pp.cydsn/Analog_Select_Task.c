/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

/* [] END OF FILE */

/* Include Micrium APIs and defines */
#include <includes.h>

/* Include PSoC system and component APIs and defines */
#include <project.h>

/* Include application function declarations and defines */
#include <Task_Defs.h>

#include "usbprint.h"

/* Main_Task TCB, start function and stack */
OS_TCB	Analog_Select_TCB;
CPU_STK	Analog_Select_Stack[ANALOG_SELECT_STACK_SIZE];

OS_ERR err;			/* Hold OS call return code */
CPU_TS ts;

uint8_t analogValues[4];

uint8_t* colourUnder = &analogValues[0];
uint8_t* objectUnder = &analogValues[1];
uint8_t* proxLeft = &analogValues[2];
uint8_t* proxRight = &analogValues[3];

void Analog_Select_Task(void* args) {
    uint8_t analogSource;
    uint8_t analogRead;
    SensorADC_Start();
    
    (void) args;
    while (DEF_ON) {
        OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,&ts,&err);
        
        analogSource = analogSelectReg_Read();
        analogRead =  (uint8_t)SensorADC_GetResult8();
        analogValues[analogSource] = analogRead;
        //usbprint("just wrute %u to %u\n\n",analogRead,analogSource);
    }
    
}
