#include <xc.h>
#include "motor.h"
#include "action.h"
#include "scan.h"
#include "color.h"
#include "i2c.h"

/***************//*
 *  Stop buggy at the end of backToStart function                
 *****************/
void nullAction(struct motor *mL, struct motor *mR){
    stop(mL,mR);
}
/***************//*
 *  Turn buggy 90 degree clockwise when red shown
 *  Turn buggy 90 degree anticlockwise on reverse mode                 
 *****************/
void redAction(char reverse, struct motor *mL, struct motor *mR){
    if (reverse==0){
        __delay_ms(1000);
        adjustbackward(mL,mR);
        __delay_ms(1000);
        turnCW45(mL,mR);
        turnCW45(mL,mR);
        __delay_ms(1000); 
    }
    else {
        turnACW45(mL,mR);
        turnACW45(mL,mR);
        adjustbackward(mL,mR);
        adjustbackward(mL,mR);
        __delay_ms(1000);
    }
}
/***************//*
 *  Turn buggy 90 degree anticlockwise when reading gree
 *  Turn buggy 90 degree clockwise on reverse mode                 
 *****************/
void greenAction(char reverse, struct motor *mL, struct motor *mR){
    if (reverse ==0){
        __delay_ms(1000);
        adjustbackward(mL,mR);
        __delay_ms(1000);
        turnACW45(mL,mR);
        turnACW45(mL,mR);
        __delay_ms(1000);
    }
    else {
        turnCW45(mL,mR);
        turnCW45(mL,mR);
        adjustbackward(mL,mR);
        adjustbackward(mL,mR);
        __delay_ms(1000);
    }
}
/***************//*
 *  Turn buggy 180 degree when reading blue                
 *****************/
void blueAction(char reverse, struct motor *mL, struct motor *mR){
        __delay_ms(1000);
        adjustbackward(mL,mR);
        __delay_ms(1000);
        turn180(mL,mR);
        __delay_ms(1000);
}
/***************//*
 *  Turn buggy backward one square, then 90 degree Clockwise when reading yellow
 *  Turn buggy 90 anticlockwise, then backup for readjustment during reverse mode              
 *****************/
void yellowAction(char reverse, struct motor *mL, struct motor *mR){
    if (reverse == 0){
        __delay_ms(1000);
        adjustbackward(mL,mR);
        __delay_ms(1000);
        backward(mL,mR);
        __delay_ms(500);
        turnCW45(mL,mR);
        turnCW45(mL,mR);
        __delay_ms(1000);
    }
    else {
        turnACW45(mL,mR);
        turnACW45(mL,mR);
        __delay_ms(100);
        backward(mL,mR);
        adjustbackward(mL,mR);
        adjustbackward(mL,mR);
        __delay_ms(1000);
    }
}

/***************//*
 *  Turn buggy backward one square, then 90 degree ACW when reading pink
 *  Turn buggy 90 CW, then backup for readjustment during reverse mode              
 *****************/
void pinkAction(char reverse, struct motor *mL, struct motor *mR){
    if (reverse == 0){
        __delay_ms(1000);
        adjustbackward(mL,mR);
        __delay_ms(1000);
        backward(mL,mR);
        __delay_ms(500);
        turnACW45(mL,mR);
        turnACW45(mL,mR);
        __delay_ms(1000);
    }
    else {
        turnCW45(mL,mR);
        turnCW45(mL,mR);
        __delay_ms(1000);
        backward(mL,mR);
        __delay_ms(500);
        adjustbackward(mL,mR);
        __delay_ms(1000);
        adjustbackward(mL,mR);
        __delay_ms(1000);
    }
}
/***************//*
 *  Turn buggy 135 degree CW when reading orange
 *  Turn buggy 135 degree ACW during reverse          
 *****************/
void orangeAction(char reverse, struct motor *mL, struct motor *mR){
    if (reverse==0){
        __delay_ms(1000);
        adjustbackward(mL,mR);
        __delay_ms(1000);
        turnCW45(mL,mR);
        turnCW45(mL,mR);
        turnCW45(mL,mR);
        __delay_ms(1000);
    }
    else {
        turnACW45(mL,mR);
        turnACW45(mL,mR);
        turnACW45(mL,mR);
        adjustbackward(mL,mR);
        __delay_ms(1000);
    }
}

/***************//*
 *  Turn buggy 135 degree ACW when reading light blue
 *  Turn buggy 135 degree CW during reverse          
 *****************/
void lightBlueAction(char reverse, struct motor *mL, struct motor *mR){
    if (reverse==0){
        __delay_ms(1000);
        adjustbackward(mL,mR);
        __delay_ms(1000);
        turnACW45(mL,mR);
        turnACW45(mL,mR);
        turnACW45(mL,mR);
        __delay_ms(1000);
    }
    else {
        turnCW45(mL,mR);
        turnCW45(mL,mR);
        turnCW45(mL,mR);
        adjustbackward(mL,mR);
        __delay_ms(1000);
    }
}
/***************//*
 *  Turn 180 degrees when white shown
 *  Trigger return to start mode     
 *****************/
void whiteAction(char reverse, struct motor *mL, struct motor *mR){
    if (reverse==0){
        adjustbackward(mL,mR);
        __delay_ms(1000);
        turn180(mL,mR);
        __delay_ms(1000);
        adjustbackward(mL,mR);
        adjustbackward(mL,mR);
    }
}
/***************//*
 *  Issue response to re-calibrate rover    
 *****************/   
void blackAction(char reverse, struct motor *mL, struct motor *mR){
    if (reverse==0){
        adjustbackward(mL,mR);
        __delay_ms(1000);
        turn180(mL,mR);
        __delay_ms(1000);
        adjustbackward(mL,mR);
        adjustbackward(mL,mR);
    }
}

//Note: if white or black shown, buggy will return to starting point.
//make another function to invert the arr and ascend the color back to origin.