/*******************************************************************************
* File Name: int_pixelReady.c  
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
#include <int_pixelReady.h>

#if !defined(int_pixelReady__REMOVED) /* Check for removal by optimization */

/*******************************************************************************
*  Place your includes, defines and code here 
********************************************************************************/
/* `#START int_pixelReady_intc` */
#include <includes.h>
#include <Task_Defs.h>
OS_ERR sw2err;
extern unsigned int pixelCount;
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
* Function Name: int_pixelReady_Start
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
void int_pixelReady_Start(void)
{
    /* For all we know the interrupt is active. */
    int_pixelReady_Disable();

    /* Set the ISR to point to the int_pixelReady Interrupt. */
    int_pixelReady_SetVector(&int_pixelReady_Interrupt);

    /* Set the priority. */
    int_pixelReady_SetPriority((uint8)int_pixelReady_INTC_PRIOR_NUMBER);

    /* Enable it. */
    int_pixelReady_Enable();
}


/*******************************************************************************
* Function Name: int_pixelReady_StartEx
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
void int_pixelReady_StartEx(cyisraddress address)
{
    /* For all we know the interrupt is active. */
    int_pixelReady_Disable();

    /* Set the ISR to point to the int_pixelReady Interrupt. */
    int_pixelReady_SetVector(address);

    /* Set the priority. */
    int_pixelReady_SetPriority((uint8)int_pixelReady_INTC_PRIOR_NUMBER);

    /* Enable it. */
    int_pixelReady_Enable();
}


/*******************************************************************************
* Function Name: int_pixelReady_Stop
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
void int_pixelReady_Stop(void)
{
    /* Disable this interrupt. */
    int_pixelReady_Disable();

    /* Set the ISR to point to the passive one. */
    int_pixelReady_SetVector(&IntDefaultHandler);
}


/*******************************************************************************
* Function Name: int_pixelReady_Interrupt
********************************************************************************
*
* Summary:
*   The default Interrupt Service Routine for int_pixelReady.
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
CY_ISR(int_pixelReady_Interrupt)
{
    /*  Place your Interrupt code here. */
    /* `#START int_pixelReady_Interrupt` */
    /*lastPixelCb = pixelRegCb_Read();
    lastPixelCr = pixelRegCr_Read();*/
    /*CPU_SR_ALLOC();
    
    CPU_CRITICAL_ENTER();
    OSIntEnter();
    CPU_CRITICAL_EXIT();
    
    //stuff
    OSTaskSemPost(
        &Main_Task_TCB,
        OS_OPT_POST_NONE,
        &sw2err
    );*/
    pixelCount++;
    //OSIntExit();
    /* `#END` */
}


/*******************************************************************************
* Function Name: int_pixelReady_SetVector
********************************************************************************
*
* Summary:
*   Change the ISR vector for the Interrupt. Note calling int_pixelReady_Start
*   will override any effect this method would have had. To set the vector 
*   before the component has been started use int_pixelReady_StartEx instead.
*
* Parameters:
*   address: Address of the ISR to set in the interrupt vector table.
*
* Return:
*   None
*
*******************************************************************************/
void int_pixelReady_SetVector(cyisraddress address)
{
    cyisraddress * ramVectorTable;

    ramVectorTable = (cyisraddress *) *CYINT_VECT_TABLE;

    ramVectorTable[CYINT_IRQ_BASE + (uint32)int_pixelReady__INTC_NUMBER] = address;
}


/*******************************************************************************
* Function Name: int_pixelReady_GetVector
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
cyisraddress int_pixelReady_GetVector(void)
{
    cyisraddress * ramVectorTable;

    ramVectorTable = (cyisraddress *) *CYINT_VECT_TABLE;

    return ramVectorTable[CYINT_IRQ_BASE + (uint32)int_pixelReady__INTC_NUMBER];
}


/*******************************************************************************
* Function Name: int_pixelReady_SetPriority
********************************************************************************
*
* Summary:
*   Sets the Priority of the Interrupt. Note calling int_pixelReady_Start
*   or int_pixelReady_StartEx will override any effect this method 
*   would have had. This method should only be called after 
*   int_pixelReady_Start or int_pixelReady_StartEx has been called. To set 
*   the initial priority for the component use the cydwr file in the tool.
*
* Parameters:
*   priority: Priority of the interrupt. 0 - 7, 0 being the highest.
*
* Return:
*   None
*
*******************************************************************************/
void int_pixelReady_SetPriority(uint8 priority)
{
    *int_pixelReady_INTC_PRIOR = priority << 5;
}


/*******************************************************************************
* Function Name: int_pixelReady_GetPriority
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
uint8 int_pixelReady_GetPriority(void)
{
    uint8 priority;


    priority = *int_pixelReady_INTC_PRIOR >> 5;

    return priority;
}


/*******************************************************************************
* Function Name: int_pixelReady_Enable
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
void int_pixelReady_Enable(void)
{
    /* Enable the general interrupt. */
    *int_pixelReady_INTC_SET_EN = int_pixelReady__INTC_MASK;
}


/*******************************************************************************
* Function Name: int_pixelReady_GetState
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
uint8 int_pixelReady_GetState(void)
{
    /* Get the state of the general interrupt. */
    return ((*int_pixelReady_INTC_SET_EN & (uint32)int_pixelReady__INTC_MASK) != 0u) ? 1u:0u;
}


/*******************************************************************************
* Function Name: int_pixelReady_Disable
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
void int_pixelReady_Disable(void)
{
    /* Disable the general interrupt. */
    *int_pixelReady_INTC_CLR_EN = int_pixelReady__INTC_MASK;
}


/*******************************************************************************
* Function Name: int_pixelReady_SetPending
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
void int_pixelReady_SetPending(void)
{
    *int_pixelReady_INTC_SET_PD = int_pixelReady__INTC_MASK;
}


/*******************************************************************************
* Function Name: int_pixelReady_ClearPending
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
void int_pixelReady_ClearPending(void)
{
    *int_pixelReady_INTC_CLR_PD = int_pixelReady__INTC_MASK;
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
