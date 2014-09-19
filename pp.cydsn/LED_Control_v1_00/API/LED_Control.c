/*******************************************************************************
* File Name: `@INSTANCE_NAME`.c
*
* Version 1.00
*
* Description:
* This file contains the LED_Control API definitions. The application can
* choose, via a control register (`@INSTANCE_NAME`_LED_Reg_Control),
* whether to drive the output from HW (schematic file) or SW (API call).
*
********************************************************************************
* Copyright 2008-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include <project.h>
#include <`@INSTANCE_NAME`.h>

/*
 * void `@INSTANCE_NAME`_Set_Channel( int use )
 *
 * Chooses which signal to route to the output/LED.
 *
 * Reads the register, modifies the bits that control the signal
 * selection, and writes the new value back to the register.
 *
 * Parameters:
 *	use:	The signal to be routed (`@INSTANCE_NAME`_USE_*)
 *
 * 	Return:	void
*/
void `@INSTANCE_NAME`_Set_Channel( int use )
{
	uint8 regval = `@INSTANCE_NAME`_LED_Reg_Control;
	
	/* Clear the bits that determine use and patch in the new value */
	regval &= ~`@INSTANCE_NAME`_USE_MASK;
	regval |= (uint8) use;
	
	/* Change the channel */
	`@INSTANCE_NAME`_LED_Reg_Control = regval;
	
} /* `@INSTANCE_NAME`_Set_Channel */


/*
 * int `@INSTANCE_NAME`_Get_Channel( void )
 *
 * Returns the value of the software bit in the control register.
 * If the output is driven from the schematic the return value is undefined.
 *
 * Parameters:	None
 *
 * 	Return:		int
*/
int `@INSTANCE_NAME`_Get_Channel( void )
{
	return ( `@INSTANCE_NAME`_LED_Reg_Control & `@INSTANCE_NAME`_USE_MASK );
} /* `@INSTANCE_NAME`_Get_Channel */


/*
 * void `@INSTANCE_NAME`_Write( int value )
 *
 * Writes to the LED when it is driven from software.
 *
 * Reads the register and checks that the selected output is software-driven.
 * If so, it updates the bit that drives the output based on the function argument.
 *
 * If the output is driven from the schematic this API has no effect.
 *
 * Parameters:
 *	value:	0 (OFF) or non-zero (ON)
 *
 * 	Return:	void
*/
void `@INSTANCE_NAME`_Write( int value )
{
	uint8 regval = `@INSTANCE_NAME`_LED_Reg_Control;
	
	if( ( regval & `@INSTANCE_NAME`_USE_MASK ) == `@INSTANCE_NAME`_USE_SOFTWARE )
	{
		/* Patch bit `@INSTANCE_NAME`_SOFTWARE_MASK depending upon value */
		if( value )
		{
			/* Set output to 1 */
			regval |= `@INSTANCE_NAME`_SOFTWARE_MASK;
		}
		else
		{
			/* Clear the output */
			regval &= ~`@INSTANCE_NAME`_SOFTWARE_MASK;
		}
	
		`@INSTANCE_NAME`_LED_Reg_Control = regval;
	} /* if( reg & `@INSTANCE_NAME`_USE_MASK == `@INSTANCE_NAME`_USE_SOFTWARE ) */
} /* `@INSTANCE_NAME`_Write */


/*
 * int `@INSTANCE_NAME`_Read( void )
 *
 * Returns the value of the software bit in the control register.
 * If the output is driven from the schematic the return value is undefined.
 *
 * Parameters:	None
 *
 * 	Return:		0 (OFF) or 1 (ON)
*/
int `@INSTANCE_NAME`_Read( void )
{
	return ( `@INSTANCE_NAME`_LED_Reg_Control & `@INSTANCE_NAME`_SOFTWARE_MASK ) ? 1 : 0;
} /* `@INSTANCE_NAME`_Read */
