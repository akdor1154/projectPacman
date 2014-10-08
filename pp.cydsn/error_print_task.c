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
OS_TCB	error_TCB;
CPU_STK	error_stack[ERROR_STACK_SIZE];

extern OS_ERR global_err;
extern uint8_t* proxLeft;
extern uint8_t* proxRight;
extern uint8_t* objectUnder;
extern uint8_t* colourUnder;

void error_print_task( void *p_arg )
{
	OS_ERR err;			/* Hold OS call return code */
    CPU_TS ts;
    
	(void)p_arg;		/* no-op prevents warning about unused p_arg */
    
    while (DEF_ON) {
    	/*OSTimeDly(
        	tenthSecond,
           	OS_OPT_TIME_DLY,
    	&err );*/
        OSTaskSemPend(0, OS_OPT_PEND_BLOCKING, &ts, &err);
        
        usbprint("Left: %u\nRight: %u\nCentre: %u\nobject1: %u\nobject2: %u\ncolour: %u\n\n ",proxLeftReg_Read(),proxRightReg_Read(),proxCentreReg_Read(),objectFirstReg_Read(),objectSecondReg_Read(),colourReg_Read());
 
    }
    
    
}