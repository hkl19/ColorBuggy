#ifndef CASE_H
#define CASE_H
#include <xc.h>
#include "motor.h"
#include "action.h"
#include "scan.h"

#define _XTAL_FREQ 64000000

//single case function to select appropriate color response
void colorCase(char val, char reverse, struct motor *mL, struct motor *mR);

#endif
