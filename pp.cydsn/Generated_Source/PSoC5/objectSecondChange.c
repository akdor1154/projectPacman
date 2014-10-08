/*******************************************************************************
* File Name: objectSecondChange.c  
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
#include <objectSecondChange.h>

#if !defined(objectSecondChange__REMOVED) /* Check for removal by optimization */

/*******************************************************************************
*  Place your includes, defines and code here 
********************************************************************************/
/* `#START objectSecondChange_intc` */
#include <includes.h>
#include "Task_Defs.h"
OS_ERR err;
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
* Function Name: objectSecondChange_Start
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
void objectSecondChange_Start(void)
{
    /* For all we know the interrupt is active. */
    objectSecondChange_Disable();

    /* Set the ISR to point to the objectSecondChange Interrupt. */
    objectSecondChange_SetVector(&objectSecondChange_Interrupt);

    /* Set the priority. */
    objectSecondChange_SetPriority((uint8)objectSecondChange_INTC_PRIOR_NUMBER);

    /* Enable it. */
    objectSecondChange_Enable();
}


/*******************************************************************************
* Function Name: objectSecondChange_StartEx
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
void objectSecondChange_StartEx(cyisraddress address)
{
    /* For all we know the interrupt is active. */
    objectSecondChange_Disable();

    /* Set the ISR to point to the objectSecondChange Interrupt. */
    objectSecondChange_SetVector(address);

    /* Set the priority. */
    objectSecondChange_SetPriority((uint8)objectSecondChange_INTC_PRIOR_NUMBER);

    /* Enable it. */
    objectSecondChange_Enable();
}


/*******************************************************************************
* Function Name: objectSecondChange_Stop
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
void objectSecondChange_Stop(void)
{
    /* Disable this interrupt. */
    objectSecondChange_Disable();

    /* Set the ISR to point to the passive one. */
    objectSecondChange_SetVector(&IntDefaultHandler);
}


/*******************************************************************************
* Function Name: objectSecondChange_Interrupt
********************************************************************************
*
* Summary:
*   The default Interrupt Service Routine for objectSecondChange.
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
CY_ISR(objectSecondChange_Interrupt)
{
    /*  Place your Interrupt code here. */
    /* `#START objectSecondChange_Interrupt` */
    CPU_SR_ALLOC();
    
    CPU_CRITICAL_ENTER();
    OSIntEnter();
    CPU_CRITICAL_EXIT();
    //stuff
    OSTaskSemPost(
        &Flipper_Task_TCB,
        OS_OPT_POST_NONE,
        &err
    );
    OSIntExit();
    /* `#END` */
}


/*******************************************************************************
* Function Name: objectSecondChange_SetVector
********************************************************************************
*
* Summary:
*   Change the ISR vector for the Interrupt. Note calling objectSecondChange_Start
*   will override any effect this method would have had. To set the vector 
*   before the component has been started use objectSecondChange_StartEx instead.
*
* Parameters:
*   address: Address of the ISR to set in the interrupt vector table.
*
* Return:
*   None
*
*******************************************************************************/
void objectSecondChange_SetVector(cyisraddress address)
{
    cyisraddress * ramVectorTable;

    ramVectorTable = (cyisraddress *) *CYINT_VECT_TABLE;

    ramVectorTable[CYINT_IRQ_BASE + (uint32)objectSecondChange__INTC_NUMBER] = address;
}


/*******************************************************************************
* Function Name: objectSecondChange_GetVector
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
cyisraddress objectSecondChange_GetVector(void)
{
    cyisraddress * ramVectorTable;

    ramVectorTable = (cyisraddress *) *CYINT_VECT_TABLE;

    return ramVectorTable[CYINT_IRQ_BASE + (uint32)objectSecondChange__INTC_NUMBER];
}


/*******************************************************************************
* Function Name: objectSecondChange_SetPriority
********************************************************************************
*
* Summary:
*   Sets the Priority of the Interrupt. Note calling objectSecondChange_Start
*   or objectSecondChange_StartEx will override any effect this method 
*   would have had. This method should only be called after 
*   objectSecondChange_Start or objectSecondChange_StartEx has been called. To set 
*   the initial priority for the component use the cydwr file in the tool.
*
* Parameters:
*   priority: Priority of the interrupt. 0 - 7, 0 being the highest.
*
* Return:
*   None
*
*******************************************************************************/
void objectSecondChange_SetPriority(uint8 priority)
{
    *objectSecondChange_INTC_PRIOR = priority << 5;
}


/*******************************************************************************
* Function Name: objectSecondChange_GetPriority
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
uint8 objectSecondChange_GetPriority(void)
{
    uint8 priority;


    priority = *objectSecondChange_INTC_PRIOR >> 5;

    return priority;
}


/*******************************************************************************
* Function Name: objectSecondChange_Enable
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
void objectSecondChange_Enable(void)
{
    /* Enable the general interrupt. */
    *objectSecondChange_INTC_SET_EN = objectSecondChange__INTC_MASK;
}


/*******************************************************************************
* Function Name: objectSecondChange_GetState
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
uint8 objectSecondChange_GetState(void)
{
    /* Get the state of the general interrupt. */
    return ((*objectSecondChange_INTC_SET_EN & (uint32)objectSecondChange__INTC_MASK) != 0u) ? 1u:0u;
}


/*******************************************************************************
* Function Name: objectSecondChange_Disable
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
void objectSecondChange_Disable(void)
{
    /* Disable the general interrupt. */
    *objectSecondChange_INTC_CLR_EN = objectSecondChange__INTC_MASK;
}


/*******************************************************************************
* Function Name: objectSecondChange_SetPending
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
void objectSecondChange_SetPending(void)
{
    *objectSecondChange_INTC_SET_PD = objectSecondChange__INTC_MASK;
}


/*******************************************************************************
* Function Name: objectSecondChange_ClearPending
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
void objectSecondChange_ClearPending(void)
{
    *objectSecondChange_INTC_CLR_PD = objectSecondChange__INTC_MASK;
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
