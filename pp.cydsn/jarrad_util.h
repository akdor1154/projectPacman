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

inline OS_ERR delaySeconds(uint16_t seconds) {
    OSTimeDlyHMSM(0,0,seconds,0, OS_OPT_TIME_HMSM_NON_STRICT, &util_err);
    return util_err;
}

inline OS_ERR delayMS(uint32_t milliseconds) {
    OSTimeDlyHMSM(0,0,0,milliseconds, OS_OPT_TIME_HMSM_NON_STRICT, &util_err);
    return util_err;
}

#endif