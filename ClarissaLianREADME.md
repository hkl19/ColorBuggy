
# Mine Navigation Search and Rescue User Guide
Clarissa Muliawan  and Lian Han Kee.

Summary of the brief:

The buggy are supposed to imitate and search and rescue robot that can navigate a mine according to specified color flag.

The color instructions are as such:
Colour | Instruction
---------|---------
Red | Turn Right 90
Green | Turn Left 90
Blue | Turn 180
Yellow | Reverse 1 square and turn right 90
Pink | Reverse 1 square and turn left 90
Orange | Turn Right 135
Light blue | Turn Left 135 
White | Finish (return home)
Black | Maze wall colour - Also used to detect if the buggy is lost


Task | Method
---------|---------
Navigate towards color and stop before impacting card | Record the clear value of 6 most bright color and find the maximum value. Then during normal running, compare the value from the clear channel with the max value, if higher then stop
Read Card Color | Calibrate each color before running, when it reaches the find color function, compare with preset value, values are converted from RGB to HSV for a more robust system 
When final card read, go home | Memory array 
Handle exceptions | When it detects black more than twice after going backward and forward, it detects that it is lost and need to go backward


## Buggy Modes and Hardware Configuration
Upon power on, the buggy will initialise all the hardware.
In order to navigate the maze, the buggy operates in three modes, each with different triggers

Mode 1 : Calibration mode - color and motor

Mode 2 : Navigation mode - color identification and operating principle

Mode 3 : Retrace back mode - Memory



Although the general color identification algorithm would work in every devices that could read RGB. The following hardware specific points should be noted for the reproduction of the project:
1. Pins are specific to PIC18F67K40 microcontroller.
2. Color click (TCS3471) is wired to mikro BUS 2
3. A makeshift hood - made with black paper, is used to cover the color click to minimise the effect of fluctuating ambient light.



 
## Calibration Mode: Color and Motor
The buggy is designed to save the RGB value of the card at a very close distance, and measure the clear value of few brightest card to determine the threshold on when to stop.

Due to the specific nature of the flag instruction, the calibration should be done in a very specific step

Video for calibration can be seen here -> https://www.youtube.com/shorts/JPwYD0N63kM

The procedure is as below:
1. Reset the buggy before doing calibration to make sure that there are no residual memory
2. Locate the buggy in front of the red card until it is flushed to the wall 
3. Put the red card againts the maze wall
4. Press RF2 button to store the value
5. Only remove the card when the light flickers off
6. Repeat step 2-6 for the next card, the order are (Red, Greeen, Blue, Yellow, Pink, Orange, Light Blue, White, Black)
7. Move the buggy around 2-3 cm away from the wall
8. Repeat step 3 and 4 for bright card (Red, Yellow, Pink, Orange, Light Blue, and White) - in this case order does not matter

*Note: After the calibration is done, LED light beside RF7 button will turn on. To move to the next mode, press RF3
**Note: 
```
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
```
For the Motor calibration, The value of the motor are calibrated specific to the floor. To ensure consistent performance, buggy must always be in full charge , value can be observed by using batVsense function


## Color Identification and Operating Principle

In this project, all color was converted from R,G,B value to H,S,V value for a more robust set of data

Although HSV conversion requires doubles, which consumes more memory, after several trial and error of different methods using RGB, we determine that the 99% accuracy of HSV is enough to offset 
the memory used.

Note: HSV values not normalised to 256. 

The conversion formula used are :
```
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
        case 1: h = 60 *((sc->G[i]-sc->B[i])/(maxValue-minValue));      //case of R max
        break;  
        case 2 : h = 60 *(2+((sc->B[i]-sc->R[i])/(maxValue-minValue))); //case of G max
        break;
        case 3 : h = 60 *(4+((sc->R[i]-sc->G[i])/(maxValue-minValue))); //case of B max
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
```
The code was sourced and referenced to a Youtube tutorial by QuantitativeBytes 
-> https://www.youtube.com/watch?v=I8i0W8ve-JI&t=1237s

The function swaps out the mapping of RGB values into cylindrical coordinates form of HSV. Whereby h is an angle, s is a value between 0 and 1 (which determines how much dark is in the color). Utilising HSV provides an extra set of data points to verify colors more accurately. 

The conversion process is as below:
1. Compare values of R/G/B and determine which color is of maximum, this determines the range of Hue.
2. Calculate the exact hue of the colour detected with ratios of the non maximum colors.
3. Some Hue values return a negative angle integer, add 360 to make it positive. 
4. Determine saturation value by ratioing the total Hue range and V value. 
5. Store V value as the maxValue of that color dominating the Hue range. 
6. Store these HSV values into the same structure, saving memory space. 

To start operating in the maze, press RF3 button 

### Scan ambient and stop before it hits a card
This function reads the clear channel whilst the buggy it's moving, then compares the value to the maximum clear channel reading from the calibration. An example of this happening is in the video snippet here -> https://www.youtube.com/shorts/7TqEe-u75A4

During calibration, only the "bright" cards are used as min threshold. Although by logic, the white card are supposed to give out the maximum brightness during calibration, due to ambient condition and shadow, this is not the case sometimes.
Hence the maximum clear value between these bright card at around 3cm are used to increase accuracy.

If clear channel reading value higher than threshold, it will move to the next function which is reading the color
```
void scanColor (struct getcolor *clear, struct storecolor *sc, struct motor *mL, struct motor *mR,unsigned int *fwdTime) {

    while(LATHbits.LATH3 == 0){
        turnonLED (1,1,1); //turn LED to make it similar condition as during calibration

        clear->CC = color_read_Clear();
        
        unsigned int C_max = 0;
        static char i;
    
        for(int i = 9; i < 15; i++){ //find maximum clear during calibration
            if(C_max < sc->C[i])
                C_max = sc->C[i];
        }
            
        if(clear->CC >= C_max){ //if clear channel is brighter than the calibration value
            stop(mL,mR);
            __delay_ms(500);
            LATHbits.LATH3 = 1; //toggle led on to trigger next function
            adjustforward(mL,mR); // adjust forward so read the color close to the wall
        }
        
        else {
            LATHbits.LATH3 = 0;
            forward(mL,mR);
            *fwdTime = *fwdTime +1 ; //timer to measure how many times it moves forward and store it in memory
        }
    }
}
```
### Reading color
 When scanColor register a card, it will turn on LATH3, which in turn will trigger findColor(). 

 The findColor algorithm is as such:
1. First it will register the RGB value of the card in front of it.
2. These values will be then converted to HSV value.
3. Individual HSV value will be compared with the calibrated HSV value, with initial tolerance of plus minus 1. 
4. If color is not indentified, the tolerance will kept be added by one until it finds a color.
5. As red and orange ; and yellow, pink white gives very similar value, double verification system is used- 
individual RGB backlit will be flashed, then HSV values are compared with preset value.
6. Int value correspoinding to each color will be returned, LATH3 turned off to trigger the scanColor()
\
Note: findColor() function is located in color.c file

## Retrace Back Memory
There are two features of the return back of the buggy : Memory array and the reverse function.
A depiction of the retrace mode is seen here -> https://www.youtube.com/shorts/JE6otdNNerA

The main flow of the retrace and memory is as such:
1. Individually fill up elements within the color and time memory array.
2. Actively count the total memory length used up within the extended array. 
3. Count backwards according to the length counted in (2).
4. Perform color actions and forwards motion based upon these array values. 
5. Realign buggy via using the bounding walls upon completing a color action. 


### Memory Array
When the buggy is navigating through the maze in mode 2, it is storing the time it moves forward and the color it's reading in an array.
 
 fwdArr[25] for time
 actArr[25] for color
 
```
actionArray(char lenArr, int *actArr, char colorScan){
        static char i = 0;
        if (i < lenArr){
            *(actArr+i) = colorScan;
            i ++;
        }
}
```
To measure time, the buggy measures the amount of times the scanColor function is
called before it breaks the loop and go to findColor. It is done by updating the pointer fwdTime.

*Note: the size of the array could be changed, 25 is an overestimation of the number of flags it will read.

Every time a color is added, the lenRead pointer adds one to count the total number of cards scanned.

When white is detected, it will trigger the buggy to move to the retrace mode, and flips the reverse value to 1

### Start Retracing back

backToStart() function first reads the fwdArr[] starting from the lenRead-1, whilst reading the actArr[] starting 
from lenRead - as white color are supposed to be ignored now.
```
backToStart(char *lenRead, unsigned int *actArr, unsigned int *fwdTime, unsigned int *fwdArr, char reverse, struct motor *mL, struct motor *mR){
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
```
Note: the last element of the actionArray is white, hence, it's not called to the colorCase function. 

### Reverse motor function

When the reverse pointers is equal to run, each color code will run in opposite sequence and direction as it first started

Example :
```
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
```
## Troubleshooting and future improvement
These are the most common occuring problem experienced by the buggy:

1. Maze with more than 25 card - increase the size of fwdArr[25] and
 actArr[25] 

2. Backlight does not turn off during calibration - module are stuck in i2C idle, restart the buggy

3. Buggy does not move in accurate direction - due to the nature of the buggy, it is almost impossible 
to calibrate buggy into precise angle and make it work that way. Make sure that buggy is fully charge, else adjust the power of the motor.

4. Buggy reads ambient light as white color - redo the calibration with bright cards closer to the buggy.
Note: During calibration, ambient light are affected by shadows, hence, during the actual reading, it might stop further away than as calibrated. 
Estimate the distance from the condition of ambient light in the maze, if ambient light is very bright, the calibrate closer to the buggy and vice versa.

Future improvement includes using interrupt to read the clear channel for more complex buggy task.


## Workflow management 

1. Han works within the main branch, Clarissa works within her seperate branch.
2. Any changes validated are pushed to each individual branches. 
3. A backup folder (either txt or md format) is stored within a shared onedrive. 
4. Code commited by Clarissa will be merged with the main branch once completed. 
5. Split tasks to prevent merging errors (Clarissa-> Color Identification, Han-> Motor and Arrays).
6. Always define changes within commits, only push if code is back-tested. 
