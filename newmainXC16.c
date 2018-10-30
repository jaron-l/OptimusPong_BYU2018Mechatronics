/*
 * File:   newmainXC16.c
 * Author: jaron
 *
 * Created on October 29, 2018, 5:52 PM
 */


#include "xc.h
#pragma config FNOSC = FRC

int main(void) {
  //Set Pin 2 & 3 for Dicection Output
 _ANSA0 = 0;
 _TRISA0 = 0;
 _ANSA1 = 0;
 _TRISA1 = 0;


 //-----------------------------------------------------------
 // CONFIGURE PWM1 USING OC1 & OC2 (on pin 14 & 4 respectively)

 // Clear control bits initially
 OC2CON1 = 0;
 OC2CON2 = 0;


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
         _LATA1 = 1;
         _LATA0 = 1;
     }

     //45 degree turn - Depends on Wheel size (100*D/d)
     //D-diameter from center of wheel to center of wheel
     //d-diameter of wheel
     while(counter >= 800 && counter < ???){
         _LATA1 = 0;
         _LATA0 = 1;
     }

     //Drive forward again
     while(counter >= ??? && counter < 2000){
         _LATA1 = 1;
         _LATA0 = 1;
     }

     //Stop
     if(counter>2000){
         OC2R = 0;
         OC1R = 0;
     }

    return 0;
}
