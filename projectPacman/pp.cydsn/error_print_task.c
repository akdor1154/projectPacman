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

void error_print_task( void *p_arg )
{
	OS_ERR err;			/* Hold OS call return code */
    
	(void)p_arg;		/* no-op prevents warning about unused p_arg */

    
    OS_RATE_HZ tenthSecond = OSCfg_TickRate_Hz/10;
    while (1) {
    	OSTimeDly(
        	tenthSecond,
           	OS_OPT_TIME_DLY,
    	&err );
        
        usbprint("global error: %u\n",global_err);
    }
    
    
}