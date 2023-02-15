#ifndef SCAN_H
#define SCAN_H
#include <xc.h>
#include "motor.h"
#include "action.h"
#include "timer.h"

#define _XTAL_FREQ 64000000

//functions for building color and time arrays
//includes final backToStart return function 
void actionArray(char actSize, int *actArr, char colorScan);
void fwdTimeArray(char fwdSize, int *fwdArr, int *fwdTime);
void backToStart(char *lenRead, unsigned int *actArr, unsigned int *fwdTime, unsigned int *fwdArr, char reverse, struct motor *mL, struct motor *mR);
#endif