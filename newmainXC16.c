/*
 * File:   newmainXC16.c
 * Author: jaron
 *
 * Created on October 29, 2018, 5:52 PM
 */


#include "xc.h"
#pragma config FNOSC = FRC
int counter = 0;

void _ISR _OC1Interrupt(void){
    _OC1IF = 0;
    counter++;
}

//Clock query to make time independant of clock source
void smartDelay(float time){ //time in seconds
  int currClock = _COSC;
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

//Function that will ramp to a duty cycle
//runs but I realized that the OCxR doesn't control how fast it is going. What controls that is OCxRS
void rampPWM(int oc_switch, int goal_ocrs){
    if(oc_switch == 1){
        int rate =  OC1RS/4;// ramp rate in OC1R per .5 seconds
        if(goal_ocrs > OC1RS){
            while((goal_ocrs - OC1RS) > (rate - 1)){
                OC1RS += rate;
                OC1R = int(.5 * OC1RS);
                smartDelay(.5);
            }
            OC1RS = goal_ocrs;
            OC1R = int(.5 * OC1RS);
        }
        else{
            while((OC1RS - goal_ocrs) > (rate - 1)){
                OC1RS -= rate;
                OC1R = int(.5 * OC1RS);
                smartDelay(.5);
            }
            OC1RS = goal_ocrs;
            OC1R = int(.5 * OC1RS);
        }
    }
    else if(oc_switch == 2){
        int rate =  OC2RS/4;// ramp rate in OC1R per .5 seconds
        if(goal_ocrs > OC2RS){
            while((goal_ocrs - OC2RS) > (rate - 1)){
                OC2RS += rate;
                OC2R = int(.5 * OC2RS);
                smartDelay(.5);
            }
            OC2RS = goal_ocrs;
            OC2R = int(.5 * OC2RS);
        }
        else{
            while((OC2RS - goal_ocrs) > (rate - 1)){
                OC2RS -= rate;
                OC2R = int(.5 * OC2RS);
                smartDelay(.5);
            }
            OC2RS = goal_ocrs;
            OC2R = int(.5 * OC2RS);
        }
    }
    else{
        int rate =  OC3RS/4;// ramp rate in OC1R per .5 seconds
        if(goal_ocrs > OC3RS){
            while((goal_ocrs - OC3RS) > (rate - 1)){
                OC3RS += rate;
                OC3R = int(.5 * OC3RS);
                smartDelay(.5);
            }
            OC3RS = goal_ocrs;
            OC3R = int(.5 * OC3RS);
        }
        else{
            while((OC3RS - goal_ocrs) > (rate - 1)){
                OC3RS -= rate;
                OC3R = int(.5 * OC3RS);
                smartDelay(.5);
            }
            OC3RS = goal_ocrs;
            OC3R = int(.5 * OC3RS);
        }
    }
}

int main(void) {
  _TRISB12 = 0; //Direction for Wheel 1 with no analog definition

  counter = 0;

 //-----------------------------------------------------------
 // CONFIGURE PWM1 USING OC1 & OC2 (on pin 14 & 4 respectively)

 // Clear control bits initially

 OC1CON1 = 0;
 OC1CON2 = 0;


 // Set period and duty cycle
 OC1R = 0;

 OC1RS = 7999;

 // Configure OC1 & OC2
 OC1CON1bits.OCTSEL = 0b111;
 OC1CON2bits.SYNCSEL = 0x1F;

 OC1CON2bits.OCTRIG = 0;
 OC1CON1bits.OCM = 0b110;

 _OC1IE = 1;

 while(1){
     counter = 0;
     _LATB12 = 1; //Forward
     rampPWM(1,7999);
     
     while(counter >= 0 && counter < 800);

     rampPWM(1,0);
     smartDelay(1);
     _LATB12 = 0;
     rampPWM(1,7999);

     //45 degree turn - Depends on Wheel size (100*D/d)
     //D-diameter from center of wheel to center of wheel
     //d-diameter of wheel
     //Will be less because both wheels?
     while(counter >= 800 && counter < 1100);

     rampPWM(1,0);
     smartDelay(1);
     _LATB12 = 1;
     rampPWM(1,7999);

     //Drive forward again
     while(counter >= 1100 && counter < 1900);

     //Stop
    rampPWM(1,0);
    
    smartDelay(5.0);
    
}
 return 0;
}