#ifndef MOTOR_H
#define MOTOR_H
#define turnPower 30 // defining threshold for 45 degree turn
#define adjustPower 30 // defining threshold for adjuster
#define forwardPower 70 // defining threshold for accelerating one square
#define backwardsPower 70 // defining threshold for reversing one square 
#define basePeriod 0.0001 //Period fo 10kHz base in seconds
#include <xc.h>

#define _XTAL_FREQ 64000000 // Defining oscillator frequency 

//Structure containing all motor variables
struct motor {
    char power; 
    char direction;
    unsigned char *dutyHighByte;
    unsigned char *dir_LAT;
    char dir_pin;
    unsigned int PWMperiod ;
};

//fundamental functions for the motor response
void initDCmotorsPWM(int PWMperiod, struct motor *mL, struct motor *mR); 
void setMotorPWM(struct motor *m);
void stop(struct motor *mL, struct motor *mR);
void forward(struct motor *mL, struct motor *mR);
void backward(struct motor *mL, struct motor *mR);
void adjustforward(struct motor *mL, struct motor *mR);
void adjustbackward(struct motor *mL, struct motor *mR);
void turnCW45(struct motor *mL, struct motor *mR);
void turnACW45(struct motor *mL, struct motor *mR);
void turn180(struct motor *mL, struct motor *mR);

#endif  