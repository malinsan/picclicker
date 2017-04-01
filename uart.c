/******************************************************************************/
/*Files to Include                                                            */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>    /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#endif

#include "system.h"
#include "uart.h"

#define TRMTStatus TXSTAbits.TRMT



char tmpString[50] = "";
char realString[50] = "";

int index = 0; //index for tmpString

char UART_Init(const long int baudrate)
{
  unsigned int x;
  
  x = (_XTAL_FREQ/baudrate)/16 -1;
  //EUSART1
  
  TXSTA1 = 0b00100100;
  RCSTA1 = 0b10010000;
  RC1IE = 1;     //Activate interrupt on receiving shit
  BAUDCON1bits.BRG16 = 0; //8-bit baudrate generator
 // TXSTA1bits.BRGH = 1;
  SPBRG1 = 155;             //Writing SPBRG Register
  SPBRGH1 = 155;
  
  // TXSTA1bits.SYNC = 0;                                     //Setting Asynchronous Mode, ie UART
 // RCSTA1bits.SPEN = 1;                                     //Enables Serial Port
  TRISC7 = 1;                                   //As Prescribed in Datasheet
  TRISC6 = 0;                                   //As Prescribed in Datasheet
 // RCSTA1bits.CREN = 1;                                     //Enables Continuous Reception
 // TXSTA1bits.TXEN = 1;                                     //Enables Transmissi
 
  
  //EUSART2
  RCSTA2 = 0b10010000;
  TXSTA2 = 0b00100100;
  
  RC2IE = 1; //enable interrupt
  BAUDCON2bits.BRG16 = 0; //8-bit baudrate generator
  SPBRG2 = 25;
  SPBRGH2 = 25;
  TRISB4 = 1;
  TRISB5 = 0;
  return 1;                                     //Returns 1 to indicate Successful Completion

 
  //return 0;                                       //Returns 0 to indicate UART initialization failed
}


void UART_Write(char data, char dev)
{
  if (dev == 1)
  {
    while(!TXSTA1bits.TRMT); //wait for the data to be ready for transmission
    TXREG1 = data;
  }
  else
  {
    while(!TXSTA2bits.TRMT); //wait for the data to be ready for transmission
    TXREG2 = data;   
  }
}

char trmtStatus()
{
  return TXSTA1bits.TRMT;
}

//is data ready to be read from the RCSTA 
char UART_Data_Ready()
{
  return RCIF;
}

char UART_Read(char dev)
{
    if(dev == 1){
        while(!RC1IF);
        return RCREG1;
    }else{
        while(!RC2IF);
        return RCREG2;
    }
}

void writeString(char str[], char dev)
{
    for(int i = 0; i<strlen(str); i++)
    {
        UART_Write(str[i], dev);
    }
}

void appendChar(char data){
    tmpString[index] = data;
    index++;
    
}

//checks if the temporary string is long enough to be put into the other string
char isItLongEnough(){
    if(index == 3) //(sizeof(yoString) / sizeof(&yoString[0])) == 3
    {
        return 1;
    }else{
        return 0;
    }
}

void clearBuffer() {
    for(int i = 0; i<strlen(tmpString); i++){
        tmpString[i] = "";   
    }
    tmpString[0] = '\0';
    index = 0;
}

void copyTmpBuffer(){
    for(int i = 0; i<index; i++){
        realString[i] = tmpString[i];
    }
}

void writeResult(char dev)
{
    writeString(realString, dev);
    wait_ms(1000);
}

char isEmpty(){
    if(tmpString[0] == '\0' ){
        return 1;
        
    }else{
        return 0;
    }
}
