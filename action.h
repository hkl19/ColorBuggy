#ifndef ACTION_H
#define ACTION_H
#include <xc.h>
#include "motor.h"

#define _XTAL_FREQ 64000000 

//combined motor functions for each color's response
void nullAction(struct motor *mL, struct motor *mR);
void redAction(char reverse,struct motor *mL, struct motor *mR);
void greenAction(char reverse, struct motor *mL, struct motor *mR);
void blueAction(char reverse, struct motor *mL, struct motor *mR);
void yellowAction(char reverse, struct motor *mL, struct motor *mR);
void pinkAction(char reverse, struct motor *mL, struct motor *mR);
void orangeAction(char reverse, struct motor *mL, struct motor *mR);
void lightBlueAction(char reverse, struct motor *mL, struct motor *mR);
void whiteAction(char reverse, struct motor *mL, struct motor *mR);
void blackAction(char reverse, struct motor *mL, struct motor *mR);

#endif