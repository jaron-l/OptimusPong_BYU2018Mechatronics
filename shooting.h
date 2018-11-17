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

#define LeftIR ADC1BUF0
#define MidIR ADC1BUF1
#define RightIR ADC1BUF13

int left_vertSort_PWM = 62;
int mid_vertSort_PWM = 93;
int right_vertSort_PWM = 125;

char vertical_sort(char gate){
    //gate: 'l' = left gate from dispensor, 'm' = middle gate, 'r' = right gate
    
    if(gate == 'l'){
        OC2R = left_vertSort_PWM;
    }
    else if(gate == 'm'){
        OC2R = mid_vertSort_PWM;
    }
    else if(gate == 'r'){
        OC2R = right_vertSort_PWM;
    }
    else{
        return 'f';
    }
    return 'p';
}

char detect_black_gate(void){
    char black_gate;// 'l' for left, 'm' for mid, 'r' for right, 'u' for unknown
    if(LeftIR > LeftIRThreshold){
        black_gate = 'l';
    }
    else if(MidIR > MidIRThreshold){
        black_gate = 'm';
    }
    else if(RightIR > RightIRThreshold){
        black_gate = 'r';
    }
    else{
        black_gate = 'u';
    }
    return black_gate;
}