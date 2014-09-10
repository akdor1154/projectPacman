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
    
typedef enum {STATE_STOPPED, STATE_SLOW, STATE_FAST} motorState_t;
void startMoving();
void goFullSpeed();
void stopMoving();
#endif

void setStraightSpeed(uint8_t speed);
void setLeftSpeed(int16_t speed);
void setRightSpeed(int16_t speed);
void turnOnSpot();