/*
 * File:   newmainXC16.c
 * Author: jaron
 *
 * Created on October 29, 2018, 5:52 PM
 */


#include "xc.h"
#define FCY 4000000UL
#include <libpic30.h>
#pragma config FNOSC = FRCDIV
#include "default_setup.h"
#include "Nav_func.h"
#include "shooting.h" 

void _ISR _OC1Interrupt(void){
    _OC1IF = 0;
    wheel_counter++;
}

void __attribute__((interrupt,no_auto_psv)) _CNInterrupt(void){
    _CNIF = 0;
    if(_RB7 == 1 && _RB8 == 1){
        hit_corner_flag = 1;
    }
    //if()
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
    
    _RCDIV = 0b110;

    init();
    
    while(1){
    
    _TRISB9 = 0;
    
    _LATB9 = 1;
    
    OC1R = 4000;
    OC1RS = 7999;
    }
    /**
    find_corner_and_go_to_center();
    
    char black_gate = detect_black_gate();
    
    vertical_sort(black_gate);
    */

    return 0;
}