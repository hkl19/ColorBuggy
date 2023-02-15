#include <xc.h>
#include "motor.h"
#include "action.h"
#include "scan.h"

// Selection of robot response based upon color input
// call this function if val is 0-9
// if 8 or 9, it should return to origin with backToStart func. 
void colorCase(char val, char reverse, struct motor *mL, struct motor *mR) {
        switch(val){
            case 0:
                nullAction(mL,mR);
                break;
            case 1:    
                redAction(reverse,mL,mR);
                break;
            case 2:
                greenAction(reverse,mL,mR);
                break;
            case 3:
                blueAction(reverse,mL,mR);
                break;
            case 4:
                yellowAction(reverse,mL,mR);
                break;
            case 5:
                pinkAction(reverse,mL,mR);
                break;
            case 6:
                orangeAction(reverse,mL,mR);
                break;
            case 7:
                lightBlueAction(reverse,mL,mR);
                break;
            case 8:
                whiteAction(reverse, mL, mR);
                break;
            case 9:
                blackAction(reverse, mL, mR);
                break;
        }
}
