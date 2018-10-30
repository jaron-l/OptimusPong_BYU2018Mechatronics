/*
 * File:   newmainXC16.c
 * Author: jaron
 *
 * Created on October 29, 2018, 5:52 PM
 */


#include "xc.h
#pragma config FNOSC = FRC
int counter = 0;

void _ISR _OC1Interrupt(void){
    _OC1IF = 0;
    counter++;
}

void delay(float time){//Assumes an 8 MHz clock, time is in seconds
  int beats = 0;
  while(beats<time*4000){//multiply time by Hz/2 of clock
    beats++;
  }
}

//Experimental clock query to make time independant of clock source
void smartDelay(float time){ //time in seconds
  int currClock = COSC;
  int beats = 0;
  if(currClock == 0b000){//8MHz clock
    while(beats<time*4000){//for each clock type, simply multiply time by Hz/2
      beats++;
    }
  }
  if(currClock == 0b111){//8MHz clock with 1:64 divider
    while(beats<time*62.5){//for each clock type, simply multiply time by Hz/2
      beats++;
    }
  }
}

int main(void) {
  _TRISA6 = 0; //Direction for Wheel 1
  _ANSA6 = 0;
  _TRISB2 = 0; //Direction for Wheel 2
  _ANSB2 = 0;

  counter = 0;

 //-----------------------------------------------------------
 // CONFIGURE PWM1 USING OC1 & OC2 (on pin 14 & 4 respectively)

 // Clear control bits initially
 OC2CON1 = 0;
 OC2CON2 = 0;

 OC1CON1 = 0;
 OC1CON2 = 0;


 // Set period and duty cycle
 OC1R = 16000;
 OC2R = 16000;

 OC1RS = 31999;
 OC2RS = 31999;

 // Configure OC1 & OC2
 OC1CON1bits.OCTSEL = 0b111;
 OC1CON2bits.SYNCSEL = 0x1F;

 OC2CON1bits.OCTSEL = 0b111;
 OC2CON2bits.SYNCSEL = 0x1F;

 OC1CON2bits.OCTRIG = 0;
 OC1CON1bits.OCM = 0b110;
 OC2CON2bits.OCTRIG = 0;
 OC2CON1bits.OCM = 0b110;

 _OC1IE = 1;
 _OC2IE = 1;

 while(1){

     //full turn (400)
     while(counter >= 0 && counter < 800){
         _LATA6 = 1; //Assuming 1 is forward
         _LATB2 = 1;
     }

     OC2R = 0;
     OC1R = 0;
     delay(1.0);
     OC2R = 1;
     OC1R = 1;

     //45 degree turn - Depends on Wheel size (100*D/d)
     //D-diameter from center of wheel to center of wheel
     //d-diameter of wheel
     //Will be less because both wheels?
     while(counter >= 800 && counter < ???){
         _LATA6 = 0;
         _LATB2 = 1;
     }

     OC2R = 0;
     OC1R = 0;
     delay(1.0);
     OC2R = 1;
     OC1R = 1;

     //Drive forward again
     while(counter >= ??? && counter < 2000){
         _LATA6 = 1;
         _LATB2 = 1;
     }

     //Stop
     if(counter>2000){
         OC2R = 0;
         OC1R = 0;
     }
    return 0;
}
