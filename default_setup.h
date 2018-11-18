/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

#include <xc.h> // include processor files - each processor file is guarded. 
#define FCY 4000000UL
#include <libpic30.h>

//Function that will ramp to a duty cycle

void set_up_PWM_and_ADC(void){
    //1:64 Postscaler for servo
    
    OC1CON1 = 0;
    OC1CON2 = 0;
    
    OC2CON1 = 0;
    OC2CON2 = 0;
    
    OC3CON1 = 0;
    OC3CON2 = 0;


    // Set period and duty cycle
    OC1R = 0;
    OC1RS = 0;
    OC2R = 50;
    OC2RS = 1250;
    OC3R = 50;
    OC3RS = 1250;
    
    OC1CON1bits.OCTSEL = 0b111;
    OC1CON2bits.SYNCSEL = 0x1F;
    
    OC2CON1bits.OCTSEL = 0b111;
    OC2CON2bits.SYNCSEL = 0x1F;    
    
    OC3CON1bits.OCTSEL = 0b111;
    OC3CON2bits.SYNCSEL = 0x1F;

    OC1CON2bits.OCTRIG = 0;
    OC1CON1bits.OCM = 0b110;
    
    OC2CON2bits.OCTRIG = 0;
    OC2CON1bits.OCM = 0b110;
    
    OC3CON2bits.OCTRIG = 0;
    OC3CON1bits.OCM = 0b110;
    
    //Vertical Sort read pin set up
    _TRISA3 = 1;
    _ANSA3 = 1;

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
    _CSS1 = 1;  //pin3 IR sensor middle
    _CSS13 = 1; //pin 7 IR sensor right
    _CSS14 = 1; //pin 8 Vertical load sensor
    
   
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
	_SMPI = 0b00101;		// AD1CON2<6:2>


	/*** Turn on A/D Module ***/
	_ADON = 1;			// AD1CON1<15
    
    _OC1IE = 1; //Turn on Motor interrupt
}

void set_up_driving_pins(void){
    _TRISB12 = 0; //Left Motor direction
    _ANSB12 = 0;
    _TRISB2 = 0; //Right Motor direction
    _ANSB2 = 0;
    
    //IR Sensors
    _TRISA0 = 1;
    _ANSA0 = 1;
    _TRISA1 = 1;
    _ANSA1 = 1;
    _TRISA2 = 1;
    _ANSA2 = 1;
    
    //Push button pins
    _TRISB8 = 1;
    _TRISB7 = 1;
    
    //Interrupt settings
    _CN23IE = 1; // Enable CN on pin right push button
    _CN22IE = 1; //Enable CN on left push button
    _CN23PUE = 0; // Disable pull-up resistor (CNPU1 register)
    _CN22PUE = 0;
    _CNIP = 6; // Set CN interrupt priority (IPC4 register)
    _CNIF = 0; // Clear interrupt flag (IFS1 register)
    _CNIE = 1; // Enable CN interrupts (IEC1 register)
}

void init(void){
    set_up_PWM_and_ADC();
    set_up_driving_pins();
}