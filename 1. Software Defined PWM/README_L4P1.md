# Software PWM Control, LED Brightness

 
## Prerequisites 
One must have an MSP430FR2533 available for use. 
 
## Modules 
* Code Composer Studio - 12.2.0  
 
## Function of the Program 
The code uses software pulse-width modulation (PWM) to control the brightness of both built-in LEDs
on the MSP430FR2533. The code provides 10 different brightnesses for each LED using 2 different 
timer modules. The built-in buttons of the MSP430FR2533 are used to cycle between the brightnesses of
the LEDs. 
## Pre-main.c code 
4 prototype void functions are declared. `LEDSetup()` will initialize the on board LEDs.
`ButtonSetup()` initializes the buttons for use. `TimerB0Setup()` initializes timer 0. `TimerB1Setup()`
initializes timer 1. This is a funciton based approach that allows for easy customization and 
troubleshooting for the user. 
## main.c 
First, the watchdog timer is held so that the CPU does not reset. Then the 4 functions that initialized
the LEDs, Buttons, and Timers are called. `PM5CTL0 &= ~LOCKLPM5` is used to enable low power mode
for the GPIO pins. `__bis_SR_register(LPM0_bits + GIE)` is used to enable low power mode 0 with
global interrupts enabled. This is needed so that the timer interrupts work, enabling PWM functionality. 

## LEDSetup()
Port 1.0, the red LED, is initialized with an output of 0 and as an output. Port 6.6, the green
LED, is initialized with an output of 0 and as an output. 
## ButtonSetup()
Button 2.3 is configured as an output with a pull-up register. The button is active low as to conserve 
power. The interrupt looks for a high to low edge transistion so that the interrupt triggers right as
the button is pressed. Button 2.3 interrupts are enabled. 
Button 4.1 is configured as an output with a pull-up register. The button is active low as to conserve 
power. The interrupt looks for a high to low edge transistion so that the interrupt triggers right as
the button is pressed. Button 4.1 interrupts are enabled. 
 
## TimerB0Setup()
The `TB0CCTL1 = CCIE` line of code is used to enable the interrupt for TB0CCR0.
TB0CCR0 is set to 1000. Whenever TB0R reaches 1000, the timer 0 interrupt is triggered.
The timer 0 control registered is assigned so that timer 0 uses ACLK, up mode, interrupts enabled, with a clk divider of /8.

## TimerB1Setup()
The `TB1CCTL1 = CCIE` line of code is used to enable the interrupt for TB1CCR0.
TB1CCR0 is set to 1000. Whenever TB1R reaches 1000, the timer 1 interrupt is triggered.
The timer 1 control registered is assigned so that timer 1 uses ACLK, up mode, interrupts enabled, with a clk divider of /8.

## Timer0B1 interrupt vector
Whenever TB0R reaches the value of TB0CCR1, this interrupt is triggered. This interrupt controls the output of P1.0, the red LED. 
The interrupt uses a switch statement to decide what state the output of the LED should be.
If the interrupt vector was triggered by CCR1, then the output of P1.0 is set to 0. 
If the interrupt vector was triggered by CCR0, then the output of P1.0 is set to 1. 

## Timer1B1 interrupt vector
Whenever TB1R reaches the value of TB1CCR1, this interrupt is triggered. This interrupt controls the output of P6.6, the green LED. 
The interrupt uses a switch statement to decide what state the output of the LED should be.
If the interrupt vector was triggered by CCR1, then the output of P6.6 is set to 0. 
If the interrupt vector was triggered by CCR0, then the output of P6.6 is set to 1. 

## Port 2 interrupt vector
Whenever the P2.3 button is pressed, this interrupt triggers. 
The interrupt contains an if and if-else statement that controls the TB0CCR1 register. 
If TB0CCR1 is greater than or equal to 1000, which is the value of the TB0CCR0 register, 
then TB0CCR1 is set to 0. This is where the PWM control would be at 100%, so rolling over to 
0 allows the brightness of the red LED to be cycled any amount of times. 
Else if TB0CCR1 is less than TB0CCR0, the register is iterated by 100. This gives 10 different brightnesses
that the red LED can be. 
## Port 4 interrupt vector
Whenever the P4.1 button is pressed, this interrupt triggers. 
The interrupt contains an if and if-else statement that controls the TB1CCR1 register. 
If TB1CCR1 is greater than or equal to 1000, which is the value of the TB1CCR0 register, 
then TB1CCR1 is set to 0. This is where the PWM control would be at 100%, so rolling over to 
0 allows the brightness of the green LED to be cycled any amount of times. 
Else if TB1CCR1 is less than TB1CCR0, the register is iterated by 100. This gives 10 different brightnesses
that the green LED can be.



