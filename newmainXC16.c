/*
 * File:   newmainXC16.c
 * Author: jaron
 *
 * Created on October 29, 2018, 5:52 PM
 */


#include "xc.h"
#include "stdio.h"
#pragma config FNOSC = FRCDIV
int counter = 0;

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
                OC1R = (int)(.5 * OC1RS);
                smartDelay(.5);
            }
            OC1RS = goal_ocrs;
            OC1R = (int)(.5 * OC1RS);
        }
        else{
            while((OC1RS - goal_ocrs) > (rate - 1)){
                OC1RS -= rate;
                OC1R = (int)(.5 * OC1RS);
                smartDelay(.5);
            }
            OC1RS = goal_ocrs;
            OC1R = (int)(.5 * OC1RS);
        }
    }
    else if(oc_switch == 2){
        int rate =  OC2RS/4;// ramp rate in OC1R per .5 seconds
        if(goal_ocrs > OC2RS){
            while((goal_ocrs - OC2RS) > (rate - 1)){
                OC2RS += rate;
                OC2R = (int)(.5 * OC2RS);
                smartDelay(.5);
            }
            OC2RS = goal_ocrs;
            OC2R = (int)(.5 * OC2RS);
        }
        else{
            while((OC2RS - goal_ocrs) > (rate - 1)){
                OC2RS -= rate;
                OC2R = (int)(.5 * OC2RS);
                smartDelay(.5);
            }
            OC2RS = goal_ocrs;
            OC2R = (int)(.5 * OC2RS);
        }
    }
    else{
        int rate =  OC3RS/4;// ramp rate in OC1R per .5 seconds
        if(goal_ocrs > OC3RS){
            while((goal_ocrs - OC3RS) > (rate - 1)){
                OC3RS += rate;
                OC3R = (int)(.5 * OC3RS);
                smartDelay(.5);
            }
            OC3RS = goal_ocrs;
            OC3R = (int)(.5 * OC3RS);
        }
        else{
            while((OC3RS - goal_ocrs) > (rate - 1)){
                OC3RS -= rate;
                OC3R = (int)(.5 * OC3RS);
                smartDelay(.5);
            }
            OC3RS = goal_ocrs;
            OC3R = (int)(.5 * OC3RS);
        }
    }
}

//Milestone 9 - Scoring Code
int main(void) {
    //Basic structure: Detect ball in slot, slide into turret, shoot ball after time delay
    //Needs: PWM signal for flow valve, color diode for detecting ball, solenoid setup
    //Assumptions: fixed sorter tube in correct position
    
    //1:64 Postscaler for servo
    _RCDIV = 0b110;

    OC3CON1 = 0;
    OC3CON2 = 0;


    // Set period and duty cycle
    OC3R = 94;
 
    OC3RS = 1250;

    // Configure OC1 & OC2
    OC3CON1bits.OCTSEL = 0b111;
    OC3CON2bits.SYNCSEL = 0x1F;

    OC3CON2bits.OCTRIG = 0;
    OC3CON1bits.OCM = 0b110;
    
    //Sort read set up
    _TRISA1 = 1;
    _ANSA1 = 0;

    //A/D Converter for pototransister
    /*** Select Voltage Reference Source ***/
	// use AVdd for positive reference
	_PVCFG = 0b00;		// AD1CON2<15:14>, pg. 212-213 datasheet
	// use AVss for negative reference
	_NVCFG = 0;			// AD1CON2<13>


	/*** Select Analog Conversion Clock Rate ***/
	// make sure Tad is at least 600ns, see Table 29-41 datasheet
	_ADCS = 0;	// AD1CON3<7:0>, pg. 213 datasheet


	/*** Select Sample/Conversion Sequence ***/
	// use auto-convert
	_SSRC = 0b0111;		// AD1CON1<7:4>, pg. 211 datasheet
	// use auto-sample
	_ASAM = 1;			// AD1CON1<2>
	// choose a sample time >= 1 Tad, see Table 29-41 datasheet
	_SAMC = 0b00001;		// AD1CON3<12:8>


	/*** Choose Analog Channels to be Used ***/
	// scan inputs
	_CSCNA = 1;			// AD1CON2<10>
	// choose which channels to scan, e.g. for ch AN12, set _CSS12 = 1;
	_CSS0 = 1;			// AD1CSSH/L, pg. 217
   
	/*** Select How Results are Presented in Buffer ***/
	// set 12-bit resolution
	_MODE12 = 1;		// AD1CON1<10>
	// use absolute decimal format
	_FORM = 0b00;			// AD1CON1<9:8>
	// load results into buffer determined by converted channel, e.g. ch AN12 
    // results appear in ADC1BUF12
	_BUFREGEN = 1;		// AD1CON2<11>


	/*** Select Interrupt Rate ***/
	// interrupt rate should reflect number of analog channels used, e.g. if 
    // 5 channels, interrupt every 5th sample
	_SMPI = 0b00010;		// AD1CON2<6:2>


	/*** Turn on A/D Module ***/
	_ADON = 1;			// AD1CON1<15
    
    _TRISA6 = 0;
    _LATA6 = 0;
    _TRISA1 = 0;
    _LATA1 = 0;
    _ANSA1 = 0;
    
    int x;
    int sort_load = 50; //top of sorter is open
    int sort_eject = 110; // eject to turret
    
    while(1){        
        x = (int)ADC1BUF0;
        OC3R = sort_load;
        _LATA1 = 0;
        
        if(x > 500){//4096 is max; I'm trying to tune this to detect a ball
            smartDelay(1.0);
            _LATA6 = 1;
            OC3R = sort_eject;
            smartDelay(1.0);
            _LATA6 = 0;
            OC3R = sort_load;
            smartDelay(1.0);
            _LATA1 = 1;
            smartDelay(1.0);
        }
        
        
    }
    
    return 0;
}