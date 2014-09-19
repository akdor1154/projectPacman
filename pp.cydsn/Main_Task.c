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
void Main_Task( void *p_arg )
{
	OS_ERR err;			/* Hold OS call return code */
	CPU_TS ts;
	(void)p_arg;		/* no-op prevents warning about unused p_arg */
    
    //uint8_t adcResult;
    
    uint8 rDMA_Chan;
    uint8 rDMA_TD[1];
    uint8 gDMA_Chan;
    uint8 gDMA_TD[1];
    uint8 bDMA_Chan;
    uint8 bDMA_TD[1];
    
    //uint8_t magicToggle;
    
    
    
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
    usbprint("blah\n\n");
    CameraConfig();
    
    PWM_1_Start();
    PWM_2_Start();
    
    LeftMotorPWM_Start();
    RightMotorPWM_Start();
    
    changeMotorState(STATE_STOPPED);
    
    //SensorADC_Start();
    //SensorADC_StartConvert();
    //Count7_1_Start();
    //adcResult = 0;

    /* Variable declarations for yDMA */
    /* Move these variable declarations to the top of the function */

    #define DMA_SRC_BASE (CYDEV_PERIPH_BASE)
    #define DMA_DST_BASE (CYDEV_SRAM_BASE)
    #define DMA_BYTES_PER_BURST 1
    #define DMA_REQUEST_PER_BURST 1

    /* DMA Configuration for yDMA */
    rDMA_Chan = rDMA_DmaInitialize(DMA_BYTES_PER_BURST, DMA_REQUEST_PER_BURST, 
        HI16(DMA_SRC_BASE), HI16(DMA_DST_BASE));
    rDMA_TD[0] = CyDmaTdAllocate();
    CyDmaTdSetConfiguration(rDMA_TD[0], 1, rDMA_TD[0], 0);
    CyDmaTdSetAddress(rDMA_TD[0], LO16((uint32)pixelRegR_Status_PTR), LO16((uint32)&rPix));
    CyDmaChSetInitialTd(rDMA_Chan, rDMA_TD[0]);
    CyDmaChEnable(rDMA_Chan, 1);
    
    /* Variable declarations for cbDMA */
    /* Move these variable declarations to the top of the function */

    /* DMA Configuration for cbDMA */
    gDMA_Chan = gDMA_DmaInitialize(DMA_BYTES_PER_BURST, DMA_REQUEST_PER_BURST, 
        HI16(DMA_SRC_BASE), HI16(DMA_DST_BASE));
    gDMA_TD[0] = CyDmaTdAllocate();
    CyDmaTdSetConfiguration(gDMA_TD[0], 1, gDMA_TD[0], 0);
    CyDmaTdSetAddress(gDMA_TD[0], LO16((uint32)pixelRegG_Status_PTR), LO16((uint32)&gPix));
    CyDmaChSetInitialTd(gDMA_Chan, gDMA_TD[0]);
    CyDmaChEnable(gDMA_Chan, 1);

    
    /* Variable declarations for crDMA */
    /* Move these variable declarations to the top of the function */

    /* DMA Configuration for crDMA */
    bDMA_Chan = bDMA_DmaInitialize(DMA_BYTES_PER_BURST, DMA_REQUEST_PER_BURST, 
        HI16(DMA_SRC_BASE), HI16(DMA_DST_BASE));
    bDMA_TD[0] = CyDmaTdAllocate();
    CyDmaTdSetConfiguration(bDMA_TD[0], 1, bDMA_TD[0], 0);
    CyDmaTdSetAddress(bDMA_TD[0], LO16((uint32)pixelRegB_Status_PTR), LO16((uint32)&bPix));
    CyDmaChSetInitialTd(bDMA_Chan, bDMA_TD[0]);
    CyDmaChEnable(bDMA_Chan, 1);

    
    uint8_t numSensors = 4;
    SensorDMA sensorDMAs[] = {
        {.dmaInit = &colourDMA_DmaInitialize, .dest = colourReg_Control_PTR},
        {.dmaInit = &objectDMA_DmaInitialize, .dest = objectReg_Control_PTR},
        {.dmaInit = &proxLeftDMA_DmaInitialize, .dest = proxLeftReg_Control_PTR},
        {.dmaInit = &proxRightDMA_DmaInitialize, .dest = proxRightReg_Control_PTR}
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
    
    SensorADC_Start();
    
    motorState = 0;
    
	while (DEF_ON) {
		/* Delay 1s */
		/*OSTimeDly(
			tenthSecond,
           	OS_OPT_TIME_DLY,
			&err );*/
		
        usbprint("waiting...\n");
        OSTaskSemPend(
            0,
            OS_OPT_PEND_BLOCKING,
            &ts,
            &err
        );
        
        usbprint("proxLeft: %u\n proxRight: %u\n objectUnder:%u\n colourUnder:%u\n\n ",proxLeftTestReg_Read(),proxRightTestReg_Read(),objectTestReg_Read(),colourTestReg_Read());
 
        
	    /*
        if (SensorADC_IsEndConversion(SensorADC_RETURN_STATUS)) {
            adcResult = (uint8_t)SensorADC_GetResult8();
        } else {
            adcResult = 3;
        }
        usbprint("adc result: %u",adcResult);
        */
        //usbprint("%u,%u,%u\n",rPix<<3,gPix<<2,bPix<<3);        
        
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
