/* =============================================================================
 *
 * Task_Defs.h
 *
 * This file contains default options for tasks.
 * It forward-defines TCBs and stacks for all tasks.
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


#include <includes.h>
/*
Default options for demo tasks
 - No arguments are passed when the task is created
 - Tasks do not use task queues
 - Use the default time quanta (only one task can be ready at any time)
 - No TCB extensions used
*/
#define NO_TASK_ARG						((void *)0)
#define NO_TASK_Q						((OS_MSG_QTY)0)
#define DEFAULT_ROUND_ROBIN_TIME_QUANTA	((OS_TICK)0)
#define NO_TCB_EXT						((void *)0)

/*
Default options for task (and regular) semaphores
 - No timeout
 - No timestamp
*/
#define NO_SEM_TIMEOUT					((OS_TICK)0)
#define NO_SEM_TIMESTAMP				((CPU_TS *)0)

/*
OSTaskCreate arguments for all tasks
- name
- priority
- stack size
- stack limit
*/

#define MAIN_TASK				((CPU_CHAR *)"Main")
#define MAIN_TASK_PRIORITY		(4)
#define MAIN_TASK_STACK_SIZE	(256)
#define MAIN_TASK_STACK_LIMIT	(64)

extern OS_TCB	Main_Task_TCB;
extern CPU_STK	Main_Task_Stack[MAIN_TASK_STACK_SIZE];

#define ERROR_TASK              ((CPU_CHAR *)"eprint")
#define ERROR_PRIORITY          (6)
#define ERROR_STACK_SIZE        (128)
#define ERROR_STACK_LIMIT       (64)

extern OS_TCB error_TCB;
extern CPU_STK  error_stack[ERROR_STACK_SIZE];

#define DRIVING_TASK              ((CPU_CHAR *)"Driving")
#define DRIVING_PRIORITY          (6)
#define DRIVING_STACK_SIZE        (256)
#define DRIVING_STACK_LIMIT       (192)

extern OS_TCB   Driving_Task_TCB;
extern CPU_STK  Driving_Task_Stack[DRIVING_STACK_SIZE];

#define DRIVING_CONTROL_TASK      ((CPU_CHAR *)"Driving Control")
#define DRIVING_CONTROL_PRIORITY  (3)
#define DRIVING_CONTROL_STACK_SIZE   (96)
#define DRIVING_CONTROL_STACK_LIMIT  (80)

extern OS_TCB   Driving_Control_TCB;
extern CPU_STK  Driving_Control_Stack[DRIVING_CONTROL_STACK_SIZE];

#define FLIPPER_TASK              ((CPU_CHAR *)"Flipper")
#define FLIPPER_PRIORITY          (2)
#define FLIPPER_STACK_SIZE        (128)
#define FLIPPER_STACK_LIMIT       (96)

extern OS_TCB   Flipper_Task_TCB;
extern CPU_STK  Flipper_Task_Stack[FLIPPER_STACK_SIZE];

#define DODGEM_TASK                 ((CPU_CHAR *)"Dodging")
#define DODGEM_PRIORITY             (5)
#define DODGEM_STACK_SIZE           (256)
#define DODGEM_STACK_LIMIT          (192)

extern OS_TCB Dodgem_Task_TCB;
extern CPU_STK Dodgem_Task_Stack[DODGEM_STACK_SIZE];

/*
Forward definitions of all task start functions.
*/
void Main_Task( void * );

void error_print_task(void*) ;

void Driving_Task(void*) ;

void Driving_Control_Task(void*) ;

void Flipper_Task(void*);

void Dodgem_Task(void*);