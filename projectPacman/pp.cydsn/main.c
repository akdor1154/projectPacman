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

// This Function pulses the Clock line
void pump() {
    CyDelayUs(3);
    ClockLine_Write(1);
    CyDelayUs(5);
    ClockLine_Write(0);
    CyDelayUs(3);
}

void endSeq() {
    ClockLine_Write(1);  //End Sequence
    CyDelayUs(3);
    DataLine_Write(1);
    CyDelayUs(50);
}

void startSeq() {
    DataLine_Write(0); // Start Transmission
    CyDelayUs(3);
    ClockLine_Write(0);
    CyDelayUs(5);
}

void SccbWritePhase1(uint8 id, uint8 addr) {
    int i;
    
    // Phase 1 Write
    startSeq();
    
    for( i=sizeof(id)*CHAR_BIT-1; i>=0;--i) {
    // write the device id
        int bit = (id>>i) & 1;
        DataLine_Write(bit);
        pump();
    }
    
    DataLine_Write(0); // Don't Care bit
    pump();
    // Phase 2 Write
    for( i=sizeof(addr)*CHAR_BIT-1; i>=0; --i) {
        // write the register
        int bit = (addr>>i) & 1;
        DataLine_Write(bit);
        pump();
    }    
    
    DataLine_Write(0); // Don't Care bit
    pump();
}

void SccbWrite(uint8 id, uint8 addr, uint8 byte) {
    SccbWritePhase1(id, addr);
    int i;
    for( i=sizeof(byte)*CHAR_BIT-1; i>=0;--i) {
        // write the register
        int bit = (byte>>i) & 1;
        DataLine_Write(bit);
        pump();
    }    
    
    DataLine_Write(0); // Don't Care bit
    pump();
    endSeq();
}

uint8_t SccbRead(uint8 id, uint8 addr ) {
    int i;
    uint8_t sccbOut = 0;
    SccbWritePhase1(id, addr);
    endSeq();
    startSeq();
    
    //Phase 1 Read
    id++; // for read
    for( i=sizeof(id)*CHAR_BIT-1; i>=0;--i) // write the device id+1 for read
    {
        int bit = (id>>i) & 1;
        DataLine_Write(bit);
        pump();
    }
    
    //Phase 2 read
    // read data 
    pump(); //don't care bit
    
    for( i=7; i>=0;--i) {
       
        sccbOut = sccbOut | ((DataLine_Read()&0x1)<<i);
        CyDelayUs(15);
        pump();
    }
    
    DataLine_Write(1);
    pump();
    DataLine_Write(0);
    CyDelayUs(4);   
 
    endSeq();
    return sccbOut;
}

void CameraConfig()
{
    // No reset pins
    Cam_Powerdown_Write(0);
    Cam_Reset_Write(1);

   
    //SccbRead(0x42, 0x1d); // Manufacturer number
    SccbWrite(0x42,0x15,0x20); // No PCLK during HREF low
    SccbWrite(0x42,0x11,0x8f); // clock output
    SccbWrite(0x42,0x12,0x25); // Output is CIF, BAYER RAW is 21
    SccbWrite(0x42,0x13,0x87); // Auto Gain Control, Auto White Balance, Auto Exposure
    SccbWrite(0x42,0x2b,0x08); //8 dummy pixels 
    SccbWrite(0x42,0x1e,0x31); //enable vertical flip and mirror 
    
    /*
    SccbWrite(0x60,0x12,0x80); // initiates soft reset
    CyDelay(20);
    SccbWrite(0x60,0x12,0x00); // Binning 08 is 1/8 binning
    SccbWrite(0x60,0x11,0x7f); // Output clock divider
    SccbWrite(0x60,0x32,0xc0); //pixel clock divider 1/4
    SccbWrite(0x60,0x15,0x20); // PCLK output only when HREF high
    SccbWrite(0x60,0x80,0x11); //Color bar enable (Test pattern?)
    //SccbWrite(0x60,0x0c,0x0d); //  enable custom output
    //SccbWrite(0x60,0x61,0x1e); // 240
   // SccbWrite(0x60,0x60,0x19); // 240
    //SccbWrite(0x60,0x1b,0x05); //pixel shift
    //SccbWrite(0x60,0x09,0x04); //Slave Mode
    //SccbRead(0x60,0x12);
    */
  
}




void main( void )
{
    OS_ERR  err;
    global_err = 0;
    
    pixelCount = 0;
    
    
	/* Perform BSP pre-initialization (post-init occurs in MainTask) */
    BSP_PreInit();
    
    //CameraConfig();
    
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
