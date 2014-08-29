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

// This Function pulses the Clock line
void pump();
void endSeq();
void startSeq();
void SccbWritePhase1(uint8 id, uint8 addr);
void SccbWrite(uint8 id, uint8 addr, uint8 byte);
uint8_t SccbRead(uint8 id, uint8 addr );

void CameraConfig();

#endif