/*******************************************************************************
* File Name: analogReady.c  
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
#include <analogReady.h>

#if !defined(analogReady__REMOVED) /* Check for removal by optimization */

/*******************************************************************************
*  Place your includes, defines and code here 
********************************************************************************/
/* `#START analogReady_intc` */
#include <includes.h>
#include <Task_Defs.h>
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
* Function Name: analogReady_Start
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
void analogReady_Start(void)
{
    /* For all we know the interrupt is active. */
    analogReady_Disable();

    /* Set the ISR to point to the analogReady Interrupt. */
    analogReady_SetVector(&analogReady_Interrupt);

    /* Set the priority. */
    analogReady_SetPriority((uint8)analogReady_INTC_PRIOR_NUMBER);

    /* Enable it. */
    analogReady_Enable();
}


/*******************************************************************************
* Function Name: analogReady_StartEx
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
void analogReady_StartEx(cyisraddress address)
{
    /* For all we know the interrupt is active. */
    analogReady_Disable();

    /* Set the ISR to point to the analogReady Interrupt. */
    analogReady_SetVector(address);

    /* Set the priority. */
    analogReady_SetPriority((uint8)analogReady_INTC_PRIOR_NUMBER);

    /* Enable it. */
    analogReady_Enable();
}


/*******************************************************************************
* Function Name: analogReady_Stop
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
void analogReady_Stop(void)
{
    /* Disable this interrupt. */
    analogReady_Disable();

    /* Set the ISR to point to the passive one. */
    analogReady_SetVector(&IntDefaultHandler);
}


/*******************************************************************************
* Function Name: analogReady_Interrupt
********************************************************************************
*
* Summary:
*   The default Interrupt Service Routine for analogReady.
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
CY_ISR(analogReady_Interrupt)
{
    /*  Place your Interrupt code here. */
    /* `#START analogReady_Interrupt` */
    CPU_SR_ALLOC();
    
    CPU_CRITICAL_ENTER();
    OSIntEnter();
    CPU_CRITICAL_EXIT();
    
    //stuff
    OSTaskSemPost(
        &Analog_Select_TCB,
        OS_OPT_POST_NONE,
        &err
    );
    OSIntExit();
    /* `#END` */
}


/*******************************************************************************
* Function Name: analogReady_SetVector
********************************************************************************
*
* Summary:
*   Change the ISR vector for the Interrupt. Note calling analogReady_Start
*   will override any effect this method would have had. To set the vector 
*   before the component has been started use analogReady_StartEx instead.
*
* Parameters:
*   address: Address of the ISR to set in the interrupt vector table.
*
* Return:
*   None
*
*******************************************************************************/
void analogReady_SetVector(cyisraddress address)
{
    cyisraddress * ramVectorTable;

    ramVectorTable = (cyisraddress *) *CYINT_VECT_TABLE;

    ramVectorTable[CYINT_IRQ_BASE + (uint32)analogReady__INTC_NUMBER] = address;
}


/*******************************************************************************
* Function Name: analogReady_GetVector
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
cyisraddress analogReady_GetVector(void)
{
    cyisraddress * ramVectorTable;

    ramVectorTable = (cyisraddress *) *CYINT_VECT_TABLE;

    return ramVectorTable[CYINT_IRQ_BASE + (uint32)analogReady__INTC_NUMBER];
}


/*******************************************************************************
* Function Name: analogReady_SetPriority
********************************************************************************
*
* Summary:
*   Sets the Priority of the Interrupt. Note calling analogReady_Start
*   or analogReady_StartEx will override any effect this method 
*   would have had. This method should only be called after 
*   analogReady_Start or analogReady_StartEx has been called. To set 
*   the initial priority for the component use the cydwr file in the tool.
*
* Parameters:
*   priority: Priority of the interrupt. 0 - 7, 0 being the highest.
*
* Return:
*   None
*
*******************************************************************************/
void analogReady_SetPriority(uint8 priority)
{
    *analogReady_INTC_PRIOR = priority << 5;
}


/*******************************************************************************
* Function Name: analogReady_GetPriority
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
uint8 analogReady_GetPriority(void)
{
    uint8 priority;


    priority = *analogReady_INTC_PRIOR >> 5;

    return priority;
}


/*******************************************************************************
* Function Name: analogReady_Enable
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
void analogReady_Enable(void)
{
    /* Enable the general interrupt. */
    *analogReady_INTC_SET_EN = analogReady__INTC_MASK;
}


/*******************************************************************************
* Function Name: analogReady_GetState
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
uint8 analogReady_GetState(void)
{
    /* Get the state of the general interrupt. */
    return ((*analogReady_INTC_SET_EN & (uint32)analogReady__INTC_MASK) != 0u) ? 1u:0u;
}


/*******************************************************************************
* Function Name: analogReady_Disable
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
void analogReady_Disable(void)
{
    /* Disable the general interrupt. */
    *analogReady_INTC_CLR_EN = analogReady__INTC_MASK;
}


/*******************************************************************************
* Function Name: analogReady_SetPending
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
void analogReady_SetPending(void)
{
    *analogReady_INTC_SET_PD = analogReady__INTC_MASK;
}


/*******************************************************************************
* Function Name: analogReady_ClearPending
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
void analogReady_ClearPending(void)
{
    *analogReady_INTC_CLR_PD = analogReady__INTC_MASK;
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
