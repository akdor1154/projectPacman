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

/* [] END OF FILE */

#ifndef JARRAD_UTIL_H
#define JARRAD_UTIL_H
    
#include <includes.h>
#include <stdint.h>
    
#include "usbprint.h"
#include "device.h"

#ifdef __GNUC__
#  define UNUSED(x) UNUSED_ ## x __attribute__((__unused__))
#else
#  define UNUSED(x) UNUSED_ ## x
#endif
    
typedef enum {
    blue = 0x01,
    red = 0x02
} colour;

OS_ERR util_err;
extern colour lastSeenColour;

inline OS_ERR delaySeconds(uint16_t seconds) {
    OSTimeDlyHMSM(0,0,seconds,0, OS_OPT_TIME_HMSM_NON_STRICT, &util_err);
    return util_err;
}

inline OS_ERR delayMS(uint32_t milliseconds) {
    OSTimeDlyHMSM(0,0,0,milliseconds, OS_OPT_TIME_HMSM_NON_STRICT, &util_err);
    return util_err;
}

inline void statusReport() {
    usbprint("Left: %u\nRight: %u\nCentre: %u\n\n",proxLeftReg_Read(),proxRightReg_Read(),proxCentreReg_Read());
    usbprint("Middle Object: %u\nRear Object: %u\n\n",objectFirstReg_Read(),objectSecondReg_Read());
    usbprint("Colour: %u\nLast Seen Colour: %u\n",colourReg_Read(),lastSeenColour);
    usbprint("(red=%u,blue=%u)\n\n\n",red,blue);
}

#endif