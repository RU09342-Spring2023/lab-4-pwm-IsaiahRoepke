/*
 * Hardware PWM.c
 *
 *  Created on: Feb 21, 2023
 *      Author: Isaiah Roepke
 *  Last Modified: March 2, 2023
 *
 */

#include <msp430.h> 


/**
 * main.c
 */

char LEDMode = 0x00; //this character is initialized as 0 and saves the LED mode the board is currently in.

void LEDSetup(); //prototype for LED setup
void TimerB3Setup(); //prototype for Timer 3 setup
void TimerB2Setup(); //prototype for Timer 2 setup

int main(void)
{

	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5; //enable low power GPIO
    __bis_SR_register(GIE);             // Enter LPM0 w/ Interrupts
    LEDSetup(); //call LED setup function
    TimerB3Setup(); //call Timer 3 setup function
    TimerB2Setup(); //call timer 2 setup funciton

    while(1){
        switch(LEDMode){
           case 0x00: //RED
               TB3CCR1 = 999; //enable full power for red led
               TB3CCR2 = 0; //0 power for green
               TB3CCR3 = 0; //0 power for blue

               break;
           case 0x01: //RED + GREEN = ORANGE (off color, red + green doesn't mix well)
               TB3CCR1 = 999; //enable full power for red
               TB3CCR2 = 300; //enable 30% power for green
               TB3CCR3 = 0; //0 power for blue
               break;
           case 0x02: //GREEN
               TB3CCR1 = 0; //0 power for red
               TB3CCR2 = 999; //enable full power for green
               TB3CCR3 = 0; //0 power for blue
               break;
           case 0x03: //GREEN + BLUE = CYAN
               TB3CCR1 = 0; //0 power for red
               TB3CCR2 = 999; //enable full power for green
               TB3CCR3 = 999; //enable full power for blue
               break;
           case 0x04: //BLUE
               TB3CCR1 = 0; //0 power for red
               TB3CCR2 = 0; //0 power for green
               TB3CCR3 = 999; //enable full power for blue
               break;
           case 0x05: //BLUE + RED + GREEN = PURPLE
               TB3CCR1 = 627; //enable 62.7% power for red
               TB3CCR2 = 125; //enable 12.5% power for green
               TB3CCR3 = 941; //enable 94.1% power for blue
               break;
        }

    }


	return 0;
}

void LEDSetup(){
    P6DIR |= BIT0;                     // P6.0 output
    P6SEL0 |= BIT0;
    P6SEL1 &= ~BIT0;                    // P6.0 options select, secondary mode

    P6DIR |= BIT1;                     // P6.1 output
    P6SEL0 |= BIT1;
    P6SEL1 &= ~BIT1;                    // P6.1 options select, secondary mode

    P6DIR |= BIT2;                     // P6.2 output
    P6SEL0 |= BIT2;
    P6SEL1 &= ~BIT2;                    // P6.2 options select, secondary mode


}

void TimerB3Setup(){

    TB3CCTL0 = CCIE;
    TB3CCR0 = 1000-1;                         // PWM Period
    TB3CCTL1 = OUTMOD_3;                      // CCR1 set/reset
    TB3CCR1 = 999;                            // CCR1 PWM duty cycle
    TB3CCTL2 = OUTMOD_3;                      // CCR2 set/reset
    TB3CCR2 = 0;                              // CCR2 PWN duty cycle
    TB3CCTL3 = OUTMOD_3;                      // CCR3 set/reset
    TB3CCR3 = 0;                              // CCR3 PWN duty cycle
    TB3CTL = TBSSEL__SMCLK | MC__UP | TBCLR | ID_3;  // SMCLK, up mode, clear TBR, clk divider = /8


}
void TimerB2Setup(){

    TB2CCTL0 = CCIE;
    TB2CCR0 = 2000-1;                         // PWM Period
    TB2CCTL1 = OUTMOD_3;                      // CCR1 set/reset
    TB2CCR1 = 1999;                            // CCR1 PWM duty cycle

    TB2CTL = TBSSEL__ACLK | MC__UP | TBCLR | ID_3;  // ACLK, up mode, clear TBR,  clk divider = /8


}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER3_B0_VECTOR //timer3 B0 interrupt vector
__interrupt void Timer3_B0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER3_B0_VECTOR))) Timer3_B0_ISR (void)
#else
#error Compiler not supported!
#endif
{
 //don't have any function that runs off of timer 3 B0. Just make sure TB3R resets to 0


}
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER2_B0_VECTOR //timer2 B0 interrupt vector
__interrupt void Timer2_B0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER2_B0_VECTOR))) Timer2_B0_ISR (void)
#else
#error Compiler not supported!
#endif
{

    switch(LEDMode){ //Every time timer 2 B0 is triggered, go to the next LEDMode. This is done by iterating the LEDMode character by 1. If the LEDMode is 0x05, roll over back to one, making the LEDMode cycle.
      case 0x00:
          LEDMode = 0x01;
          break;
      case 0x01:
          LEDMode = 0x02;
          break;
      case 0x02:
          LEDMode = 0x03;
          break;
      case 0x03:
          LEDMode = 0x04;
          break;
      case 0x04:
          LEDMode = 0x05;
          break;
      case 0x05:
          LEDMode = 0x00;
          break;

      }

}

