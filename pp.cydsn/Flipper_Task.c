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
#include "arbitrary_constants.h"

extern colour targetColour;
extern colour lastSeenColour;

extern uint8_t gotUSB;

OS_TCB   Flipper_Task_TCB;
CPU_STK  Flipper_Task_Stack[FLIPPER_STACK_SIZE];

OS_TICK objectFirstTicks;
int objectFirstCheck;

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
    OS_TICK objectSecondTicks;
    OS_TICK deltaTicks;
    OS_TICK maxTicks = 1200;
    objectFirstCheck = 0;
    while (DEF_ON) {
        OSTaskSemPend(0, OS_OPT_PEND_BLOCKING, &ts, &err);
        objectSecondChange_Disable();
        
        if (!objectFirstCheck) {
            usbprint("triggered objectSecond without objectFirst\n");
            continue; // if objectFirst hasn't triggered, ignore
        }
        objectSecondTicks = OSTimeGet(&err);
        deltaTicks = objectSecondTicks - objectFirstTicks;
        objectFirstCheck = 0;
        if (deltaTicks > maxTicks) {
            usbprint("waited %u ticks, which is longer than our max of %u",deltaTicks,maxTicks);
            continue; // ignore stupid wait times
        
        }
        usbprint("all triggered successfully, waiting for %u ticks",deltaTicks);
        colour colourSelection = ColourSelectReg_Read();
        usbprint("lastSeenColour is %u, selection is %u\n",lastSeenColour,colourSelection);
        if (lastSeenColour != colourSelection) {
            deltaTicks = (deltaTicks / WAITRATIO_DENOM) * WAITRATIO_NUM;
            deltaTicks += WAITRATIO_OFFSET;
            OSTimeDly(deltaTicks, OS_OPT_TIME_DLY, &err);
            flipperDown();
            delayMS(FLIPPER_DOWN_TIME_MS);
            flipperUp();
        }
        OSTaskSemSet(NULL, 0, &err);
        objectSecondChange_Enable();
    }
}
