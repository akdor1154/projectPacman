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

/* Main_Task TCB, start function and stack */
OS_TCB	Main_Task_TCB;
CPU_STK	Main_Task_Stack[MAIN_TASK_STACK_SIZE];

extern uint8_t lastPixelY;
extern uint8_t lastPixelCb;
extern uint8_t lastPixelCr;
extern unsigned int pixelCount;
extern int8_t* YTest;

extern uint8_t YTest1[480];
extern uint8_t YTest2[480];

extern uint8_t yPix;
extern uint8_t cbPix;
extern uint8_t crPix;

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
    OS_RATE_HZ tenthSecond = OSCfg_TickRate_Hz/10;
    
    uint8_t pwmLevel;
    uint8_t pwmStep;
    uint8_t adcResult;
    
    uint8 yDMA_Chan;
    uint8 yDMA_TD[1];
    uint8 cbDMA_Chan;
    uint8 cbDMA_TD[1];
    uint8 crDMA_Chan;
    uint8 crDMA_TD[1];
    
    uint8_t magicToggle;
    
    
    
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

    CameraConfig();

    
    pwmLevel = 0;
    pwmStep = UINT8_MAX/16;
    
    PWM_1_Start();
    PWM_2_Start();
    
    SensorADC_Start();
    //SensorADC_StartConvert();
    Count7_1_Start();
    adcResult = 0;

    /* Variable declarations for yDMA */
    /* Move these variable declarations to the top of the function */


    /* DMA Configuration for yDMA */
    #define yDMA_BYTES_PER_BURST 1
    #define yDMA_REQUEST_PER_BURST 1
    #define yDMA_SRC_BASE (CYDEV_PERIPH_BASE)
    #define yDMA_DST_BASE (CYDEV_SRAM_BASE)
    yDMA_Chan = yDMA_DmaInitialize(yDMA_BYTES_PER_BURST, yDMA_REQUEST_PER_BURST, 
        HI16(yDMA_SRC_BASE), HI16(yDMA_DST_BASE));
    yDMA_TD[0] = CyDmaTdAllocate();
    CyDmaTdSetConfiguration(yDMA_TD[0], 1, yDMA_TD[0], 0);
    CyDmaTdSetAddress(yDMA_TD[0], LO16((uint32)pixelRegY_Status_PTR), LO16((uint32)&yPix));
    CyDmaChSetInitialTd(yDMA_Chan, yDMA_TD[0]);
    CyDmaChEnable(yDMA_Chan, 1);
    
    /* Variable declarations for cbDMA */
    /* Move these variable declarations to the top of the function */

    /* DMA Configuration for cbDMA */
    #define cbDMA_BYTES_PER_BURST 1
    #define cbDMA_REQUEST_PER_BURST 1
    #define cbDMA_SRC_BASE (CYDEV_PERIPH_BASE)
    #define cbDMA_DST_BASE (CYDEV_SRAM_BASE)
    cbDMA_Chan = cbDMA_DmaInitialize(cbDMA_BYTES_PER_BURST, cbDMA_REQUEST_PER_BURST, 
        HI16(cbDMA_SRC_BASE), HI16(cbDMA_DST_BASE));
    cbDMA_TD[0] = CyDmaTdAllocate();
    CyDmaTdSetConfiguration(cbDMA_TD[0], 1, cbDMA_TD[0], 0);
    CyDmaTdSetAddress(cbDMA_TD[0], LO16((uint32)pixelRegCb_Status_PTR), LO16((uint32)&cbPix));
    CyDmaChSetInitialTd(cbDMA_Chan, cbDMA_TD[0]);
    CyDmaChEnable(cbDMA_Chan, 1);

    
    /* Variable declarations for crDMA */
    /* Move these variable declarations to the top of the function */

    /* DMA Configuration for crDMA */
    #define crDMA_BYTES_PER_BURST 1
    #define crDMA_REQUEST_PER_BURST 1
    #define crDMA_SRC_BASE (CYDEV_PERIPH_BASE)
    #define crDMA_DST_BASE (CYDEV_SRAM_BASE)
    crDMA_Chan = crDMA_DmaInitialize(crDMA_BYTES_PER_BURST, crDMA_REQUEST_PER_BURST, 
        HI16(crDMA_SRC_BASE), HI16(crDMA_DST_BASE));
    crDMA_TD[0] = CyDmaTdAllocate();
    CyDmaTdSetConfiguration(crDMA_TD[0], 1, crDMA_TD[0], 0);
    CyDmaTdSetAddress(crDMA_TD[0], LO16((uint32)pixelRegCr_Status_PTR), LO16((uint32)&crPix));
    CyDmaChSetInitialTd(crDMA_Chan, crDMA_TD[0]);
    CyDmaChEnable(crDMA_Chan, 1);

    magicToggle=1;
    
	while (1) {
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
        //usbprint("error: %u\n",err);
        
        
		/* Toggle the LED3 */
        
        
        PWM_1_WriteCompare(pwmLevel);
        PWM_2_WriteCompare(UINT8_MAX-pwmLevel);
        pwmLevel += pwmStep;
        
       
        //usbprint("pixelcount is %u",pixelCount);
        
        //uint8_t pclk_count = pclk_test_reg_Read();
        //usbprint("pclk count is %u\n",pclk_count);
	    /*
        if (SensorADC_IsEndConversion(SensorADC_RETURN_STATUS)) {
            adcResult = (uint8_t)SensorADC_GetResult8();
        } else {
            adcResult = 3;
        }
        */
        /*
        usbprint("adc result: %u",adcResult);
        usbprint("ytest: %hhu",*YTest);
        */
        magicToggle = ~magicToggle;
        usbprint("%u,%u,%u\n",yPix,cbPix,crPix);
        
        
        
	}
	
} /* Main_Task */
