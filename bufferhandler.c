/*
 * File:   bufferhandler.c
 * Author: Malin
 *
 * Created on den 1 april 2017, 20:10
 */


#include <xc.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "system.h"
#include "user.h"
#include "bufferhandler.h"
#include "uart.h"

#define LENGTH 64   //length of string received from BMS


char tmpString[70] = "";
char allDataString[70] = "";
char parsedString[17] = "";
int index = 0; //index for tmpString

int positions[11]; //variable to save all the positions of the BMS string, to be used in parsing

//append the received char to the tmpString
void appendChar(char data){
    tmpString[index] = data;
    index++;
}

//checks if the temporary string is long enough to be put into the other string
char isItLongEnough()
{
    if(index == LENGTH){
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
        allDataString[i] = tmpString[i];
    }
}

void writeResult(char dev)
{
//    writeString(parsedString, dev);
    writeString(allDataString, dev);
    wait_ms(1000);
}

char isEmpty(){
    if(tmpString[0] == '\0'){
        return 1;
    }else{
        return 0;
    }
}

char isValid(){
    if(tmpString[0] == 0x54 &&  //if first position is = T
            tmpString[1] == 0x59 && //if second position is = Y
            tmpString[2] == 0x52 && //if third position is = R
            tmpString[3] == 0x49 && //if fourth position is = I
            tmpString[4] == 0x5F){//if fifth position is = _
        return 1;
    } 
    return 0;
}


/*
 * Transforms the allDataString into a parsedString that only contains 
 * SOCxx;CHGzz;ACK;
 * 
 */
char parseString(){
    
    //find SOC
    //SOCs startposition should be at the end of the one before SOC in the string, that is position[8]
    //SOCs endposition should be at positions[9]
    int socStartPos = positions[8]+1; //+1 and -1 to get rid of the ;
    int socEndPos = positions[9]-1;
    int socSize = (socEndPos - socStartPos)+1; //get size of SOC
    //take what is at SOC and transform into a two-digit number 
    int SOC[socSize];
    //put SOC from allDataString into SOC
    int k = 0;
    for(int i = socStartPos; i<=socEndPos; i++){
        SOC[k] = allDataString[i];
        k++;
    }
    
    
    
    
    
    
            
    return 1;
}


char* createCHG(int pos){
    char *tmp = "";
    switch(pos){
        case 1 :
            *tmp = allDataString[46];
            break;
        case 2 :
            *tmp = allDataString[46] + allDataString[47];
            break;
        case 3 : 
            *tmp = allDataString[46] + allDataString[47] + allDataString[48];
            break;
    }
    //then see if tmp is a negative number
    //if negative CHG = 1
    //if positive CHG = 0
    return tmp;
}


//check the allDataString and set the positions for getting CHG and SOC 
void checkString(){
    //every string is made out of 10 modules, each separated by a ';' = 0x3B
    //every string starts with the "TYRI_0_1;"
    //therefore we start at pos=9 to check
    //so then we have the time positions from pos=9 until we reach a new ';' and so on    
    positions[0] = 8;
    
    for(int i = 0; i<10;i++){
        positions[i] = checkPos(i);
        positions[i+1] = positions[i] + 1;
    }
    
    positions[10] = '\0';
}

//check which position where the current attribute ends in the string
int checkPos(int pos){
    for(int i = pos+1; i < strlen(allDataString); i++){
        if(allDataString[i] == 0x3B){
            pos = i;
        }
    }
    return pos;
}

