/*
 * File:   newmainXC16.c
 * Author: jaron
 *
 * Created on October 29, 2018, 5:52 PM
 */


#include "xc.h"
#define FCY 4000000UL
#include <libpic30.h>
#pragma config FNOSC = FRC
#include "default_setup.h"
#include "Nav_func.h"
#include "shooting.h" 

#define LeftIR ADC1BUF0
#define MidIR ADC1BUF1
#define RightIR ADC1BUF4

void _ISR _OC1Interrupt(void){
    _OC1IF = 0;
    wheel_counter++;
}

void __attribute__((interrupt,no_auto_psv)) _CNInterrupt(void){
    _CNIF = 0;
    if(_RB7 == 1 && _RB8 == 1){
        hit_corner_flag = 1;
    }
}

//Milestone 10 - Controlled Navigation
int main(void) {
    //Demonstrate controlled 
    //navigation of your robot. Show that if 
    //the robot is placed in the dispensing 
    //area of the arena, it can navigate to a 
    //location appropriate for your scoring method 
    //and then turn toward an active goal. Submit a 
    //video of the navigation via Learning Suite. For 
    //full points, this should be accomplished under battery power.
    
     /**
     * current plan: drive forward (pray you're not perpendicular to a wall),
     * when both push buttons are activated, reverse to center,
     * drive a full 360
     * when any IR sensor goes high (indicating dispensor), 
     * turn towards the opposite goal to the dispensor
      */
    
    //init();
    OC1CON1 = 0;
    OC1CON2 = 0;
    OC1CON1bits.OCTSEL = 0b111;
    OC1CON2bits.SYNCSEL = 0x1F;
    OC1CON2bits.OCTRIG = 0;
    OC1CON1bits.OCM = 0b110;
    
    _TRISB12 = 0; //Left Motor direction
    _ANSB12 = 0;
    _TRISB2 = 0; //Right Motor direction
    _ANSB2 = 0;
    
    _TRISA0 = 1;
    _ANSA0 = 1;
    _TRISA1 = 1;
    _ANSA1 = 1;
    _TRISA2 = 1;
    _ANSA2 = 1;
    
    //A/D Converter for phototransister
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
	_CSS0 = 1;	//pin2 IR sensor left		// AD1CSSH/L, pg. 217
    //_CSS1 = 1;  //pin3 IR sensor middle
    //_CSS4 = 1; //pin 7 IR sensor right
    //_CSS14 = 1; //pin 8 Vertical load sensor
    
   
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
	_SMPI = 0b00011;		// AD1CON2<6:2>


	/*** Turn on A/D Module ***/
	_ADON = 1;			// AD1CON1<15
    
    OC1CON1 = 0;
    OC1CON2 = 0;
    OC1CON1bits.OCTSEL = 0b111;
    OC1CON2bits.SYNCSEL = 0x1F;
    OC1CON2bits.OCTRIG = 0;
    OC1CON1bits.OCM = 0b110;
    
    /**
    char black_gate = detect_black_gate();
    
    vertical_sort(black_gate);
     */
    int abc = 1;
    
    while(1){
        
        abc = 0;

        OC1RS = 7999;
        OC1R = 4000;
        
        _LATB12 = 1;
        _LATB2 = 0;
        
        __delay_ms(1300);
        
        OC1R = 0;
        __delay_ms(250);
        
        _LATB12 = 0;
        _LATB2 = 0;
        OC1R = 4000;
        
        if(LeftIR > LeftIRThreshold){
            __delay_ms(530);
        }
        
        OC1R = 0;
        __delay_ms(5000);
    }
    return 0;
}