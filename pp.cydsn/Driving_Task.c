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

/* Include PSoC system and component APIs and defines */
#include <project.h>

/* Include application function declarations and defines */
#include <Task_Defs.h>

#include "motorControl.h"
#include "usbprint.h"
#include "jarrad_util.h"

/* Main_Task TCB, start function and stack */
OS_TCB	Driving_Task_TCB;
CPU_STK	Driving_Task_Stack[DRIVING_STACK_SIZE];

OS_ERR err;			/* Hold OS call return code */
CPU_TS ts;

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
    

void setStraightSpeed(uint8_t speed) {
    setLeftSpeed(speed);
    setRightSpeed(speed);
}

void turnOnSpot() {
    setLeftSpeed(SLOWLEVEL);
    setRightSpeed(-SLOWLEVEL);
}

void changeMotorState(motorState_t targetState) {
    motorState = targetState;
    OSTaskSemPost(
        &Driving_Control_TCB, 
        OS_OPT_POST_NONE,
        &err
    );
}


void Driving_Task(void* p_arg) {
    (void)p_arg;
    switch (motorState) {
        case STATE_SLOW:
            startMoving();
            break;
        case STATE_FAST:
            goFullSpeed();
            break;
        case STATE_TURNING:
            turnOnSpot();
            break;
        case STATE_DEMO: 
            while (DEF_ON) {
                turnOnSpot();
                delaySeconds(1);
                startMoving();
                delaySeconds(2);
            }
            break;
        default:
        case STATE_STOPPED:
            stopMoving();
            break;
    }
    
    OSTaskSuspend(NULL, &err);
}