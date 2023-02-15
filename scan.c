#include <xc.h>
#include "motor.h"
#include "action.h"
#include "case.h"
#include "timer.h"
#include "interrupt.h"


//This array function should store data regarding each scan
//colorScan determines the response of the robot (int 0-9)
//store it in an pre-built array
//increment the array size 
//note we must point *actionArr to actionArray[0] to initiate it in main
void actionArray(char lenArr, int *actArr, char colorScan){
        static char i = 0;
        if (i < lenArr){
            *(actArr+i) = colorScan;
            i ++;
        }
}

//Similarly for time data, store information in seperate array
void fwdTimeArray(char fwdSize, int *fwdArr, int *fwdTime){
        static char j = 0;
        if (j<fwdSize){
            *(fwdArr+j) = *fwdTime;
            *fwdTime = 0;
            j ++;
        }
}

//this function comes when mode 4 is called
//allows buggy to retrace back to its starting point 
//runs fwdTimeArr and actionArr backwards, according to amount of colors scanned
void backToStart(char *lenRead, unsigned int *actArr, unsigned int *fwdTime, unsigned int *fwdArr, char reverse, struct motor *mL, struct motor *mR){
        unsigned char val;
       *fwdTime = 0;

        for (char k=(*lenRead-1);k>=0;k--){
                val = *(actArr+k-1);
            while(*fwdTime < *(fwdArr+k)){
                forward(mL,mR);
                *fwdTime = *fwdTime + 1;
            }
            colorCase(val,reverse,mL,mR);
            *fwdTime = 0;
        }
}