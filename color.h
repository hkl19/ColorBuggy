#ifndef _color_H
#define _color_H

#include <xc.h>
#include "motor.h"
#include "timer.h"
#define _XTAL_FREQ 64000000 

// Use int, char will cause overflow
// Structure to store RGBC and HSV color values from calibration 
struct storecolor {
    unsigned int R[15] ;
    unsigned int G[15] ;
    unsigned int B[15] ;
    unsigned int C[15] ;
};
// Structure to store RGBC and HSV color values during scanning 
struct getcolor {
    unsigned int RR;
    unsigned int GG;
    unsigned int BB;
    unsigned int CC;
};

//Functions related to color calibration, scanning and finding
void initButton(void) ;
void color_click_init(void);
void color_writetoaddr(char address, char value);
void convertHSV(struct storecolor *sc, struct storecolor *sh);
void findHSV(struct getcolor *cr,struct getcolor *hr);
unsigned int color_read_Red(void);
unsigned int color_read_Green(void);
unsigned int color_read_Blue(void);
unsigned int color_read_Clear(void);
void turnonLED (unsigned int RL, unsigned int GL, unsigned int BL);
void calibrateColor (struct storecolor *sc); 
void scanColor (struct getcolor *clear, struct storecolor *sc, struct motor *mL, struct motor *mR,unsigned int *fwdTime);
unsigned int findcolor (struct getcolor *cr, struct getcolor *hr, struct storecolor *sh,char *lenRead);
#endif