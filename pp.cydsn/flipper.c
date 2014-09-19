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

#include "flipper.h"

void flipperUp() {
    ServoPWM_WriteCompare(FLIPPER_UP);
}

void flipperDown() {
    ServoPWM_WriteCompare(FLIPPER_DOWN);
}

