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
            OSTaskSemSet(NULL, 0, &err);
            objectSecondChange_Enable();
            continue; // if objectFirst hasn't triggered, ignore
        }
        objectSecondTicks = OSTimeGet(&err);
        deltaTicks = objectSecondTicks - objectFirstTicks;
        objectFirstCheck = 0;
        if (deltaTicks > maxTicks) {
            usbprint("waited %u ticks, which is longer than our max of %u",deltaTicks,maxTicks);
            OSTaskSemSet(NULL, 0, &err);
            objectSecondChange_Enable();
            continue; // ignore stupid wait times
        
        }
        led3_toggler_Write(1);
        usbprint("all triggered successfully, waiting for %u ticks",deltaTicks);
        colour colourSelection = ColourSelectReg_Read();
        usbprint("lastSeenColour is %u, selection is %u\n",lastSeenColour,colourSelection);
        if (lastSeenColour != colourSelection) {
            led4_toggler_Write(1);
            usbprint("triggering because difference detected\n");
            deltaTicks = (deltaTicks * WAITRATIO_NUM) / WAITRATIO_DENOM;
            deltaTicks += WAITRATIO_OFFSET;
            OSTimeDly(deltaTicks, OS_OPT_TIME_DLY, &err);
            flipperDown();
            delayMS(FLIPPER_DOWN_TIME_MS);
            flipperUp();
        } else {
            usbprint("not triggering because already target colour\n");
        }
        led3_toggler_Write(0);
        led4_toggler_Write(0);
        OSTaskSemSet(NULL, 0, &err);
        objectSecondChange_Enable();
    }
}
