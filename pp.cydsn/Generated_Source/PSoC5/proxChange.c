/*******************************************************************************
* File Name: proxChange.c  
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
#include <proxChange.h>

#if !defined(proxChange__REMOVED) /* Check for removal by optimization */

/*******************************************************************************
*  Place your includes, defines and code here 
********************************************************************************/
/* `#START proxChange_intc` */
#include <device.h>
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
* Function Name: proxChange_Start
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
void proxChange_Start(void)
{
    /* For all we know the interrupt is active. */
    proxChange_Disable();

    /* Set the ISR to point to the proxChange Interrupt. */
    proxChange_SetVector(&proxChange_Interrupt);

    /* Set the priority. */
    proxChange_SetPriority((uint8)proxChange_INTC_PRIOR_NUMBER);

    /* Enable it. */
    proxChange_Enable();
}


/*******************************************************************************
* Function Name: proxChange_StartEx
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
void proxChange_StartEx(cyisraddress address)
{
    /* For all we know the interrupt is active. */
    proxChange_Disable();

    /* Set the ISR to point to the proxChange Interrupt. */
    proxChange_SetVector(address);

    /* Set the priority. */
    proxChange_SetPriority((uint8)proxChange_INTC_PRIOR_NUMBER);

    /* Enable it. */
    proxChange_Enable();
}


/*******************************************************************************
* Function Name: proxChange_Stop
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
void proxChange_Stop(void)
{
    /* Disable this interrupt. */
    proxChange_Disable();

    /* Set the ISR to point to the passive one. */
    proxChange_SetVector(&IntDefaultHandler);
}


/*******************************************************************************
* Function Name: proxChange_Interrupt
********************************************************************************
*
* Summary:
*   The default Interrupt Service Routine for proxChange.
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
CY_ISR(proxChange_Interrupt)
{
    /*  Place your Interrupt code here. */
    /* `#START proxChange_Interrupt` */
    CPU_SR_ALLOC();
    
    CPU_CRITICAL_ENTER();
    OSIntEnter();
    CPU_CRITICAL_EXIT();
    proxChange_Disable();
    proxChange_ClearPending();
    //stuff
    OSTaskSemPost(
        &Dodgem_Task_TCB,
        OS_OPT_POST_NONE,
        &err
    );
    OSTimeDlyResume(&Dodgem_Task_TCB, &err);
    proxChange_Enable();
    OSIntExit();
    /* `#END` */
}


/*******************************************************************************
* Function Name: proxChange_SetVector
********************************************************************************
*
* Summary:
*   Change the ISR vector for the Interrupt. Note calling proxChange_Start
*   will override any effect this method would have had. To set the vector 
*   before the component has been started use proxChange_StartEx instead.
*
* Parameters:
*   address: Address of the ISR to set in the interrupt vector table.
*
* Return:
*   None
*
*******************************************************************************/
void proxChange_SetVector(cyisraddress address)
{
    cyisraddress * ramVectorTable;

    ramVectorTable = (cyisraddress *) *CYINT_VECT_TABLE;

    ramVectorTable[CYINT_IRQ_BASE + (uint32)proxChange__INTC_NUMBER] = address;
}


/*******************************************************************************
* Function Name: proxChange_GetVector
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
cyisraddress proxChange_GetVector(void)
{
    cyisraddress * ramVectorTable;

    ramVectorTable = (cyisraddress *) *CYINT_VECT_TABLE;

    return ramVectorTable[CYINT_IRQ_BASE + (uint32)proxChange__INTC_NUMBER];
}


/*******************************************************************************
* Function Name: proxChange_SetPriority
********************************************************************************
*
* Summary:
*   Sets the Priority of the Interrupt. Note calling proxChange_Start
*   or proxChange_StartEx will override any effect this method 
*   would have had. This method should only be called after 
*   proxChange_Start or proxChange_StartEx has been called. To set 
*   the initial priority for the component use the cydwr file in the tool.
*
* Parameters:
*   priority: Priority of the interrupt. 0 - 7, 0 being the highest.
*
* Return:
*   None
*
*******************************************************************************/
void proxChange_SetPriority(uint8 priority)
{
    *proxChange_INTC_PRIOR = priority << 5;
}


/*******************************************************************************
* Function Name: proxChange_GetPriority
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
uint8 proxChange_GetPriority(void)
{
    uint8 priority;


    priority = *proxChange_INTC_PRIOR >> 5;

    return priority;
}


/*******************************************************************************
* Function Name: proxChange_Enable
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
void proxChange_Enable(void)
{
    /* Enable the general interrupt. */
    *proxChange_INTC_SET_EN = proxChange__INTC_MASK;
}


/*******************************************************************************
* Function Name: proxChange_GetState
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
uint8 proxChange_GetState(void)
{
    /* Get the state of the general interrupt. */
    return ((*proxChange_INTC_SET_EN & (uint32)proxChange__INTC_MASK) != 0u) ? 1u:0u;
}


/*******************************************************************************
* Function Name: proxChange_Disable
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
void proxChange_Disable(void)
{
    /* Disable the general interrupt. */
    *proxChange_INTC_CLR_EN = proxChange__INTC_MASK;
}


/*******************************************************************************
* Function Name: proxChange_SetPending
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
void proxChange_SetPending(void)
{
    *proxChange_INTC_SET_PD = proxChange__INTC_MASK;
}


/*******************************************************************************
* Function Name: proxChange_ClearPending
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
void proxChange_ClearPending(void)
{
    *proxChange_INTC_CLR_PD = proxChange__INTC_MASK;
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
