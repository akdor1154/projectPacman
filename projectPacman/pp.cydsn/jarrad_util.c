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

#include "jarrad_util.h"

OS_ERR err;

void delaySeconds(uint16_t seconds) {
    OSTimeDlyHMSM(0,0,seconds,0, OS_OPT_TIME_HMSM_NON_STRICT, &err);
}