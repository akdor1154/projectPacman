/*
 *  Driving_Task.c
 *
 *  Drives according to some mode.
 *  This task is restarted by Driving_Control_Task.c
 *  when the driving mode needs to be changed.
 *
 *******************************************************/


/* Include Micrium APIs and defines */
#include <includes.h>
#include <lib_math.h>

/* Include PSoC system and component APIs and defines */
#include <project.h>
#include <stdlib.h>

/* Include application function declarations and defines */
#include <Task_Defs.h>

#include "motorControl.h"
#include "usbprint.h"
#include "jarrad_util.h"
#include "arbitrary_constants.h"

OS_ERR err;			/* Hold OS call return code */
CPU_TS ts;

int randSeeded = 0;

int perturbation = 0;
int lastPerturbation = 0;

motorState_t motorState;

void startMoving() {
    setStraightSpeed(SLOWLEVEL);
}

void goFullSpeed() {
    setStraightSpeed(STARTLEVEL);
}

void stopMoving() {
    setStraightSpeed(STOPLEVEL);
}

void setLeftSpeed(int16_t speed) {
    uint8_t directions = MotorDirection_Read();
    uint8_t absSpeed;
    if (speed < 0) {
        speed = -speed;
        directions = directions & (~LEFT_FORWARD);
    } else {
        directions = directions | LEFT_FORWARD;
    }
    absSpeed = (speed > 255) ? 255 : speed;
    MotorDirection_Write(directions);
    LeftMotorPWM_WriteCompare(absSpeed);
}

void setRightSpeed(int16_t speed) {
    uint8_t directions = MotorDirection_Read();
    uint8_t absSpeed;
    if (speed < 0) {
        speed = -speed;
        directions = directions & (~RIGHT_FORWARD);
    } else {
        directions = directions | RIGHT_FORWARD;
    }
    absSpeed = (speed > 255) ? 255 : speed;
    MotorDirection_Write(directions);
    RightMotorPWM_WriteCompare(absSpeed);
}

#define walkBottom (SLOWLEVEL - 32)
#define walkTop (SLOWLEVEL + 32)

void setStraightSpeed(uint8_t speed) {
    setLeftSpeed(speed);
    setRightSpeed(speed);
}

void turnOnSpot(int16_t speed) {
    setLeftSpeed(speed);
    setRightSpeed(-speed);
}

void changeMotorState(motorState_t targetState) {
    motorState = targetState;
    OSTaskSemPost(
        &Driving_Control_TCB, 
        OS_OPT_POST_NONE,
        &err
    );
}

OS_TCB	Driving_Task_TCB;
CPU_STK	Driving_Task_Stack[DRIVING_STACK_SIZE];

void Driving_Task(void* UNUSED(p_arg)) {
    
    int perturbation;
    RAND_NBR random;
    
    int16_t leftSpeed;
    int16_t rightSpeed;
    
    proxLateralThreshold_Write(DODGE_INTERRUPT_THRESHOLD);
    proxCentreLow_Write(DODGE_HEADON_INTERRUPT_THRESHOLD);
    
    if (!randSeeded) {    
        Math_RandSetSeed(OSTimeGet(&err));
        randSeeded = 1;
    }
    switch (motorState) {
        case STATE_SLOW:
            startMoving();
            break;
        case STATE_FAST:
            goFullSpeed();
            break;
        case STATE_TURNING:
            turnOnSpot(SLOWLEVEL);
            break;
        case STATE_DEMO:
            proxChange_Start();
            proxChange_Enable();
            startMoving();
            leftSpeed = SLOWLEVEL;
            rightSpeed = SLOWLEVEL;
            while (DEF_ON) {
                random = Math_Rand(); // rand is alternating perfectly between even and odd numbers...
                perturbation = (signed int) ( random & 0x1F ); // take the last 5 bits of a random number, equivalent to rand() % 32
                
                usbprint("perturbation is %i\n",perturbation);
                if (((random) ^ (random >> 4)) & 0x08) { // all I want is a random bit; why must I go and swear "screw it"; my face I want to hit; oh wouldn't it be lovely..
                    perturbation = -perturbation;
                }
                perturbation += (lastPerturbation/2);
                usbprint("flipped perturbation is %i\n",perturbation);
                delaySeconds(1);
                OSSchedLock(&err);
                leftSpeed = leftSpeed - perturbation;
                rightSpeed = rightSpeed + perturbation;
                leftSpeed = (leftSpeed < walkBottom) ? walkBottom : leftSpeed;
                rightSpeed = (rightSpeed < walkBottom) ? walkBottom : rightSpeed;
                leftSpeed = (leftSpeed > walkTop) ? walkTop : leftSpeed;
                rightSpeed = (rightSpeed > walkTop) ? walkTop : rightSpeed;
                setLeftSpeed(leftSpeed);
                setRightSpeed(rightSpeed);
                OSSchedUnlock(&err);
                usbprint("set speed to %i, %i",leftSpeed,rightSpeed);
                lastPerturbation = perturbation;
            }
            while (DEF_ON) {
                turnOnSpot(SLOWLEVEL);
                delaySeconds(1);
                startMoving();
                delaySeconds(2);
            }
            break;
        default:
        case STATE_STOPPED:
            proxChange_Disable();
            stopMoving();
            break;
    }
    
    OSTaskSuspend(NULL, &err);
}

OS_TCB	Dodgem_Task_TCB;
CPU_STK	Dodgem_Task_Stack[DODGEM_STACK_SIZE];

//when our proximity sensors fire, this task takes over the wheel(s).
void Dodgem_Task(void* UNUSED(args)) {
    OS_ERR taskErr;
    CPU_TS taskTs;
    
    uint8_t proxLeft;
    uint8_t proxRight;
    uint8_t proxCentre;
    
    int suspendedDriving;
    
    while (DEF_ON) {
        OSTaskSemPend(0, OS_OPT_PEND_BLOCKING, &taskTs, &taskErr);
        //proxChange_Disable();
        usbprint("pend error: %u\n",taskErr);
        suspendedDriving = 0;
        OSTaskSuspend(&Driving_Task_TCB, &taskErr);
        if (taskErr != OS_ERR_NONE) {
            usbprint("suspend error: %u\n",taskErr);
            suspendedDriving = 0;
        } else {
            suspendedDriving += 1;
        }
        proxLeft = proxLeftReg_Read();
        proxRight = proxRightReg_Read();
        proxCentre = proxCentreReg_Read();
        
        if (proxCentre > DODGE_HEADON_THRESHOLD) {
            if (proxLeft > proxRight) {
                usbprint("180 backwards CCW\n");
                setLeftSpeed(-SLOWLEVEL);
                setRightSpeed(-10);
            } else {
                usbprint("180 backwards CW\n");
                setLeftSpeed(-10);
                setRightSpeed(-SLOWLEVEL);
            }
            delayMS(U_TURN_TIME_MS);
        } else if (proxLeft > DODGE_THRESHOLD) {
            usbprint("holding right motor\n");
            setRightSpeed(-1);
            delayMS(SWERVE_TIME_MS);
        } else if (proxRight > DODGE_THRESHOLD) {
            usbprint("holding left motor\n");
            setLeftSpeed(-1);
            delayMS(SWERVE_TIME_MS);
        }
        OSSchedLock(&err);
        for (int i = 0; i<suspendedDriving; i++) {
            perturbation = 0;
            lastPerturbation = 0;
            OSTaskResume(&Driving_Task_TCB, &taskErr);
            if (taskErr != OS_ERR_NONE) {
                usbprint("resume error: %u\n",taskErr);
            }
        }
        OSSchedUnlock(&err);
        //OSTaskSemSet(NULL, 0, &taskErr);
        //proxChange_Enable();
    }
}