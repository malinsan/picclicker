/*
 * File:   bufferhandler.c
 * Author: Malin
 *
 * Created on den 1 april 2017, 20:10
 */


#include <xc.h>
#include <string.h>
#include "system.h"
#include "bufferhandler.h"
#include "uart.h"

#define LENGTH 64   //length of string received from BMS

char tmpString[70] = "";
char realString[70] = "";

int index = 0; //index for tmpString


void appendChar(char data){
    tmpString[index] = data;
    index++;
}

//checks if the temporary string is long enough to be put into the other string
char isItLongEnough()
{
    return index == LENGTH;
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
    return tmpString[0] == '\0';
}

char isValid(){
    return tmpString[0] == 0x54 &&  //if first position is = T
            tmpString[1] == 0x59 && //if second position is = Y
            tmpString[2] == 0x52 && //if third position is = R
            tmpString[3] == 0x49 && //if fourth position is = I
            tmpString[4] == 0x5F; //if fifth position is = _
}

char parseTmpString(){
    return 0;
}

