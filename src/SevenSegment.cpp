#include "SevenSegment.h"

SevenSegment::SevenSegment(PinName ser, PinName srclk, PinName rclk) : DSERIAL(ser), RCLK(rclk), SRCLK(srclk) { }


SevenSegment::~SevenSegment(){}

bool SevenSegment::displayInt(const int num){
    return displayFourNums(num);
}
bool SevenSegment::displayDecimal(const float num){
    return false;                           // <-------- add code for this
}

void SevenSegment::displayOneNum(const int num){
    writeArray(segment[num]);
    return;
}

bool SevenSegment::displayFourNums(const int num){
    if(num >= 0){
        int digi1 = 0;
        int digi2 = 0;
        int digi3 = 0;
        int digi4 = 0;
        digi4 = (num < 1000) ? BLANK : (num/1000);
        //digi3 = (num < 100) ? BLANK : ((num-(num/1000)*1000)/100);
        digi3 = (num < 100) ? BLANK : (num%1000)/100;
        //digi2 = (num < 10) ? BLANK : (num-(((num-(num/1000)*1000)/100)*100-(num/1000)*1000))/10;
        digi2 = (num < 10) ? BLANK : (num%100)/10;
        digi1 = num%10;
        displayOneNum(digi1);
        displayOneNum(digi2);
        writeBit2(digi3);           // <-change to "displayOneNum" for fixed design
        displayOneNum(digi4);
        return true;
    }
    else if(num < 0 && num > (-1000)) {
        int digi1 = (num%10);
        int digi2 = (num > -10) ? NEG : (num > -10) ? BLANK : (num%100)/10;
        int digi3 = (num > -100 && digi2 != NEG) ? NEG : (num > -100) ? BLANK : (num%1000)/100;
        int digi4 = (num > -1000 && digi3 != NEG && digi2 != NEG) ? NEG : BLANK;
        //digi3 = (num < 100) ? BLANK : ((num-(num/1000)*1000)/100);
        //digi2 = (num < 10) ? BLANK : (num-(((num-(num/1000)*1000)/100)*100-(num/1000)*1000))/10;
        displayOneNum(abs(digi1));
        displayOneNum(abs(digi2));
        writeBit2(abs(digi3));           // <-change to "displayOneNum" for fixed design
        displayOneNum(digi4);
        return true;
    }
    else{
        return false;
    }
}




bool SevenSegment::writeBit2(const int num){
    char convert[8] = {segment[num][0], segment[num][1], segment[num][6], segment[num][3], 
    segment[num][4], segment[num][5], segment[num][2], segment[num][7]};
    writeArray(convert);
    return true;
}


bool SevenSegment::displayTwoNums(const int num){
    int digi1 = 0;
    int digi2 = 0;
    if((num/10) > 1){
        digi2 = num/10;
    }
    digi1 = num - (digi2*10);
    displayOneNum(digi1);
    displayOneNum(digi2);
    return true;
}

bool SevenSegment::clear() {
    for (int i = 0; i < 33; i++){
       writeBit(0);
    }
    return true;
}
void SevenSegment::on(){
    writeBit(1);
}
void SevenSegment::off(){
    writeBit(0);
}


//Pulses the register
void SevenSegment::updateRegister(){
    SRCLK = 1;
    //wait_us(.000001);
    SRCLK = 0;    
}
//Updates the output register
void SevenSegment::updateOutput(){
    RCLK = 1;
    //wait_us(.000001);
    RCLK = 0;    
}
//Writes a bit to the shift register
void SevenSegment::writeBit(unsigned char bit){
    DSERIAL = bit & 0x01;
    updateRegister();
    updateOutput();
    } 

void SevenSegment::writeArray(char arr[8*REGISTER_CNT]){
    for(int i = (8*REGISTER_CNT)-1; i >= 0; i--) {
        writeBit(arr[i]);
    }       
    updateRegister();
}