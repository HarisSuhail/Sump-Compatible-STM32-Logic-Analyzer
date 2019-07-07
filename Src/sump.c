#include "sump.h"
#include <string.h>
//Definition of SUMP commands

//Short Commands

#define SUMP_RESET 0x00
#define SUMP_RUN   0x01
#define SUMP_ID    0x02
#define SUMP_XON   0x11
#define SUMP_XOFF  0x13


//Long Commands

#define SUMP_MASK_SET_TRIGGER           0xF3
#define SUMP_SET_TRIGGER_MASK           0xC0
#define SUMP_SET_TRIGGER_VALUES         0xC1
#define SUMP_SET_TRIGGER_CONFIG         0xC2

#define SUMP_SET_DIVIDER                0x80
#define SUMP_SET_READCOUNT              0x81
#define SUMP_SET_FLAGS                  0x82

// Global Variables
sumpDataTypedef sumpData;
sumpResponseTypedef sumpResponse;

// Local Variables
const static uint8_t metadata[] = {
  1, 'S', 'T', 'M', '3', '2', 'F', '0', 0, // device name
  2, '0', '.', '1', '3', 0,                // firmware version
  0x21, 0, 0, 40, 0,                       // sample memory = 40*256 = 10240 bytes
  0x23, 0, 0x5B, 0x8D, 80,                 // sample rate (6MHz)
  0x40, 8,                                 // number of probes = 8
  0x41, 2,                                 // protocol version 2
  0                                        // end of data
};

//Initializes the variables used for SUMP protocol to zero.
void SumpInit()
{
  memset(&sumpData, 0u, sizeof(sumpData));
}

//Analyzes the data provided according to SUMP specificaions.
uint8_t SumpAnalyze(uint8_t* data)
{
  uint8_t isResponsePrepared = FALSE;
  uint8_t cmd = data[0];
  if(cmd == SUMP_RESET)
  {
    sumpData.resetCount++;
  }
  else if (cmd == SUMP_RUN)
  {
    sumpData.isArmed = TRUE;
  }
  else if (cmd == SUMP_ID)   
  {
    char resp[] = "1ALS";
    sumpResponse.length = strlen(resp);
    memcpy(sumpResponse.response, resp, sumpResponse.length);
    isResponsePrepared = TRUE;
  }
  else if (cmd == SUMP_XON)
  {
    sumpData.isPaused = FALSE;
  }
  else if (cmd == SUMP_XOFF)
  {
    sumpData.isPaused = TRUE;
  }
  else if ((cmd & SUMP_MASK_SET_TRIGGER) == SUMP_SET_TRIGGER_MASK)
  {
    sumpData.triggerMask = (data[1]) | ((uint32_t)data[2] << 8) | ((uint32_t)data[3] << 16) | ((uint32_t)data[4] << 24);
  }
  else if((cmd & SUMP_MASK_SET_TRIGGER) == SUMP_SET_TRIGGER_VALUES)
  {
    sumpData.triggerValues = (data[1]) | ((uint32_t)data[2] << 8) | ((uint32_t)data[3] << 16) | ((uint32_t)data[4] << 24);
  }
  else if((cmd & SUMP_MASK_SET_TRIGGER) == SUMP_SET_TRIGGER_CONFIG)
  {
    sumpData.triggerConfig.delay = (data[1]) | ((uint32_t)data[2] << 8);
    sumpData.triggerConfig.level = (0x03 & data[3]);
    sumpData.triggerConfig.start = (0x08 & data[4]) >> 3;
    sumpData.triggerConfig.serial= (0x04 & data[4]) >> 2;
    sumpData.triggerConfig.channel = ((0x01 & data[4]) << 4) | ((0xF0 & data[3]) >> 4);
  }
  else if(cmd == SUMP_SET_DIVIDER)
  {
    sumpData.divider = (data[1]) | ((uint32_t)data[2] << 8) | ((uint32_t)data[3] << 16);
  }
  else if(cmd == SUMP_SET_READCOUNT)
  {
    sumpData.readCount = (data[1]) | ((uint32_t)data[2] << 8);
    sumpData.resetCount = (data[3]) | ((uint32_t)data[4] << 8);
  }
  else if(cmd == SUMP_SET_FLAGS)
  {
    sumpData.flags = data[1];
  }
  else if (cmd == 0x04)
  {
    sumpResponse.length = sizeof(metadata);
    memcpy(sumpResponse.response, metadata ,sumpResponse.length);
    isResponsePrepared = TRUE;
  }
  else
  {
    //Error - Wrong Sump Command
  }
  return isResponsePrepared;
}



//Check if command is long command or short command
uint8_t SumpIsLongCommand(uint8_t cmd)
{
  uint8_t isLong = FALSE;
  if((cmd == SUMP_SET_FLAGS) || (cmd == SUMP_SET_READCOUNT) || (cmd == SUMP_SET_DIVIDER)\
     || ((cmd & SUMP_MASK_SET_TRIGGER) == SUMP_SET_TRIGGER_CONFIG)\
     || ((cmd & SUMP_MASK_SET_TRIGGER) == SUMP_SET_TRIGGER_VALUES)\
     || ((cmd & SUMP_MASK_SET_TRIGGER) == SUMP_SET_TRIGGER_MASK))
  {
    isLong = TRUE;
  }
  
  return isLong;
  
}












