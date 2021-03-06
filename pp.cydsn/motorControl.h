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
#ifndef motorControl_h
#define motorControl_h

#include <includes.h>
#include "project.h"
#include "arbitrary_constants.h"
    
typedef enum {
    STATE_STOPPED,
    STATE_SLOW,
    STATE_FAST,
    STATE_TURNING,
    STATE_DEMO 
} motorState_t;


#define LEFT_FORWARD 0x01
#define RIGHT_FORWARD 0x02
#define LEFT_BACKWARD 0x00
#define RIGHT_BACKWARD 0x00

void startMoving();
void goFullSpeed();
void stopMoving();

void changeMotorState(motorState_t targetState);

void setStraightSpeed(int16_t speed);
void setLeftSpeed(int16_t speed);
void setRightSpeed(int16_t speed);
void turnOnSpot(int16_t speed);

#endif