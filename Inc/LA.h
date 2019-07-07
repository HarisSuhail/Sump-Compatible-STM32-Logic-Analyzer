#ifndef _LOGIC_ANALYZER_H
#define _LOGIC_ANALYZER_H
#include <stdint.h>

//Defines
#define SAMPLESIZE 0x2FFF
#define TIMERBASEFREQ  72000000u
#define SUMPBASEFREQ  100000000u

//Global variables
extern uint8_t capturedData[SAMPLESIZE];



//Global Functions
void LASetSamplingFreq(uint32_t divider);
void LABeginSampling  (uint32_t numberOfSamples);
uint8_t LAIsSamplingComplete();


#endif
