#include <msp430.h> 


/**
 * main.c
 */

void LEDSetup();
void ButtonSetup();
void TimerB0Setup();
void TimerB1Setup();

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	

	LEDSetup(); //Setup LEDs
	ButtonSetup(); //Setup Buttons
	TimerB0Setup(); //Setup TimerB0
	TimerB1Setup(); //Setup Timer B1

	PM5CTL0 &= ~LOCKLPM5; //Enable low power mode for GPIO pins
	__bis_SR_register(LPM0_bits + GIE); //Enable global interrupts and low power mode 0

	return 0;
}

void LEDSetup(){
    P1OUT &= ~BIT0; //Setup P1.0/red LED
    P1DIR |= BIT0; //as output

    P6OUT &= ~BIT6; //Setup P6.6/green LED
    P6DIR |= BIT6; //as output

}


void ButtonSetup(){
    P2OUT |= BIT3;                          // Configure P2.3 as pulled-up
      P2REN |= BIT3;                          // P2.3 pull-up register enable
      P2IES |= BIT3;                         // P2.3 High to low edge/active low
      P2IE |= BIT3;                           // P2.3 interrupt enabled

      P4OUT |= BIT1;                          // Configure P4.1 as pulled-up
      P4REN |= BIT1;                          // P4.1 pull-up register enable
      P4IES |= BIT1;                         // P4.1 High to low edge/active low
      P4IE |= BIT1;                           // P4.1 interrupt enabled

}


void TimerB0Setup(){
    TB0CCTL1 = CCIE;                          // TB0CCR0 interrupt enabled
    TB0CCR0 = 1000; //1000 is used to start for the Capture Compare Register
    TB0CTL = TBSSEL_2 | MC_1 | TBCLR | TBIE |ID_3 ;                 // ACLK, continuous mode, interrupts enabled, clk divider = /8
}

void TimerB1Setup(){
        TB1CCTL1 = CCIE;                          // TB1CCR0 interrupt enabled
        TB1CCR0 = 1000; //1000 is used to start for the Capture Compare Register
        TB1CTL = TBSSEL_2 | MC_1 | TBCLR | TBIE | ID_3 ;                 // ACLK, up mode, interrupts enabled, clk divider = /8

}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER0_B1_VECTOR //timer0 B1 interrupt vector
__interrupt void Timer0_B1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_B0_VECTOR))) Timer1_B0_ISR (void)
#else
#error Compiler not supported!
#endif
{

    switch(__even_in_range(TB0IV,TB0IV_TBIFG))
        {
            case TB0IV_NONE:
                break;                               // No interrupt
            case TB0IV_TBCCR1:
                P1OUT &= ~BIT0;
                break;                               // CCR1 Set the pin to a 0
            case TB0IV_TBCCR2:
                break;                               // CCR2 not used
            case TB0IV_TBIFG:
                P1OUT |= BIT0;                       // overflow Set the pin to a 1
                break;
            default:
                break;
        }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER1_B1_VECTOR //timer B0 interrupt vector
__interrupt void Timer1_B1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_B0_VECTOR))) Timer1_B0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(TB1IV,TB1IV_TBIFG))
        {
            case TB1IV_NONE:
                break;                               // No interrupt
            case TB1IV_TBCCR1:
                P6OUT &= ~BIT6;
                break;                               // CCR1 Set the pin to a 0
            case TB1IV_TBCCR2:
                break;                               // CCR2 not used
            case TB1IV_TBIFG:
                P6OUT |= BIT6;                       // overflow Set the pin to a 1
                break;
            default:
                break;
        }

}

#pragma vector=PORT2_VECTOR //port 2 interrupt vector
__interrupt void Port_2(void){

    P2IFG &= ~BIT3; //port 2 interrupt flag is cleared
    if(TB0CCR1 >= 1000){ //if the CCR1 register reaches the value of the CCR0 register
        TB0CCR1 = 0; //Roll over as an overflow
    }
    else {
        TB0CCR1 += 100; //else, add 100 to the CCR1 register
    }
}

#pragma vector=PORT4_VECTOR //port 4 interrupt vector
__interrupt void Port_4(void){
    P4IFG &= ~BIT1; //port 4 interrupt flag is cleared
        if(TB1CCR1 >= 1000){ //if the CCR1 register reaches the value of the CCR0 register
            TB1CCR1 = 0; //Roll over as an overflow
        }
        else {
            TB1CCR1 += 100; //else, add 100 to the CCR1 register
        }

}
