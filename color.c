#include <xc.h>
#include <stdbool.h>
#include <stdlib.h>
#include "color.h"
#include "i2c.h"
#include "motor.h"
#include "serial.h"
#include "action.h"
#include "timer.h"
#include "scan.h"

/***************//*
 *  Function to initialise the buttons used
 *****************/
void initButton(void){
    // setup pin for input (connected to button)
    TRISFbits.TRISF2=1; //set TRIS value for pin 2 (input)
    TRISFbits.TRISF3=1; // set TRIS value for pin 3 (input)
    ANSELFbits.ANSELF2=0; //turn off analogue input on pin  
    ANSELFbits.ANSELF3=0; //turn off analogue input on pin
    
    // setup pin for output (connected to LED)
    LATDbits.LATD7=0;   //set initial output state
    TRISDbits.TRISD7=0; //set TRIS value for pin (output)
    LATHbits.LATH3=0;   //set initial output state
    TRISHbits.TRISH3=0; //set TRIS value for pin (output) 
    LATDbits.LATD4=0;   //set initial output state
    TRISDbits.TRISD4=0; //set TRIS value for pin (output)
}
/***************//*
 *  Function to initialise the colour click module using I2C
 *****************/
void color_click_init(void)
{   
    //setup colour sensor via i2c interface
    I2C_2_Master_Init();      //Initialise i2c Master

     //set device PON
	 color_writetoaddr(0x00, 0x01);
    __delay_ms(3); //need to wait 3ms for everthing to start up
    
    //turn on device ADC
	color_writetoaddr(0x00, 0x03);

    //set integration time
	color_writetoaddr(0x01, 0xD5);
}
/***************//*
 *  Function to write to the colour click module
 *  address is the register within the colour click to write to
 *	value is the value that will be written to that address
 *****************/
void color_writetoaddr(char address, char value){
    I2C_2_Master_Start();         //Start condition
    I2C_2_Master_Write(0x52 | 0x00);     //7 bit device address + Write mode
    I2C_2_Master_Write(0x80 | address);    //command + register address
    I2C_2_Master_Write(value);    
    I2C_2_Master_Stop();          //Stop condition
}
/***************//*
 *  Function to read the red channel
 *	Returns a 16 bit ADC value representing colour intensity
 *****************/
unsigned int color_read_Red(void)
{
	unsigned int tmp;
	I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode
	I2C_2_Master_Write(0xA0 | 0x16);    //command (auto-increment protocol transaction) + start at RED low register
	I2C_2_Master_RepStart();			// start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);			//read the Red LSB
	tmp=tmp | (I2C_2_Master_Read(0)<<8); //read the Red MSB (don't acknowledge as this is the last read)
	I2C_2_Master_Stop();          //Stop condition
	return tmp;
}
/***************//*
 *  Function to read the Green channel
 *	Returns a 16 bit ADC value representing colour intensity
 *****************/
unsigned int color_read_Green(void)
{
	unsigned int tmp;
	I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode
	I2C_2_Master_Write(0xA0 | 0x18); //   //command (auto-increment protocol transaction) + start at RED low register
	I2C_2_Master_RepStart();			// start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);			//read the Red LSB
	tmp=tmp | (I2C_2_Master_Read(0)<<8); //read the Red MSB (don't acknowledge as this is the last read)
	I2C_2_Master_Stop();          //Stop condition
	return tmp;
}
/***************//*
 *  Function to read the Blue channel
 *	Returns a 16 bit ADC value representing colour intensity
 *****************/
unsigned int color_read_Blue(void)
{
	unsigned int tmp;
	I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode
	I2C_2_Master_Write(0xA0 | 0x1A); //   //command (auto-increment protocol transaction) + start at RED low register
	I2C_2_Master_RepStart();			// start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);			//read the Red LSB
	tmp=tmp | (I2C_2_Master_Read(0)<<8); //read the Red MSB (don't acknowledge as this is the last read)
	I2C_2_Master_Stop();          //Stop condition
	return tmp;
}
/***************//*
 *  Function to read the clear channel
 *	Returns a 16 bit ADC value representing colour intensity
 *****************/
unsigned int color_read_Clear(void) // 
{
	unsigned int tmp;
	I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode
	I2C_2_Master_Write(0xA0 | 0x14); //   //command (auto-increment protocol transaction) + start at RED low register
	I2C_2_Master_RepStart();			// start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);			//read the Red LSB
	tmp=tmp | (I2C_2_Master_Read(0)<<8); //read the Red MSB (don't acknowledge as this is the last read)
	I2C_2_Master_Stop();          //Stop condition
	return tmp;
}
/***************//*
 *  Function to turn on back-light LED
 *	To call :
     * (1,0,0) for red
     * (0,1,0) for green
     * (0,0,1) for blue
     * (1,1,1) for clear light
 *****************/
void turnonLED (unsigned int RL, unsigned int GL, unsigned int BL)
{
    //Pin initialisation
    TRISGbits.TRISG1=0; //set TRIS value for pin (output), AN in MIKROBUS 1, LED_R  
    TRISAbits.TRISA4=0; //set TRIS value for pin (output), CS in MIKROBUS 1, LED_G
    TRISFbits.TRISF7=0; //set TRIS value for pin (output), PWM in MIKROBUS 1, LED_RB

    if (RL == 1){
   LATGbits.LATG1=1;   //set initial output state, red        
    }
    else {
    LATGbits.LATG1=0;   //set initial output state, red               
    }
    
    if (GL==1){
    LATAbits.LATA4=1;   //set initial output state, green        
    }
    else{
    LATAbits.LATA4=0;   //set initial output state, green   
    }

    
    if (BL==1){
    LATFbits.LATF7=1;   //set initial output state, BL      
    } 
    else{
    LATFbits.LATF7=0;   //set initial output state
    }

}
/***************//*
 *  convert RGB values from the structure to HSV, then replace the structure
 *  function called in calibration mode
 *  HSV values not normalised to 256
 *  Referenced from Youtube: https://www.youtube.com/watch?v=I8i0W8ve-JI&t=666s
 *****************/
void convertHSV (struct storecolor *sc, struct storecolor *sh){
    double minValue, maxValue;
    char i;
    for (i=0;i<10;i++){
    unsigned char maxIndex;
    
    //case if no color found (RGB = 0,0,0)
    if ((sc->R[i]==sc->G[i])&&(sc->R[i]==sc->B[i])){
        maxIndex = 0;
        maxValue = sc->R[i];
        minValue = sc->R[i];
    }
    //case if red value is max (H between 300-60degrees)
    else if ((sc->R[i]>=sc->G[i])&&(sc->R[i]>=sc->B[i])){
        maxIndex = 1;
        maxValue = sc->R[i];
        minValue = (sc->G[i]<sc->B[i])?sc->G[i] : sc->B[i];
    }
    //case if green value is max (H between 60-180degrees)
    else if ((sc->G[i]>=sc->R[i])&&(sc->G[i]>=sc->B[i])){
        maxIndex = 2;
        maxValue = sc->G[i];
        minValue = (sc->R[i]<sc->B[i])?sc->R[i]:sc->B[i];
    }
    //case if green value is max (H between 180-300degrees)
    else {
        maxIndex = 3;
        maxValue = sc->B[i];
        minValue = (sc->R[i]<sc->G[i])?sc->R[i]:sc->G[i];
    }
    
    //convert these RGB values into Hue values
    double h;
    switch(maxIndex)     
    {
        case 0: h = 0;
        break;
        case 1: h = 60 *((sc->G[i]-sc->B[i])/(maxValue-minValue));     //case of R max
        break;  
        case 2 : h = 60 *(2+((sc->B[i]-sc->R[i])/(maxValue-minValue)));//case of G max
        break;
        case 3 : h = 60 *(4+((sc->R[i]-sc->G[i])/(maxValue-minValue)));//case of B max
        break;
    }
    
    //prevent negative h values
    if (h<0){
        h += 360;
    }
    
    //range between max to min is saturation * value (S*V)
    //s = range/v OR =(max-min)/max
    double s;
    unsigned int ss;
    s = (maxIndex==0)? 0:(maxValue-minValue)/maxValue;
    ss = 100*s;  //multiplied 100 to yield readable saturation values
    
    //value defined as maxValue
    double v = maxValue;
    
    //store HSV values into data structure
    sh->R[i] = h;
    sh->G[i] = ss;
    sh->B[i] = v;
}

}
/***************//*
 *  convert RGB to HSV, when reading color
 *  function called within findcolor
 *  similar explanation as above
 *****************/   
void findHSV(struct getcolor *cr,struct getcolor *hr){
    double minValue, maxValue;
    unsigned char maxIndex;
    if ((cr->RR==cr->GG)&&(cr->RR==cr->BB)){
        maxIndex = 0;
        maxValue = cr->RR;
        minValue = cr->RR;
    }
    else if ((cr->RR>=cr->GG)&&(cr->RR>=cr->BB)){
        maxIndex = 1;
        maxValue = cr->RR;
        minValue = (cr->GG<cr->BB)?cr->GG : cr->BB;
    }
    else if ((cr->GG>=cr->RR)&&(cr->GG>=cr->BB)){
        maxIndex = 2;
        maxValue = cr->GG;
        minValue = (cr->RR<cr->BB)?cr->RR:cr->BB;
    }
    else {
        maxIndex = 3;
        maxValue = cr->BB;
        minValue = (cr->RR<cr->GG)?cr->RR:cr->GG;
    }
    double h;
    switch(maxIndex)
    {
        case 0: h = 0;
        break;
        case 1: h = 60 *((cr->GG-cr->BB)/(maxValue-minValue));
        break;
        case 2 : h = 60 *(2+((cr->BB-cr->RR)/(maxValue-minValue)));
        break;
        case 3 : h = 60 * (4+((cr->RR-cr->GG)/(maxValue-minValue)));
        break;
    }
    if (h<0.0){
        h += 360;
    }
    double s;
    unsigned int ss;
    s = (maxIndex==0)? 0:(maxValue-minValue)/maxValue;
    ss = 100*s;
    double v = maxValue;
    
    hr->RR = h;
    hr->GG = ss;
    hr->BB = v;
}

/**************//*
 *  Function to store RGB  of the cards, used to calibrate card
 *  Press RF2 to move on between colors
 *  Start with buggy FLUSED TO THE WALL in the order of (R G B Y P O LB W B)
 *  then buggy around 7 cm away from wall (R Y P O LB W) --> to register where to stop
 *  R Y P O LB W are chosen because they are the more reflective colors
 *****************/
void calibrateColor (struct storecolor *sc) {
    static char i = 0; //Start count

    turnonLED (1,1,1);
    __delay_ms(500);

    sc->C[i] = color_read_Clear(); //store value in array
    sc->R[i] = color_read_Red();
    sc->G[i] = color_read_Green();
    sc->B[i] = color_read_Blue();

    i++;
    turnonLED (0,0,0);    
    if (i == 15) {
         LATDbits.LATD7 = 1; //toggle D7 LED when finished, warning system to not overflow the array and move on to next mode
    }
}
    
/**************//*
 *  Function to register when to stop and read color
 *  Threshold set according mean brightness values during calibration
 *****************/
void scanColor (struct getcolor *clear, struct storecolor *sc, struct motor *mL, struct motor *mR,unsigned int *fwdTime) {

    while(LATHbits.LATH3 == 0){
        turnonLED (1,1,1);           //turn LED on such as during calibration

        clear->CC = (color_read_Clear() * 2);
        
        
        unsigned int C_max = 0;
        static char i;
    
        for(int i = 9; i < 15; i++){ //find max clear value during calibration
//            if(C_min > sc->C[i])
//                C_min = sc->C[i];
            if(C_max < sc->C[i])
                C_max = sc->C[i];
        }
        
        //if threshold met, color is found 
        if(clear->CC >= C_max){
            stop(mL,mR);
            __delay_ms(500);
            LATHbits.LATH3 = 1;
            adjustforward(mL,mR);   //allows more accurate reading distance
        }
        
        //if threshold not met, continue forward
        else {
            LATHbits.LATH3 = 0;
            forward(mL,mR);
            *fwdTime = *fwdTime +1 ;//mock timer to measure distance travelled 
        }
    }
}

/**************//*
 *  Function to register which color was scanned 
 *  Particular colors require logic and double verification for accurate results (R O Y P W)
 *****************/
unsigned int findcolor (struct getcolor *cr, struct getcolor *hr, struct storecolor *sh,char *lenRead){
      
    turnonLED (1,1,1);
    __delay_ms(200);
    cr->RR = color_read_Red();
     __delay_ms(200);
    cr->GG = color_read_Green();
    __delay_ms(200);
    cr->BB = color_read_Blue();
     __delay_ms(200);
     turnonLED(0,0,0);
     
    findHSV(cr,hr);
    //set tolerance for HSV values
    //if target color not found within calibrated data, increase tolerance 
    unsigned int tolH = 1;
    unsigned char tolS = 1;
    unsigned int tolV = 1;
    char col = 0;
    char a = 0;
    char b = 0;
    char c= 0;

    while (col == 0){
        char i=0;
        for (i=0;i<9;i++){
            //compare hue 
            if (hr->RR>(sh->R[i]-tolH)&&hr->RR<(sh->R[i]+tolH)){
                a = i+1;
                char j = 0;
            
                for (j=0;j<9;j++){
                    //compare saturation
                    if (hr->GG>(sh->G[j]-tolS)&&hr->GG<(sh->G[j]+tolS)){
                        b = j+1;
                        char k =0;
                    
                        for (k=0;k<9;k++){
                            //compare value
                            if (hr->BB>(sh->B[k]-tolV)&&hr->BB<(sh->B[k]+tolV)){
                                c = k+1;
                            }
                            else {
                                tolV ++;
                            }
                        }
                    }
                    else {
                        tolS ++; 
                        }
                }
            }
            else{
                tolH ++;
            }
        }
        
    // logic statements to select most correct color guess
    //see readMe file for detailed description 
        if (a==b && b==c){
            col = a;
        }    
        else if (b==c && a!=b){
            col = b;
        }    
        else {
            col = a;
        }
    
    // double verification for R vs O based up H values 
        if (col ==1 || col ==6){
            turnonLED (1,0,0);
            cr->RR = color_read_Red();
            __delay_ms(200);
            turnonLED (0,1,0);
            cr->GG = color_read_Green();
            __delay_ms(200);
            turnonLED (0,0,1);
            cr->BB = color_read_Blue();
            __delay_ms(200);
            turnonLED(0,0,0);
            findHSV(cr,hr);
        
            if (hr->RR==0){ //orange will always return H of 0 
            col = 6;
            }
            else {
            col = 1;
            }
        }
        
    // double verification for Y,P,W based upon H & V values
        if (col==9){
            turnonLED (1,1,1);
            __delay_ms(200);
            cr->RR = color_read_Red();
            __delay_ms(200);
            cr->GG = color_read_Green();
            __delay_ms(200);
            cr->BB = color_read_Blue();
            __delay_ms(200);
            turnonLED(0,0,0);
            findHSV(cr,hr);
            if (hr->RR<=20 && hr->RR>=13){
                //if V value less than 1000, maxValue is low hence color is black
                if (hr->BB<1000){
                col = 9;
                }
                //else set it as yellow by default if within Hue range 13<H<20
                else{
                    col = 4;
                }
            }
            //pink returns Hue less than 8
            else if (hr->RR<8){
                col = 5;
            }
            //white Hue values always above yellow (approx 24<H<30)
            else {
                col = 8;
            }
        }
    }
    LATHbits.LATH3 = 0;
    __delay_ms(200);
    *lenRead = *lenRead + 1 ;
    return col;
}



   
