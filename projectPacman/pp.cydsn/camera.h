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

#ifndef CAMERA_H
#define CAMERA_H

#include <stdint.h>
    
void SccbWrite(uint8_t addr, uint8_t byte);
uint8_t SccbRead(uint8_t addr );

void CameraConfig();

#endif