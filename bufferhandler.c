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
int SOC[3] = 0; //needed to hold the SOC int
int toCharArray[3] = 0; //perhaps not needed
char charSOC[3] = ""; //needed to turn the int into an array of chars so i can print it
    
char parseString(){
    
    //find SOC
    //SOCs startposition should be at the end of the one before SOC in the string, that is position[8]
    //SOCs endposition should be at positions[9]
    int socStartPos = positions[8]+1; //+1 and -1 to get rid of the ;
    int socEndPos = positions[9]-1;
    int socSize = (socEndPos - socStartPos)+1; //get size of SOC
    //take what is at SOC and transform into a two-digit number 
    //put SOC from allDataString into SOC
    int k = 0;
    for(int i = socStartPos; i<=socEndPos; i++){
        SOC[k] = allDataString[i];
        k++;
    }
    //transform the int into a char
    int tmp = transformToTwoDigitNumber(SOC, socSize);
    
    //put the int into an array of ints and chars
    int count = lenHelper(tmp) - 1;
    while(tmp > 0){
        toCharArray[count] = tmp%10;
        charSOC[count] = (tmp%10)+48;
        tmp /= 10;
        count --;
    }
    
    //find CHG: if <0 = 1 if >0 = 0
    //CHG should start at positions[6] and end at positions[7]
    int chgStart = positions[6] + 1;
    int chgStop = positions[7] - 1;
    
    //guessing that we only have to check for a '-' = 0x2D
    char CHG = '0';
    //if the first letter/number is a '-'
    if(allDataString[chgStart] == 0x2D){
        CHG = '1';
    }
    
    //now we have SOC and CHG
    //since strcpy(array, sentence) doesn't work and i can't seem to find any other way around this...
    //lets fill it the hard way then!
    int parsedStringSize = 13 + count;
    int n = 4;
    
    parsedString[0] = 'S';
    parsedString[1] = 'O';
    parsedString[2] = 'C';
    parsedString[3] = charSOC[0];
    if(count > 0){
        parsedString[4] = charSOC[1];
        n ++; //n becomes 5
    }
    parsedString[n] = ";";
    parsedString[n+1] = "C";
    parsedString[n+2] = "H";
    parsedString[n+3] = "G";
    parsedString[n+4] = CHG;
    parsedString[n+5] = ";";
    parsedString[n+6] = "A";
    parsedString[n+7] = "C";
    parsedString[n+8] = "K";
    parsedString[n+9] = ";";
    
    
      
    return 1;
}


//simple function to get the length of an int
int lenHelper(int x){
    if(x>=1000) return 4;
    if(x>=100) return 3;
    if(x>=10) return 2;
    return 1;
}

//transforms an array of ints into a two digit number, up to a size of 3
//e.g 793 => 79
//e.g 79 => 8
//e.g 7 => 1
int transformToTwoDigitNumber(int number[], int size){
    
    int result = 0;
    switch(size){
        case 1: //when size is 1, the number is a decimal between 0.0 - 0.9
            if(number[0] >= 5){
                return 1;
            }else{
                return 0;
            }
        case 2: //when size is 2, the number is a number between 10-99
            result = number[0];
            if(number[1] >= 5){
                return result++;
            }else{
                return result;
            }
        case 3: 
            result = (number[0]*10)+(number[1]);
            if(number[2] >= 5){
                 result++;
                 return result;
            }else{
                return result;
            }           
    }
    
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
            break;
        }
    }
    return pos;
}

