/* =============================================================================
 *
 * main.c
 *
 * This file contains the main() function.
 *
 * v1.0	YFS	Initial release.
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

OS_ERR global_err;

/*
 * Function:	main
 *
 * Initialize the system, create the first task (MainTask) and start the OS.
 *
 * Globals:		Uses MainTask_TCB and Main_Task_Stack
 *
 * Parameters:	None
 *
 * Return:		None (OSStart API never returns)
 */

unsigned int pixelCount;

uint8_t yPix;
uint8_t cbPix;
uint8_t crPix;






void main( void )
{
    OS_ERR  err;
    global_err = 0;
    
    pixelCount = 0;
    
    
	/* Perform BSP pre-initialization (post-init occurs in MainTask) */
    BSP_PreInit();
    
    
	/* Initialize the uC/CPU services */
    CPU_Init();

	/* Initialize the OS - make it ready to create the task, below */
    OSInit( &err );
    
    
    CYGlobalIntEnable;
    
    SW2_Interrupt_Start();
    int_pixelReady_Start();
    
    USB_Start(0,USB_DWR_VDDD_OPERATION);
    while(!USB_GetConfiguration());
    USB_CDC_Init();
    
    err = 0;
	/* Create the task - it will not run until OSStart */
	OSTaskCreate(
		&Main_Task_TCB,
		MAIN_TASK,
		Main_Task,
		NO_TASK_ARG,
		MAIN_TASK_PRIORITY,
		Main_Task_Stack,
		MAIN_TASK_STACK_LIMIT,
		MAIN_TASK_STACK_SIZE,
		NO_TASK_Q,
		DEFAULT_ROUND_ROBIN_TIME_QUANTA,
		NO_TCB_EXT,
		OS_OPT_TASK_STK_CHK,
		&err );
    
    err = 0;
    usbprint("mainerr: %u",err);
    /*OSTaskCreate(
		&error_TCB,
		ERROR_TASK,
		error_print_task,
		NO_TASK_ARG,
		ERROR_PRIORITY,
		error_stack,
		ERROR_STACK_LIMIT,
		ERROR_STACK_SIZE,
		NO_TASK_Q,
		DEFAULT_ROUND_ROBIN_TIME_QUANTA,
		NO_TCB_EXT,
		OS_OPT_TASK_STK_CHK,
		&err );
    
    led1_toggler_Write((
        //(err == OS_ERR_NONE) |
        (err == OS_ERR_PRIO_INVALID)
        //(err == OS_ERR_STK_INVALID) |
        //(err == OS_ERR_STK_SIZE_INVALID) |
        //(err == OS_ERR_TASK_CREATE_ISR) |
        //(err == OS_ERR_TASK_INVALID) |
        //(err == OS_ERR_TCB_INVALID)
        //(err == OS_ERR_ILLEGAL_CREATE_RUN_TIME)
    ));*/
    usbprint("errorerr: %u",err);
    
    
	/* Start multitasking (give control to uC/OS-III) - never returns */
    OSStart( &err );                                            
	
} /* main() */
