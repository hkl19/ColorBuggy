// CONFIG1L
#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)

// CONFIG3L
#pragma config WDTCPS = WDTCPS_31// WDT Period Select bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

#include <xc.h>
#include <stdio.h>
#include <math.h>

#include "motor.h"
#include "action.h"
#include "scan.h"
#include "case.h"
#include "serial.h"
#include "i2c.h"
#include "color.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

void main(void){
//initialise all struct , pointers, and hardware   
    struct motor motorR;   //motor       
    struct motor motorL;
    struct storecolor sc;  //RGB store array
    struct getcolor cr;    //read instant RGB
    struct getcolor clear; //read instant clear
    
    struct storecolor sh;  //HSV store array
    struct getcolor hr;    //read instant HSV, converted from RGB
    struct getcolor hh; 
    
    unsigned int actArr[25];    //color memory array
    unsigned int fwdArr[25];    //time memory array
 
    char str[80];               //string for debugging
    unsigned char lenArr = 25;  //set the size of memory arrays
    unsigned char lenRead = 0;  //count number of cards read
    unsigned int fwdTime=0;     //count number of times buggy moved forward
    unsigned int PWMperiod =99; //PWM modulation of motor
       
    //Initiation functions
    initUSART4();
    I2C_2_Master_Init();
    color_click_init();
    initButton ();
    initDCmotorsPWM(PWMperiod,&motorL,&motorR);
    
    
   //Mode has 4 defaults, calibration(1), start(2), retrace(3), end(4)
    char mode = 1; 
    char reverse = 0;
    
    //calibration mode, start with buggy FLUSED TO THE WALL in the order of (R G B Y P O LB W B), then buggy around 7 cm away from wall (R Y P O LB W)        
    while (mode == 1){ 
        while(!PORTFbits.RF2){  //press RF2 button to move between color    
            calibrateColor(&sc);
            convertHSV(&sc,&sh);
            
            //DEBUGGING SCRIPT TO CHECK CALIBRATION VALUES
//            sprintf(str,"\r%d \t%d \t%d \t%d \t%d \t%d \t%d \t%d \t%d \t%d  \n", sh.R[0], sh.R[1], sh.R[2], sh.R[3],sh.R[4], sh.R[5], sh.R[6], sh.R[7], sh.R[8],sh.R[9]);
//            sendStringSerial4(str);
//            sprintf(str,"\r%d \t%d \t%d \t%d \t%d \t%d \t%d \t%d \t%d \t%d \n", sh.G[0], sh.G[1], sh.G[2], sh.G[3],sh.G[4], sh.G[5], sh.G[6], sh.G[7], sh.G[8],sh.G[9]);
//            sendStringSerial4(str);
//            sprintf(str,"\r%d \t%d \t%d \t%d \t%d \t%d \t%d \t%d \t%d \t%d \n", sh.B[0], sh.B[1], sh.B[2], sh.B[3],sh.B[4], sh.B[5], sh.B[6], sh.B[7], sh.B[8],sh.B[9]);
//            sendStringSerial4(str);
        }
    
        if (LATDbits.LATD7 == 1 && !PORTFbits.RF3){ //Press RF3 to start the buggy
            mode = 2;
        }
    }
    
    while (mode==2){  //running mode   
        
        //ACTUAL
            scanColor (&clear,&sc,&motorL,&motorR,&fwdTime); //scan color 
            fwdTimeArray(lenArr,&fwdArr,&fwdTime); //
            
            unsigned char v = findcolor(&cr,&hr,&sh,&lenRead); // 
            colorCase(v,0,&motorL,&motorR);
            actionArray(lenArr,&actArr,v);
            
            __delay_ms(200);
            
        //DEBUGGING SCRIPT TO CHECK ARRAYS
//            sprintf (str,"\r%d \t%d \t%d \t%d \t%d \t%d \t%d \t%d \n",actArr[0],actArr[1],actArr[2],actArr[3],actArr[4],actArr[5],actArr[6],actArr[7]);
//            sendStringSerial4(str);
//            sprintf (str,"\r%d \t%d \t%d \t%d \t%d \t%d \t%d \t%d \n",fwdArr[0],fwdArr[1],fwdArr[2],fwdArr[3],fwdArr[4],fwdArr[5],fwdArr[6],fwdArr[7]);
//            sendStringSerial4(str);
            
            if (v==8||v==9){
                mode = 3; 
            }
    }
            
            
        //MOTOR TOGGLE FOR CALIBRATION
//            int z[] = {1,2,3,4,5,6,7,8,9};
//            char j = 0;
//            for (j=0;j<9;j++){
//                colorCase(z[j],0,&motorL,&motorR);
//                __delay_ms(1000);
//            }
//    }
            

    //Change reverse to 1 for retrace mode 
    if (mode==3){
        reverse = 1;
    }
    
    //Retrace buggy to starting line
    while (mode==3) {
        backToStart(&lenRead, &actArr, &fwdTime, &fwdArr, reverse, &motorL, &motorR);
        while(1);
    }
}