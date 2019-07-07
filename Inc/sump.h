#ifndef _SUMP_H
#define _SUMP_H
#include <stdint.h>

#define TRUE  1u
#define FALSE 0u

typedef struct
{
  uint16_t delay;
  uint8_t channel;
  uint8_t level;
  uint8_t serial;
  uint8_t start;
} sumpTriggerConfigTypedef;

typedef struct
{
  uint32_t resetCount;
  uint8_t  isArmed;
  uint8_t isPaused;
  uint32_t triggerMask;
  uint32_t triggerValues;
  sumpTriggerConfigTypedef triggerConfig;
  uint32_t divider;
  uint16_t readCount;
  uint16_t delayCount;
  uint8_t  flags;
} sumpDataTypedef;

typedef struct
{
  uint8_t response[50];
  uint8_t length;
  
} sumpResponseTypedef;

//Global Data Declaration
extern sumpDataTypedef sumpData;
extern sumpResponseTypedef sumpResponse;

//Global Function Declarations

//Initializes the variables used for SUMP protocol to zero.
void SumpInit();

//Analyzes the data provided according to SUMP specificaions.
uint8_t SumpAnalyze(uint8_t* data);

//Check if command is long command or short command
uint8_t SumpIsLongCommand(uint8_t cmd);


#endif