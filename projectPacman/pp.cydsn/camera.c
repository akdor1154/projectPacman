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

// This Function pulses the Clock line
void pump() {
    CyDelayUs(3);
    ClockLine_Write(1);
    CyDelayUs(5);
    ClockLine_Write(0);
    CyDelayUs(3);
}

void endSeq() {
    ClockLine_Write(1);  //End Sequence
    CyDelayUs(3);
    DataLine_Write(1);
    CyDelayUs(50);
}

void startSeq() {
    DataLine_Write(0); // Start Transmission
    CyDelayUs(3);
    ClockLine_Write(0);
    CyDelayUs(5);
}

void SccbWritePhase1(uint8 id, uint8 addr) {
    int i;
    
    // Phase 1 Write
    startSeq();
    
    for( i=sizeof(id)*CHAR_BIT-1; i>=0;--i) {
    // write the device id
        int bit = (id>>i) & 1;
        DataLine_Write(bit);
        pump();
    }
    
    DataLine_Write(0); // Don't Care bit
    pump();
    // Phase 2 Write
    for( i=sizeof(addr)*CHAR_BIT-1; i>=0; --i) {
        // write the register
        int bit = (addr>>i) & 1;
        DataLine_Write(bit);
        pump();
    }    
    
    DataLine_Write(0); // Don't Care bit
    pump();
}

uint16_t SccbWrite(uint8 id, uint8 addr, uint8 byte) {
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
    usbprint("clearing..\n");
    status = cami2c_MasterClearStatus();
    usbprint("status was %u; cleared... starting...\n",status);
    ce(1,cami2c_MasterSendStart(id, cami2c_WRITE_XFER_MODE));
    usbprint("writing addr..\n");
    ce(2,cami2c_MasterWriteByte(addr));
    usbprint("writing byte..\n");
    ce(3,cami2c_MasterWriteByte(byte));
    usbprint("done..\n");
    ce(4,cami2c_MasterSendStop());
    return 0;
}

uint8_t SccbRead(uint8 id, uint8 addr ) {
    uint8_t sccbOut = 0;
    uint16_t errSrc = 0;
    uint8_t err = 0;
    uint8_t status = 0;
    usbprint("clearing..\n");
    status = cami2c_MasterClearStatus();
    usbprint("status was %u; cleared... starting...\n",status);
    ce(1,cami2c_MasterSendStart(id,cami2c_WRITE_XFER_MODE));
    usbprint("writing addr\n");
    ce(2,cami2c_MasterWriteByte(addr));
    ce(3,cami2c_MasterSendStop());
    usbprint("starting read\n");
    ce(4,cami2c_MasterSendStart(id,cami2c_READ_XFER_MODE));
    usbprint("reading..\n");
    usbprint("status pre-clear: %u",cami2c_MasterClearStatus());
    sccbOut = cami2c_MasterReadByte(1);
    usbprint("stopping\n");
    cami2c_MasterSendStop();
    usbprint("status post-read: %u",cami2c_MasterStatus());
    
    /*
    endSeq();
    startSeq();
    
    //Phase 1 Read
    id++; // for read
    for( i=sizeof(id)*CHAR_BIT-1; i>=0;--i) // write the device id+1 for read
    {
        int bit = (id>>i) & 1;
        DataLine_Write(bit);
        pump();
    }
    
    //Phase 2 read
    // read data 
    pump(); //don't care bit
    
    for( i=7; i>=0;--i) {
       
        sccbOut = sccbOut | ((DataLine_Read()&0x1)<<i);
        CyDelayUs(15);
        pump();
    }
    
    DataLine_Write(1);
    pump();
    DataLine_Write(0);
    CyDelayUs(4);   
 
    endSeq();
    */
    return sccbOut;
}

void CameraConfig() {
    // No reset pins
    //Cam_Powerdown_Write(0);
    //Cam_Reset_Write(1);
   uint8_t regRead;
    //SccbRead(0x42, 0x1d); // Manufacturer number
    usbprint("cam enabling...\n");
    cami2c_EnableInt();
    usbprint("cam starting...\n");
    cami2c_Start();
    usbprint("cam configuring...\n");
    uint16_t err;
    regRead = SccbRead(CAM_ADDRESS,0x15);
    usbprint("read reg pre-write: %u\n",regRead);
    err = SccbWrite(CAM_ADDRESS,0x15,0x20); // No PCLK during HREF low
    usbprint("camera err: %u\n",err);
    regRead = SccbRead(CAM_ADDRESS,0x15);
    usbprint("read reg post-write: %u\n",regRead);
    //SccbWrite(CAM_ADDRESS,0x11,0x8f); // clock output
    //SccbWrite(CAM_ADDRESS,0x12,0x25); // Output is CIF, BAYER RAW is 21
    //SccbWrite(CAM_ADDRESS,0x13,0x87); // Auto Gain Control, Auto White Balance, Auto Exposure
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