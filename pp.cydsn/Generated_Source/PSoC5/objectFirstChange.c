/*******************************************************************************
* File Name: objectFirstChange.c  
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
#include <objectFirstChange.h>

#if !defined(objectFirstChange__REMOVED) /* Check for removal by optimization */

/*******************************************************************************
*  Place your includes, defines and code here 
********************************************************************************/
/* `#START objectFirstChange_intc` */
#include <includes.h>
extern OS_TICK objectFirstTicks;
extern int objectFirstCheck;
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
* Function Name: objectFirstChange_Start
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
void objectFirstChange_Start(void)
{
    /* For all we know the interrupt is active. */
    objectFirstChange_Disable();

    /* Set the ISR to point to the objectFirstChange Interrupt. */
    objectFirstChange_SetVector(&objectFirstChange_Interrupt);

    /* Set the priority. */
    objectFirstChange_SetPriority((uint8)objectFirstChange_INTC_PRIOR_NUMBER);

    /* Enable it. */
    objectFirstChange_Enable();
}


/*******************************************************************************
* Function Name: objectFirstChange_StartEx
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
void objectFirstChange_StartEx(cyisraddress address)
{
    /* For all we know the interrupt is active. */
    objectFirstChange_Disable();

    /* Set the ISR to point to the objectFirstChange Interrupt. */
    objectFirstChange_SetVector(address);

    /* Set the priority. */
    objectFirstChange_SetPriority((uint8)objectFirstChange_INTC_PRIOR_NUMBER);

    /* Enable it. */
    objectFirstChange_Enable();
}


/*******************************************************************************
* Function Name: objectFirstChange_Stop
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
void objectFirstChange_Stop(void)
{
    /* Disable this interrupt. */
    objectFirstChange_Disable();

    /* Set the ISR to point to the passive one. */
    objectFirstChange_SetVector(&IntDefaultHandler);
}


/*******************************************************************************
* Function Name: objectFirstChange_Interrupt
********************************************************************************
*
* Summary:
*   The default Interrupt Service Routine for objectFirstChange.
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
CY_ISR(objectFirstChange_Interrupt)
{
    /*  Place your Interrupt code here. */
    /* `#START objectFirstChange_Interrupt` */
    CPU_SR_ALLOC();
        
    CPU_CRITICAL_ENTER();
    OSIntEnter();
    CPU_CRITICAL_EXIT();
        
        //stuff
    objectFirstTicks = OSTimeGet(&err);
    objectFirstCheck = 1;
    OSIntExit();
    /* `#END` */
}


/*******************************************************************************
* Function Name: objectFirstChange_SetVector
********************************************************************************
*
* Summary:
*   Change the ISR vector for the Interrupt. Note calling objectFirstChange_Start
*   will override any effect this method would have had. To set the vector 
*   before the component has been started use objectFirstChange_StartEx instead.
*
* Parameters:
*   address: Address of the ISR to set in the interrupt vector table.
*
* Return:
*   None
*
*******************************************************************************/
void objectFirstChange_SetVector(cyisraddress address)
{
    cyisraddress * ramVectorTable;

    ramVectorTable = (cyisraddress *) *CYINT_VECT_TABLE;

    ramVectorTable[CYINT_IRQ_BASE + (uint32)objectFirstChange__INTC_NUMBER] = address;
}


/*******************************************************************************
* Function Name: objectFirstChange_GetVector
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
cyisraddress objectFirstChange_GetVector(void)
{
    cyisraddress * ramVectorTable;

    ramVectorTable = (cyisraddress *) *CYINT_VECT_TABLE;

    return ramVectorTable[CYINT_IRQ_BASE + (uint32)objectFirstChange__INTC_NUMBER];
}


/*******************************************************************************
* Function Name: objectFirstChange_SetPriority
********************************************************************************
*
* Summary:
*   Sets the Priority of the Interrupt. Note calling objectFirstChange_Start
*   or objectFirstChange_StartEx will override any effect this method 
*   would have had. This method should only be called after 
*   objectFirstChange_Start or objectFirstChange_StartEx has been called. To set 
*   the initial priority for the component use the cydwr file in the tool.
*
* Parameters:
*   priority: Priority of the interrupt. 0 - 7, 0 being the highest.
*
* Return:
*   None
*
*******************************************************************************/
void objectFirstChange_SetPriority(uint8 priority)
{
    *objectFirstChange_INTC_PRIOR = priority << 5;
}


/*******************************************************************************
* Function Name: objectFirstChange_GetPriority
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
uint8 objectFirstChange_GetPriority(void)
{
    uint8 priority;


    priority = *objectFirstChange_INTC_PRIOR >> 5;

    return priority;
}


/*******************************************************************************
* Function Name: objectFirstChange_Enable
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
void objectFirstChange_Enable(void)
{
    /* Enable the general interrupt. */
    *objectFirstChange_INTC_SET_EN = objectFirstChange__INTC_MASK;
}


/*******************************************************************************
* Function Name: objectFirstChange_GetState
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
uint8 objectFirstChange_GetState(void)
{
    /* Get the state of the general interrupt. */
    return ((*objectFirstChange_INTC_SET_EN & (uint32)objectFirstChange__INTC_MASK) != 0u) ? 1u:0u;
}


/*******************************************************************************
* Function Name: objectFirstChange_Disable
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
void objectFirstChange_Disable(void)
{
    /* Disable the general interrupt. */
    *objectFirstChange_INTC_CLR_EN = objectFirstChange__INTC_MASK;
}


/*******************************************************************************
* Function Name: objectFirstChange_SetPending
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
void objectFirstChange_SetPending(void)
{
    *objectFirstChange_INTC_SET_PD = objectFirstChange__INTC_MASK;
}


/*******************************************************************************
* Function Name: objectFirstChange_ClearPending
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
void objectFirstChange_ClearPending(void)
{
    *objectFirstChange_INTC_CLR_PD = objectFirstChange__INTC_MASK;
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
