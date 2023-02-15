#include <xc.h>
#include "serial.h"

#define _XTAL_FREQ 64000000  


//code to set up USART4 for Reception and Transmission 
void initUSART4(void) {
    TRISCbits.TRISC1=1;
    TRISCbits.TRISC0=1;
    RC0PPS = 0x12; // Map EUSART4 TX to RC0
    RX4PPS = 0x11; // RX is RC1   
    
    BAUD4CONbits.BRG16 = 0; 	//set baud rate scaling - 8 BIT
    TX4STAbits.BRGH = 0; 		//high baud rate select bit
    
    // Baud rate 9600bps    L =103, H = 0
    // Baud rate 19200      L = 51, H = 0
    SP4BRGL = 51; 			//set baud rate to 51 = 9600bps (103.17) - formula == Fosc/[64(n+1)] n = value of SPxBRGH : SPsBRGL
    SP4BRGH = 0;			//not used

    RC4STAbits.CREN = 1; 		//enable continuous reception
    TX4STAbits.TXEN = 1; 		//enable transmitter
    RC4STAbits.SPEN = 1; 		//enable serial port
    
}


//function to check the TX reg is free and send a byte
void sendCharSerial4(char charToSend) {
    while (!PIR4bits.TX4IF); // wait for flag to be set
    TX4REG = charToSend; //transfer char to transmitter
}


//function to send a string over the serial interface
void sendStringSerial4(char *string){
    while (*string!=0){
        sendCharSerial4(*string++);
    }
}
