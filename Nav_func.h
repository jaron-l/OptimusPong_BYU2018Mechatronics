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

#define LeftWheel _LATB12
#define RightWheel _LATB2

#define LeftIR ADC1BUF0
#define MidIR ADC1BUF1
#define RightIR ADC1BUF13

int wheel_counter = 0;

int hit_corner_flag = 0; //1 for when both push buttons have been activated

int speed = 7999;

int LeftIRThreshold = 1187;
int MidIRThreshold = 1105;
int RightIRThreshold = 1185;

void rampPWM(char x){
    //Hard coded to only work with ramping from zero to speed and from speed fown to 0
    //x: 'b' = backwards, 'f' = forwards
    if(x == 'f'){
        LeftWheel = 1;
        RightWheel = 1;
    }
    else{
        LeftWheel = 0;
        RightWheel = 0;
    }
    int rate =  speed/4;// ramp rate in OC1R per .5 seconds
    if(OC1R == 0){
        OC1RS = 0;
        while((speed - OC1RS) > (rate - 1)){
            OC1RS += rate;
            OC1R = (int)(.5 * OC1RS);
            __delay_ms(100);
        }
        OC1RS = speed;
        OC1R = (int)(.5 * OC1RS);
    }
    else{
        while((OC1RS - speed) > (rate - 1)){
            OC1RS -= rate;
            OC1R = (int)(.5 * OC1RS);
            __delay_ms(100);
        }
        OC1RS = speed;
        OC1R = (int)(.5 * OC1RS);
    }
}

void drive(char direction,float howFar){
    //char direction: f = forward, b = backwards
    //float howFar: how long to drive in car lengths
    
    howFar = (int)howFar*800/3; //estimate NEEDS TUNING
    
    if(direction == 'f'){
        LeftWheel = 1;
        RightWheel = 1;
        rampPWM(speed);
        while(wheel_counter < howFar);
        OC1R = 0;
        wheel_counter = 0;
    }
    else if (direction == 'b'){
        LeftWheel = 0;
        RightWheel = 0;
        rampPWM(speed);
        while(wheel_counter < howFar);
        OC1R = 0;
        wheel_counter = 0;
    }
}

void turn(int angle, char direction){
    //angle: in degrees
    //direction: r = right, l = left
    
    angle = angle/90*300;
    
    if(angle == 'r'){
        LeftWheel = 1;
        RightWheel = 0;
        rampPWM(speed);
        while(wheel_counter < angle);
        OC1R = 0;
        wheel_counter = 0;
    }
    else{
        LeftWheel = 0;
        RightWheel = 1;
        rampPWM(speed);
        while(wheel_counter < angle);
        OC1R = 0;
        wheel_counter = 0;
    }
}

void find_corner_and_go_to_center(void){
    
    //find center
    while(hit_corner_flag == 0){
        RightWheel = 1;
        LeftWheel = 1;
        rampPWM('f');
    }
    hit_corner_flag = 0;
    drive('b',2.0);
    
    //find orientation
    LeftWheel = 0;
    RightWheel = 1;
    rampPWM(speed);
    //all IR sensors are low, wait
    while(LeftIR < LeftIRThreshold && MidIR < MidIRThreshold && RightIR < RightIRThreshold);
    //find the IR sensor that went high and determine what to do
    if(LeftIR > LeftIRThreshold){
        OC1R = 0;
        wheel_counter = 0;
        turn(90,'r');
    }
    else if(MidIR > MidIRThreshold){
        OC1R = 0;
        wheel_counter = 0;
        turn(180,'r');
    }
    else if(RightIR > RightIRThreshold){
        OC1R = 0;
        wheel_counter = 0;
        turn(90,'l');
    }
}