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
#include "camera.h"
#include "motorControl.h"
#include "flipper.h"
#include "jarrad_util.h"

/* Main_Task TCB, start function and stack */
OS_TCB	Main_Task_TCB;
CPU_STK	Main_Task_Stack[MAIN_TASK_STACK_SIZE];

extern unsigned int pixelCount;

extern uint8_t YTest1[480];
extern uint8_t YTest2[480];

extern uint8_t rPix;
extern uint8_t gPix;
extern uint8_t bPix;
extern motorState_t motorState;

extern uint8_t gotUSB;
extern colour targetColour;

uint8_t analogValues[4];

uint8_t* colourUnder = &analogValues[0];
uint8_t* objectUnder = &analogValues[1];
uint8_t* proxLeft = &analogValues[2];
uint8_t* proxRight = &analogValues[3];

typedef struct {
    uint8 channel;
    uint8 TD;
    uint8 (*dmaInit)(uint8, uint8, uint16, uint16);
    reg8* dest;
} SensorDMA;


/* DMA Configuration for colourDMA */
#define SensorDMA_BYTES_PER_BURST 1
#define SensorDMA_REQUEST_PER_BURST 1
#define SensorDMA_SRC_BASE (CYDEV_PERIPH_BASE)
#define SensorDMA_DST_BASE (CYDEV_PERIPH_BASE)

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
void Main_Task( void* UNUSED(p_arg) )
{
	OS_ERR err;			/* Hold OS call return code */
	CPU_TS ts;
    
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

    OSTaskCreate(
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
    
    OSTaskCreate(
		&Driving_Control_TCB,
		DRIVING_CONTROL_TASK,
		Driving_Control_Task,
		NO_TASK_ARG,
		DRIVING_CONTROL_PRIORITY,
		Driving_Control_Stack,
		DRIVING_CONTROL_STACK_LIMIT,
		DRIVING_CONTROL_STACK_SIZE,
		NO_TASK_Q,
		DEFAULT_ROUND_ROBIN_TIME_QUANTA,
		NO_TCB_EXT,
		OS_OPT_TASK_STK_CHK,
		&err );
    
    OSTaskCreate(
		&Flipper_Task_TCB,
		FLIPPER_TASK,
		Flipper_Task,
		NO_TASK_ARG,
		FLIPPER_PRIORITY,
		Flipper_Task_Stack,
		FLIPPER_STACK_LIMIT,
		FLIPPER_STACK_SIZE,
		NO_TASK_Q,
		DEFAULT_ROUND_ROBIN_TIME_QUANTA,
		NO_TCB_EXT,
		OS_OPT_TASK_STK_CHK,
		&err );
    
    OSTaskCreate(
        &Dodgem_Task_TCB,
        DODGEM_TASK,
        Dodgem_Task,
        NO_TASK_ARG,
        DODGEM_PRIORITY,
        Dodgem_Task_Stack,
        DODGEM_STACK_LIMIT,
        DODGEM_STACK_SIZE,
        NO_TASK_Q,
        DEFAULT_ROUND_ROBIN_TIME_QUANTA,
        NO_TCB_EXT,
        OS_OPT_TASK_STK_CHK,
        &err );
    
    usbprint("tasks created\n\n");
    
    changeMotorState(STATE_STOPPED);

    uint8_t numSensors = 6;
    SensorDMA sensorDMAs[] = {
        {.dmaInit = &colourDMA_DmaInitialize, .dest = colourReg_Control_PTR},
        {.dmaInit = &objectFirstDMA_DmaInitialize, .dest = objectFirstReg_Control_PTR},
        {.dmaInit = &proxLeftDMA_DmaInitialize, .dest = proxLeftReg_Control_PTR},
        {.dmaInit = &proxRightDMA_DmaInitialize, .dest = proxRightReg_Control_PTR},
        {.dmaInit = &proxCentreDMA_DmaInitialize, .dest = proxCentreReg_Control_PTR},
        {.dmaInit = &objectSecondDMA_DmaInitialize, .dest = objectSecondReg_Control_PTR}
    };
    
    for (int i = 0; i<numSensors; i++) {
        sensorDMAs[i].channel = (*sensorDMAs[i].dmaInit)(
            SensorDMA_BYTES_PER_BURST,
            SensorDMA_REQUEST_PER_BURST,
            HI16(SensorDMA_SRC_BASE),
            HI16(SensorDMA_DST_BASE)
        );
        
        sensorDMAs[i].TD = CyDmaTdAllocate();
        CyDmaTdSetConfiguration(sensorDMAs[i].TD, 1, sensorDMAs[i].TD, 0);
        CyDmaTdSetAddress(sensorDMAs[i].TD, LO16((uint32)SensorADC_SAR_WRK0_PTR), LO16((uint32)sensorDMAs[i].dest));
        CyDmaChSetInitialTd(sensorDMAs[i].channel, sensorDMAs[i].TD);
        CyDmaChEnable(sensorDMAs[i].channel, 1);
    }
    
    //make sure we are stopped at init
    motorState = 0;
    
    //set our colour thresholds
    colourBlue_High_Write(BLUE_MAX);
    colourRed_Low_Write(RED_MIN);
    
    //set puck detection threshold
    object_Low_Write(PUCK_THRESHOLD);
    
    //start PWMs
    PWM_1_Start();
    LeftMotorPWM_Start();
    RightMotorPWM_Start();
    
    //servo likes to pull huge currents from its data line if it doesn't have power -.-
    if (!gotUSB) ServoPWM_Start();
    
    flipperUp();
    
    //start the ADC
    SensorADC_Start();
    
    //enable all interrupts
    SW2_Interrupt_Start();
    SW3_Interrupt_Start();
    colourChange_Start();
    objectFirstChange_Start();
    objectSecondChange_Start();
    
    changeMotorState(STATE_DEMO);
    
	while (DEF_ON) {
		
        usbprint("waiting...\n");
        OSTaskSemPend(
            0,
            OS_OPT_PEND_BLOCKING,
            &ts,
            &err
        );
        
        usbprint("Left: %u\nRight: %u\nCentre: %u\nobject1: %u\nobject2: %u\ncolour: %u\n\n ",proxLeftReg_Read(),proxRightReg_Read(),proxCentreReg_Read(),objectFirstReg_Read(),objectSecondReg_Read(),colourReg_Read());
 
        
        switch (motorState) {
            case STATE_STOPPED: // stopped
                changeMotorState(STATE_DEMO);
                break;
            case STATE_DEMO:
                changeMotorState(STATE_STOPPED);
                break;
            default:
                changeMotorState(STATE_STOPPED);
        }
        
	}
	
} /* Main_Task */
