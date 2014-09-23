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


#include <includes.h>
#include "Task_Defs.h"
#include "flipper.h"
#include "jarrad_util.h"
#include "usbprint.h"

extern colour targetColour;
extern colour lastSeenColour;

extern uint8_t gotUSB;

OS_TCB   Flipper_Task_TCB;
CPU_STK  Flipper_Task_Stack[FLIPPER_STACK_SIZE];


void flipperUp() {
    if (gotUSB) return;
    ServoPWM_WriteCompare(FLIPPER_UP);
}

void flipperDown() {
    if (gotUSB) return;
    ServoPWM_WriteCompare(FLIPPER_DOWN);
}

void Flipper_Task(void* UNUSED(taskArgs)) {
    CPU_TS ts;
    OS_ERR err;
    
    while (DEF_ON) {
        OSTaskSemPend(0, OS_OPT_PEND_BLOCKING, &ts, &err);
        objectChange_Disable();
        colour colourSelection = ColourSelectReg_Read();
        usbprint("lastSeenColour is %u, selection is %u\n",lastSeenColour,colourSelection);
        if (((uint8_t)lastSeenColour) != colourSelection) {
            delayMS(FLIPPER_DOWN_DELAY_MS);
            flipperDown();
            delayMS(FLIPPER_DOWN_TIME_MS);
            flipperUp();
        }
        OSTaskSemSet(NULL, 0, &err);
        objectChange_Enable();
    }
}
