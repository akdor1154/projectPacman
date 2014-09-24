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

#ifndef ARBCONSTS
#define ARBCONSTS
    
//these are the 'logic low' proportions, out of 65536 ticks per cycle, of the pwm cycle to the servo for the relevant state.
#define FLIPPER_UP 62000
#define FLIPPER_DOWN 61500

//wait times to trigger the flipper -- down_delay is time before down, down_time is time before up again
#define FLIPPER_DOWN_DELAY_MS 230
#define FLIPPER_DOWN_TIME_MS 400

//pwm duty cycles out of 255 for some useful motor speeds.
#define STOPLEVEL 0
#define SLOWLEVEL 92
#define STARTLEVEL 255

//how much response the IR sensors need (low = far, high = close) before generating an interrupt
#define DODGE_INTERRUPT_THRESHOLD 90
#define DODGE_HEADON_INTERRUPT_THRESHOLD 80
//how much the IR sensors need for the interrupt routine to interpret there is a wall in front of a sensor.
//you want this < INTERRUPT_THRESHOLD above. (if it's equal to the interrupt threshold,
//then you will probably never detect "wall dead ahead", as one sensor will almost certainly always fire first).
#define DODGE_THRESHOLD 45
#define DODGE_HEADON_THRESHOLD 40

#endif