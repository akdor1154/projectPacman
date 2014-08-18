/*******************************************************************************
* File Name: `@INSTANCE_NAME`.h
*
* Version 1.00
*
* Description:
* This file contains the LED_Control API declarations and the Control
* Register define.
*
********************************************************************************
* Copyright 2008-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(`@INSTANCE_NAME`_H)
#define `@INSTANCE_NAME`_H

/* LED_Control_Channel type definitions */
`#cy_declare_enum LED_Control_Channel`

/* Definitions of control register meanings */
#define `@INSTANCE_NAME`_SOFTWARE_BIT		(3)
#define `@INSTANCE_NAME`_SOFTWARE_MASK		(1<<`@INSTANCE_NAME`_SOFTWARE_BIT)

/* Definitions of MUX channels */
#define `@INSTANCE_NAME`_USE_SOFTWARE		(`@INSTANCE_NAME`__SOFTWARE)
#define `@INSTANCE_NAME`_USE_IN1			(`@INSTANCE_NAME`__IN1)
#define `@INSTANCE_NAME`_USE_IN2			(`@INSTANCE_NAME`__IN2)
#define `@INSTANCE_NAME`_USE_IN1_AND_IN2	(`@INSTANCE_NAME`__IN1_AND_IN2)
#define `@INSTANCE_NAME`_USE_IN1_XOR_IN2	(`@INSTANCE_NAME`__IN1_XOR_IN2)
#define `@INSTANCE_NAME`_USE_MASK			(7)


/***************************************
*         Function Prototypes 
***************************************/

/* Chooses the signal to be routed to the output/LED */
void `@INSTANCE_NAME`_Set_Channel( int use );

/* Returns the selected output/LED */
int `@INSTANCE_NAME`_Get_Channel( void );

/* Writes to the output/LED when it is driven from software */
void `@INSTANCE_NAME`_Write( int value );

/* Returns the state of the output when it is driven from software */
int	`@INSTANCE_NAME`_Read(void);


/***************************************
*            Registers        
***************************************/

/* Control Register */
#define `@INSTANCE_NAME`_LED_Reg_Control \
		(* (reg8 *)`@INSTANCE_NAME`_LED_Reg_Sync_ctrl_reg__CONTROL_REG)

#endif
