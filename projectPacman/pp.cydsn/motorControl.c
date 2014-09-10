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


#include "motorControl.h"

#define STOPLEVEL 0
#define SLOWLEVEL 127
#define STARTLEVEL 255

#define LEFT_FORWARD 0x01
#define RIGHT_FORWARD 0x02
#define LEFT_BACKWARD 0x00
#define RIGHT_BACKWARD 0x00

motorState_t motorState;

void startMoving() {
    setStraightSpeed(SLOWLEVEL);
    motorState = STATE_SLOW;
}

void goFullSpeed() {
    setStraightSpeed(STARTLEVEL);
    motorState = STATE_FAST;
}

void stopMoving() {
    setStraightSpeed(STOPLEVEL);
    motorState = STATE_STOPPED;
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
    setLeftSpeed(speed);
}

void turnOnSpot() {
    setLeftSpeed(SLOWLEVEL);
    setRightSpeed(-SLOWLEVEL);
}