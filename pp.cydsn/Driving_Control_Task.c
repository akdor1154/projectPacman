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
//#include <project.h>

/* Include application function declarations and defines */
#include <Task_Defs.h>

#include "usbprint.h"

/* Main_Task TCB, start function and stack */
OS_TCB	Driving_Control_TCB;
CPU_STK	Driving_Control_Stack[DRIVING_CONTROL_STACK_SIZE];

OS_ERR err;			/* Hold OS call return code */
CPU_TS ts;

void Driving_Control_Task(void* p_arg) {
    (void) p_arg;
    while (DEF_ON) {
        OSTaskSemPend(
            0,
            OS_OPT_PEND_BLOCKING,
            &ts,
            &err
        );
        
        OSTaskDel(&Driving_Task_TCB, &err);
        
        OSTaskCreate(
    		&Driving_Task_TCB,
    		DRIVING_TASK,
    		Driving_Task,
    		NO_TASK_ARG,
    		DRIVING_PRIORITY,
    		Driving_Task_Stack,
    		DRIVING_STACK_LIMIT,
    		DRIVING_STACK_SIZE,
    		NO_TASK_Q,
    		DEFAULT_ROUND_ROBIN_TIME_QUANTA,
    		NO_TCB_EXT,
    		OS_OPT_TASK_STK_CHK,
    		&err
        );        
    }
    
}

