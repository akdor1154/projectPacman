/*******************************************************************************
* File Name: colourChange.c  
* Version 1.70
*
*  Description:
*   API for controlling the state of an interrupt.
*
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/


#include <cydevice_trm.h>
#include <CyLib.h>
#include <colourChange.h>

#if !defined(colourChange__REMOVED) /* Check for removal by optimization */

/*******************************************************************************
*  Place your includes, defines and code here 
********************************************************************************/
/* `#START colourChange_intc` */
#include "colourStatus.h"
#include "jarrad_util.h"
extern colour lastSeenColour;
/* `#END` */

#ifndef CYINT_IRQ_BASE
#define CYINT_IRQ_BASE      16
#endif /* CYINT_IRQ_BASE */
#ifndef CYINT_VECT_TABLE
#define CYINT_VECT_TABLE    ((cyisraddress **) CYREG_NVIC_VECT_OFFSET)
#endif /* CYINT_VECT_TABLE */

/* Declared in startup, used to set unused interrupts to. */
CY_ISR_PROTO(IntDefaultHandler);


/*******************************************************************************
* Function Name: colourChange_Start
********************************************************************************
*
* Summary:
*  Set up the interrupt and enable it.
*
* Parameters:  
*   None
*
* Return:
*   None
*
*******************************************************************************/
void colourChange_Start(void)
{
    /* For all we know the interrupt is active. */
    colourChange_Disable();

    /* Set the ISR to point to the colourChange Interrupt. */
    colourChange_SetVector(&colourChange_Interrupt);

    /* Set the priority. */
    colourChange_SetPriority((uint8)colourChange_INTC_PRIOR_NUMBER);

    /* Enable it. */
    colourChange_Enable();
}


/*******************************************************************************
* Function Name: colourChange_StartEx
********************************************************************************
*
* Summary:
*  Set up the interrupt and enable it.
*
* Parameters:  
*   address: Address of the ISR to set in the interrupt vector table.
*
* Return:
*   None
*
*******************************************************************************/
void colourChange_StartEx(cyisraddress address)
{
    /* For all we know the interrupt is active. */
    colourChange_Disable();

    /* Set the ISR to point to the colourChange Interrupt. */
    colourChange_SetVector(address);

    /* Set the priority. */
    colourChange_SetPriority((uint8)colourChange_INTC_PRIOR_NUMBER);

    /* Enable it. */
    colourChange_Enable();
}


/*******************************************************************************
* Function Name: colourChange_Stop
********************************************************************************
*
* Summary:
*   Disables and removes the interrupt.
*
* Parameters:  
*
* Return:
*   None
*
*******************************************************************************/
void colourChange_Stop(void)
{
    /* Disable this interrupt. */
    colourChange_Disable();

    /* Set the ISR to point to the passive one. */
    colourChange_SetVector(&IntDefaultHandler);
}


/*******************************************************************************
* Function Name: colourChange_Interrupt
********************************************************************************
*
* Summary:
*   The default Interrupt Service Routine for colourChange.
*
*   Add custom code between the coments to keep the next version of this file
*   from over writting your code.
*
* Parameters:  
*
* Return:
*   None
*
*******************************************************************************/
CY_ISR(colourChange_Interrupt)
{
    /*  Place your Interrupt code here. */
    /* `#START colourChange_Interrupt` */

    uint8_t c = colourStatus_Status;
    if (c & red) lastSeenColour = red;
    else if (c & blue) lastSeenColour = blue;
    /* `#END` */
}


/*******************************************************************************
* Function Name: colourChange_SetVector
********************************************************************************
*
* Summary:
*   Change the ISR vector for the Interrupt. Note calling colourChange_Start
*   will override any effect this method would have had. To set the vector 
*   before the component has been started use colourChange_StartEx instead.
*
* Parameters:
*   address: Address of the ISR to set in the interrupt vector table.
*
* Return:
*   None
*
*******************************************************************************/
void colourChange_SetVector(cyisraddress address)
{
    cyisraddress * ramVectorTable;

    ramVectorTable = (cyisraddress *) *CYINT_VECT_TABLE;

    ramVectorTable[CYINT_IRQ_BASE + (uint32)colourChange__INTC_NUMBER] = address;
}


/*******************************************************************************
* Function Name: colourChange_GetVector
********************************************************************************
*
* Summary:
*   Gets the "address" of the current ISR vector for the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   Address of the ISR in the interrupt vector table.
*
*******************************************************************************/
cyisraddress colourChange_GetVector(void)
{
    cyisraddress * ramVectorTable;

    ramVectorTable = (cyisraddress *) *CYINT_VECT_TABLE;

    return ramVectorTable[CYINT_IRQ_BASE + (uint32)colourChange__INTC_NUMBER];
}


/*******************************************************************************
* Function Name: colourChange_SetPriority
********************************************************************************
*
* Summary:
*   Sets the Priority of the Interrupt. Note calling colourChange_Start
*   or colourChange_StartEx will override any effect this method 
*   would have had. This method should only be called after 
*   colourChange_Start or colourChange_StartEx has been called. To set 
*   the initial priority for the component use the cydwr file in the tool.
*
* Parameters:
*   priority: Priority of the interrupt. 0 - 7, 0 being the highest.
*
* Return:
*   None
*
*******************************************************************************/
void colourChange_SetPriority(uint8 priority)
{
    *colourChange_INTC_PRIOR = priority << 5;
}


/*******************************************************************************
* Function Name: colourChange_GetPriority
********************************************************************************
*
* Summary:
*   Gets the Priority of the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   Priority of the interrupt. 0 - 7, 0 being the highest.
*
*******************************************************************************/
uint8 colourChange_GetPriority(void)
{
    uint8 priority;


    priority = *colourChange_INTC_PRIOR >> 5;

    return priority;
}


/*******************************************************************************
* Function Name: colourChange_Enable
********************************************************************************
*
* Summary:
*   Enables the interrupt.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void colourChange_Enable(void)
{
    /* Enable the general interrupt. */
    *colourChange_INTC_SET_EN = colourChange__INTC_MASK;
}


/*******************************************************************************
* Function Name: colourChange_GetState
********************************************************************************
*
* Summary:
*   Gets the state (enabled, disabled) of the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   1 if enabled, 0 if disabled.
*
*******************************************************************************/
uint8 colourChange_GetState(void)
{
    /* Get the state of the general interrupt. */
    return ((*colourChange_INTC_SET_EN & (uint32)colourChange__INTC_MASK) != 0u) ? 1u:0u;
}


/*******************************************************************************
* Function Name: colourChange_Disable
********************************************************************************
*
* Summary:
*   Disables the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void colourChange_Disable(void)
{
    /* Disable the general interrupt. */
    *colourChange_INTC_CLR_EN = colourChange__INTC_MASK;
}


/*******************************************************************************
* Function Name: colourChange_SetPending
********************************************************************************
*
* Summary:
*   Causes the Interrupt to enter the pending state, a software method of
*   generating the interrupt.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void colourChange_SetPending(void)
{
    *colourChange_INTC_SET_PD = colourChange__INTC_MASK;
}


/*******************************************************************************
* Function Name: colourChange_ClearPending
********************************************************************************
*
* Summary:
*   Clears a pending interrupt.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void colourChange_ClearPending(void)
{
    *colourChange_INTC_CLR_PD = colourChange__INTC_MASK;
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
