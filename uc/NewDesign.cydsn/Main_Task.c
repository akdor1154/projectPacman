/* =============================================================================
 *
 * Main_Task.c
 *
 * This file contains the start function for Main_Task.
 *
 * v1.0	YFS		Initial release.
 *
 * =============================================================================
*/

/*
* Copyright (2013), Cypress Semiconductor Corporation. All Rights Reserved.
*
* This software is owned by Cypress Semiconductor Corporation (Cypress)
* and is protected by and subject to worldwide patent protection (United
* States and foreign), United States copyright laws and international treaty
* provisions. Cypress hereby grants to licensee a personal, non-exclusive,
* non-transferable license to copy, use, modify, create derivative works of,
* and compile the Cypress Source Code and derivative works for the sole
* purpose of creating custom software in support of licensee product to be
* used only in conjunction with a Cypress integrated circuit as specified in
* the applicable agreement. Any reproduction, modification, translation,
* compilation, or representation of this software except as specified above 
* is prohibited without the express written permission of Cypress.
*
* Disclaimer: CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH 
* REGARD TO THIS MATERIAL, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
* Cypress reserves the right to make changes without further notice to the 
* materials described herein. Cypress does not assume any liability arising out 
* of the application or use of any product or circuit described herein. Cypress 
* does not authorize its products for use as critical components in life-support 
* systems where a malfunction or failure may reasonably be expected to result in 
* significant injury to the user. The inclusion of Cypress' product in a life-
* support systems application implies that the manufacturer assumes all risk of 
* such use and in doing so indemnifies Cypress against all charges. 
*
* Use of this Software may be limited by and subject to the applicable Cypress
* software license agreement. 
*/

/* Include Micrium APIs and defines */
#include <includes.h>

/* Include PSoC system and component APIs and defines */
#include <project.h>

/* Include application function declarations and defines */
#include <Task_Defs.h>

#include "usbprint.h"

/* Main_Task TCB, start function and stack */
OS_TCB	Main_Task_TCB;
CPU_STK	Main_Task_Stack[MAIN_TASK_STACK_SIZE];

/*
 * Function:		Main_Task
 *
 * This function runs as a uC/OS-III task with a low priority.
 * It toggles LED3 on/off once a second.
 *
 *			Start task
 *			Complete OS initialization
 *			Set LED3 to be under software control
 *		 -> Toggle LED3
 *		|	Delay a second
 *		 -- Loop
 * 
 * Globals:		Uses OSCfg_TickRate_Hz to create 1Hz LED flash rate
 *
 * Parameters:	p_arg (not used)
 *
 * Return:		None (infinite loop)
 */
void Main_Task( void *p_arg )
{
	OS_ERR err;			/* Hold OS call return code */
	
	(void)p_arg;		/* no-op prevents warning about unused p_arg */
	
	/* Perform BSP post-initialization functions */
	BSP_PostInit();
    
	/* Perform PSoC Tick Initialization */
    BSP_CPU_TickInit();

#if (OS_CFG_STAT_TASK_EN > 0u)
    OSStatTaskCPUUsageInit(&err);
#endif    

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif 	

	/* Init the LED controller - enable software control */
	LED3_Control_Set_Channel( LED3_Control_USE_IN1_AND_IN2 );
	LED4_Control_Set_Channel( LED4_Control_USE_IN1_AND_IN2 );

    OS_RATE_HZ tenthSecond = OSCfg_TickRate_Hz/10;
    
    uint8_t pwmLevel = 0;
    uint8_t pwmStep = UINT8_MAX/16;
    
    
	while (1) {
		/* Delay 1s */
		OSTimeDly(
			tenthSecond,
           	OS_OPT_TIME_DLY,
			&err );
		
		/* Toggle the LED3 */
        
        PWM_1_WriteCompare(pwmLevel);
        PWM_2_WriteCompare(UINT8_MAX-pwmLevel);
        pwmLevel += pwmStep;
        
        
		led3_toggler_Write(!led3_toggler_Read());
        led4_toggler_Write(!led4_toggler_Read());
        usbprint("led3 is %u, pwmlevel is %u\n",led3_toggler_Read(),pwmLevel);
        
			
	}
	
} /* Main_Task */
