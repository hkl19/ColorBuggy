#include <xc.h>
#include "motor.h"
#include "color.h"


// function initialise T2 and PWM for DC motor control
void initDCmotorsPWM(int PWMperiod,struct motor *mL, struct motor *mR){
	//initialise your TRIS and LAT registers for PWM
    TRISEbits.TRISE2 = 0;
    TRISEbits.TRISE4 = 0;
    TRISCbits.TRISC7 = 0;
    TRISGbits.TRISG6 = 0;
    LATEbits.LATE2 = 0;
    LATEbits.LATE4 = 0;
    LATCbits.LATC7 = 0;
    LATGbits.LATG6 = 0; 
    
    
    TRISFbits.TRISF2=1; //set TRIS value for pin (input)
    ANSELFbits.ANSELF2=0; //turn off analogue input on pin  

    // timer 2 configuration
    T2CONbits.CKPS=0b100; // 1:16 prescaler chosen, 
    T2HLTbits.MODE=0b00000; // Free Running Mode, software gate only
    T2CLKCONbits.CS=0b0001; // Fosc/4

    // Tpwm*(Fosc/4)/prescaler - 1 = PTPER
    T2PR=PWMperiod; //Period register 10kHz base period

    T2CONbits.ON=1;
    
    RE2PPS=0x0A; //PWM6 on RE2
    RC7PPS=0x0B; //PMW7 on RC7

    PWM6DCH=0; //0% power
    PWM7DCH=0; //0% power
    
    PWM6CONbits.EN = 1;
    PWM7CONbits.EN = 1;
    
    mL->dir_LAT = (unsigned char*)(&LATE); //Initialise LAT trigger for mL
    mL->dir_pin = 4; //set it to LATE4bits
    mR->dir_LAT = (unsigned char*)(&LATG); //Initialise LAT trigger for mR
    mR->dir_pin = 6;  //set it to LATG6bits
    mL->dutyHighByte = (unsigned char*)(&PWM6DCH); 
    mR->dutyHighByte = (unsigned char*)(&PWM7DCH);
    mL->PWMperiod=T2PR;
    mR->PWMperiod=T2PR;
}


// function to set PWM output from the values in the motor structure
void setMotorPWM(struct motor *m)
{
	int PWMduty; //tmp variable to store PWM duty cycle

	if (m->direction){ //if forward
		// low time increases with power
		PWMduty=m->PWMperiod - ((int)(m->power)*(m->PWMperiod))/100;
	}
	else { //if reverse
		// high time increases with power
		PWMduty=((int)(m->power)*(m->PWMperiod))/100;
	}

	*(m->dutyHighByte) = PWMduty; //set high duty cycle byte
        
	if (m->direction){ // if direction is high
		*(m->dir_LAT) = *(m->dir_LAT) | (1<<(m->dir_pin)); // set dir_pin bit in LAT to high without changing other bits
	} else {
		*(m->dir_LAT) = *(m->dir_LAT) & (~(1<<(m->dir_pin))); // set dir_pin bit in LAT to low without changing other bits
	}
}

//function to stop the robot gradually  
void stop(struct motor *mL, struct motor *mR)
{
    while (mL->power>0 && mR->power>0){
        mL->power = mL->power =0; 
        mR->power = mR->power =0;
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_us(10); 
        if (mL -> power <=0 && mR-> power <= 0)break;
            mL->power = 0; 
            mR->power = 0;
            setMotorPWM(mL);
            setMotorPWM(mR);       
        }
    }

//function to make the robot turn right 45 degrees
void turnCW45(struct motor *mL, struct motor *mR)

{   mL->power = 28; //initialise left motor power
    mR->power = 29; //initialise right motor power
    mL->direction = 1; //left motor goes forwards
    mR->direction = 0; //right motors goes forwards


    while (mL->power<30 && mR->power<30){
        mL->power = mL-> power +10 ;
        mR->power = mR-> power +10;
        setMotorPWM(mL);
        setMotorPWM(mR);   
        __delay_ms(300);    
    }
    stop(mL,mR);
}

    
//function to make the robot turn left 45 degrees 
void turnACW45(struct motor *mL, struct motor *mR)
{   mL->power = 29; //initialise left motor power
    mR->power = 28; //initialise right motor power
    mL->direction = 0; //left motor goes forwards
    mR->direction = 1; //right motors goes backwords

    while (mL->power<30 && mR->power<30){
        mL->power = mL->power +10 ;
        mR->power = mR->power +10;
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(300);   
    }
    stop(mL,mR);
}

//function to make buggy turn 180 degrees
void turn180(struct motor*mL, struct motor*mR){
    //set 2 different powers for motors of each side, to turn on central axis
    mL-> power = 53;
    mR-> power = 70;
    mL-> direction = 0;
    mR-> direction = 1;
     
    while (mL->power<60 && mR->power<75){
        mL->power = mL->power +10 ;
        mR->power = mR->power +10;
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(400);   
    }
    stop(mL,mR);
    
}

//function to make the robot go straight
void forward(struct motor *mL, struct motor *mR)
{   mL->power = 23; 
    mR->power = 23; 
    mL->direction = 1; 
    mR->direction = 1; 


    while (mL->power<24 && mR->power<24){
        mL->power = mL-> power ++ ;
        mR->power = mR-> power ++;
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(10);
        }
    stop(mL,mR);
}

//function to make the robot go backwards
void backward(struct motor *mL, struct motor *mR){
    mL->power = 70; //initialise left motor power
    mR->power = 73; //initialise right motor power
    mL->direction = 0; //left motor goes forwards
    mR->direction = 0; //right motors goes forwards


   while (mL->power<90 && mR->power<90){
        mL->power = mL-> power +10 ;
        mR->power = mR-> power +10;
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(250);
      }
    stop(mL,mR);
}

//function to make the robot go backwards
void adjustbackward(struct motor *mL, struct motor *mR){
    mL->power = 70; //initialise left motor power
    mR->power = 70; //initialise right motor power
    mL->direction = 0; //left motor goes forwards
    mR->direction = 0; //right motors goes forwards

        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(250);
    stop(mL,mR);
}

//function to make the robot go forwards
void adjustforward(struct motor *mL, struct motor *mR){
    mL->power = 80; 
    mR->power = 80; 
    mL->direction = 1; 
    mR->direction = 1; 


   while (mL->power<100 && mR->power<100){
        mL->power = mL-> power +10 ;
        mR->power = mR-> power +10;
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(100);
      }
    stop(mL,mR);
}

