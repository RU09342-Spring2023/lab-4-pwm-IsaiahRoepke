/*
 * Servo Control.c
 *
 *  Created on: March 1, 2023
 *      Author: Isaiah Roepke
 *  Last Modified: March 1, 2023
 *
 */

#include <msp430.h> 

void ServoSetup();
void ButtonSetup();
void TimerB3Setup();

/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	PM5CTL0 &= ~LOCKLPM5;
	__bis_SR_register(GIE);             // Enter LPM0 w/ Interrupts
	ServoSetup();
	ButtonSetup();
	

	return 0;
}



void ServoSetup(){
    P6DIR |= BIT0;                     // P6.0 output
    P6SEL0 |= BIT0;
    P6SEL1 &= ~BIT0;                    // P6.0 options select




}

void ButtonSetup(){
    P2OUT |= BIT3;                          // Configure P2.3 as pulled-up
      P2REN |= BIT3;                          // P2.3 pull-up register enable
      P2IES |= BIT3;                         // P2.3 High to low edge/active low
      P2IE |= BIT3;                           // P2.3 interrupt enabled

      P4OUT |= BIT1;                          // Configure P2.3 as pulled-up
      P4REN |= BIT1;                          // P2.3 pull-up register enable
      P4IES |= BIT1;                         // P2.3 High to low edge/active low
      P4IE |= BIT1;                           // P2.3 interrupt enabled

}

void TimerB3Setup(){

    TB3CCTL0 = CCIE;
    TB3CCR0 = 1000-1;                         // PWM Period
    TB3CCTL1 = OUTMOD_7;                      // CCR1 reset/set
    TB3CCR1 = 500;                            // CCR1 PWM duty cycle

    TB3CTL = TBSSEL__SMCLK | MC__UP | TBCLR | ID_3;  // SMCLK, up mode, clear TBR

}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER3_B0_VECTOR //timer B3 interrupt vector
__interrupt void Timer3_B0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER3_B0_VECTOR))) Timer3_B0_ISR (void)
#else
#error Compiler not supported!
#endif
{

}

#pragma vector=PORT2_VECTOR //port 2 interrupt vector
__interrupt void Port_2(void){

    P2IFG &= ~BIT3;
    if(TB0CCR1 >= 1000){
        break;
    }
    else {
        TB0CCR1 += 1;
    }
}

#pragma vector=PORT4_VECTOR //port 4 interrupt vector
__interrupt void Port_4(void){
    P4IFG &= ~BIT1;
        if(TB1CCR1 <= 50){
         break;
        }
        else {
            TB1CCR1 -= 1;
        }

}


