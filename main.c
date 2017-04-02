/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>        /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>       /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>   /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */
#include <string.h>

#endif

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp */
#include "uart.h"


/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

/* i.e. uint8_t <variable_name>; */

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/



void main(void)
{
    /////////////
    //SERIAL YO//
    /////////////
    /* Configure the oscillator for the device */
    ConfigureOscillator();

    /* Initialize I/O and Peripherals for application */
    InitApp();
    
    UART_Init(19200);
    writeString("L 50", 1);
    UART_Write(0x0D,1);
    writeString("D 8", 1);
    UART_Write(0x0D,1);
    
    uint8_t var = 0;
    char result;
    
    //set lamps as output
    TRISAbits.TRISA0 = 0; //set pin RA0 (LED) as output
    TRISAbits.TRISA1 = 0; //set pin RA1 (LED) as output
    LATAbits.LATA0 = 0;
    LATAbits.LATA1 = 0;
    TRISDbits.TRISD3 = 1; //set button1 as input
    
    clearBuffer();
    
    while(1){
        
        
       if(isItLongEnough()){
           if(isValid()){
               LATAbits.LATA1 = 1;
                wait_ms(1000);
                copyTmpBuffer(); //copies tmpBuffer to the real string and empties the tmpBuffer
                clearBuffer();
           }else{
               clearBuffer();
           }
       }else{
           LATAbits.LATA1 = 0;
       }
        
        if(!PORTDbits.RD3){
            writeResult(2);
        }
        
        
    }
    
    
    
    

    //skriv till TXreg -> TSR
   // UART_Write(0xFF);
   // UART_Write(0xAA);
    
   /* for(uint8_t i = 0; i<=10; i++){    
        char yo = '3'+i;
        UART_Write(yo);
    }*/
    
    
    //TODO, do this method in uart.c
    //read the data from RXreg
   // if(UART_Data_Ready){
    //    result = UART_Read();
//      var = result - 48;
        
    //}
    

    
    
    /*if(var > 200){
        LATAbits.LATA1 = 1;
        wait_ms(500);
    }*/
    
    
    
    
    //blinky blink
    //for(uint8_t i = 0; i<var; i++){
      //  LATAbits.LATA0 = 1;
       // wait_ms(500);
       // LATAbits.LATA0 = 0;
        //wait_ms(500);
    //}
    

}

