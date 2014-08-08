/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>

#include <stdio.h>
#include <stdarg.h>

void print(const char *fmt, ...)
{
   char buffer[64];
   va_list args;
   va_start(args, fmt);
   vsnprintf(buffer, sizeof(buffer), fmt, args);
   va_end(args);
   while(!USB_CDCIsReady());
   USB_PutString(buffer);
}

int main()
{
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    CyGlobalIntEnable; /* Uncomment this line to enable global interrupts. */
    USB_Start(0,USB_DWR_VDDD_OPERATION);
    while(!USB_GetConfiguration());
    USB_CDC_Init();
    
    for(;;)
    {
        CyDelay(20);
        /* Place your application code here. */
        print("spam\n");
    }
}

/* [] END OF FILE */
