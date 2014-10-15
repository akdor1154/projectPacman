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

#include "device.h"
#include "usbprint.h"

#define CAM_ADDRESS 0x21
#define ce(n,X) err = X; if (err != cami2c_MSTR_NO_ERROR) { errSrc = n; return (errSrc << 8) | err; }
#if 0
uint16_t SccbWrite(uint8_t addr, uint8_t byte) {
    /*SccbWritePhase1(id, addr);
    int i;
    for( i=sizeof(byte)*CHAR_BIT-1; i>=0;--i) {
        // write the register
        int bit = (byte>>i) & 1;
        DataLine_Write(bit);
        pump();
    }    
    
    DataLine_Write(0); // Don't Care bit
    pump();
    endSeq();*/
    uint16_t errSrc = 0;
    uint8_t err = 0;
    uint8_t status = 0;
    status = cami2c_MasterClearStatus();
    if (status != 0) usbprint("status was %u; cleared... starting...\n",status);
    ce(1,cami2c_MasterSendStart(CAM_ADDRESS, cami2c_WRITE_XFER_MODE));
    ce(2,cami2c_MasterWriteByte(addr));
    ce(3,cami2c_MasterWriteByte(byte));
    ce(4,cami2c_MasterSendStop());
    return 0;
}

uint8_t SccbRead(uint8_t addr ) {
    uint8_t sccbOut = 0;
    uint16_t errSrc = 0;
    uint8_t err = 0;
    uint8_t status = 0;
    usbprint("clearing..\n");
    status = cami2c_MasterClearStatus();
    if (status != 0) usbprint("status was %u; cleared... starting...\n",status);
    ce(1,cami2c_MasterSendStart(CAM_ADDRESS,cami2c_WRITE_XFER_MODE));
    ce(2,cami2c_MasterWriteByte(addr));
    ce(3,cami2c_MasterSendStop());
    ce(4,cami2c_MasterSendStart(CAM_ADDRESS,cami2c_READ_XFER_MODE));
    status = cami2c_MasterClearStatus();
    if (status != 0) usbprint("status pre-clear: %u",status);
    sccbOut = cami2c_MasterReadByte(1);
    cami2c_MasterSendStop();
    status = cami2c_MasterStatus();
    if (status != 0) usbprint("status post-read: %u",cami2c_MasterStatus());
    
    return sccbOut;
}

void CameraConfig() {
    // No reset pins
    //Cam_Powerdown_Write(0);
    //Cam_Reset_Write(1);
   uint8_t regRead;
    //SccbRead(0x42, 0x1d); // Manufacturer number
    cami2c_EnableInt();
    cami2c_Start();
    uint16_t err;
    regRead = SccbRead(0x15);
    usbprint("read reg pre-write: %x\n",regRead);
    err = SccbWrite(0x15,0x20); // No PCLK during HREF low
    usbprint("camera err: %u\n",err);
    regRead = SccbRead(0x15);
    usbprint("read reg post-write: %x\n",regRead);
    SccbWrite(0x11,0x8f); // clock output
    SccbWrite(0x12,0x21); // Output is CIF, RGB
    SccbWrite(0x13,0x87); // Auto Gain Control, Auto White Balance, Auto Exposure
    SccbWrite(0x32,0x00); // HREF start
    SccbWrite(0x1b,0x00); // no buffer pixels
    SccbWrite(0x3a,0x04); //use dummy u,v; use output format YxYx
    SccbWrite(0x3d,0x99); //should be default, but to be sure (output u then v)
    //SccbWrite(0x67,69); //manual u
    //SccbWrite(0x68,96); //manual v
    //SccbWrite(CAM_ADDRESS,0x2b,0x08); //8 dummy pixels 
    //SccbWrite(CAM_ADDRESS,0x1e,0x31); //enable vertical flip and mirror 
    cami2c_Stop();
    /*
    SccbWrite(0x60,0x12,0x80); // initiates soft reset
    CyDelay(20);
    SccbWrite(0x60,0x12,0x00); // Binning 08 is 1/8 binning
    SccbWrite(0x60,0x11,0x7f); // Output clock divider
    SccbWrite(0x60,0x32,0xc0); //pixel clock divider 1/4
    SccbWrite(0x60,0x15,0x20); // PCLK output only when HREF high
    SccbWrite(0x60,0x80,0x11); //Color bar enable (Test pattern?)
    //SccbWrite(0x60,0x0c,0x0d); //  enable custom output
    //SccbWrite(0x60,0x61,0x1e); // 240
   // SccbWrite(0x60,0x60,0x19); // 240
    //SccbWrite(0x60,0x1b,0x05); //pixel shift
    //SccbWrite(0x60,0x09,0x04); //Slave Mode
    //SccbRead(0x60,0x12);
    */
}

 #endif 