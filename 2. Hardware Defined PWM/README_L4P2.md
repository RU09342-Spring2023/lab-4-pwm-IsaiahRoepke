# Hardware PWM Control, LED Brightness Control

 
## Prerequisites 
One must have an MSP430FR2533 available for use. 
One must also have 4 wires and 3 resistors to connect the RGB LED to the MSP430FR2533. A 1.5k ohm resistors are used for the red and green LED.
A 910 ohm resistor is used for the blue LED. 
 
## Modules 
* Code Composer Studio - 12.2.0  
 
## Function of the Program 
This code uses hardware pulse-width modulation (PWM) to control the brightness and combinations of colors
of a RGB LED. The code automatically cycles between 6 different states to display 6 different colors using
two separate timer modules. 
## Pre-main.c code 
The character `LEDMode` is declared and assigned the value of 0x00. This will be the initial mode of the 
RGB LED. 
Three function prototypes are declared. `void LEDSetup()`, `void TimerB3Setup()`, and `void TimerB2Setup()`. 
The first is what will initialize the pins connected to the RGB LED. 
The second will set up timer 3, which will be used to control the brightness of each LED. 
The third will set up timer 2, which will be used to control the transitions between the different modes.  
## main.c 
The watchdog timer is put on hold to prevent the cpu from reseting. 
The code `PM5CTL0 &= ~LOCKLPM5` is used to enable low power mode for the GPIO pins. 
`__bis_SR_register(GIE)` is used to enable general interrupts.

## while(1)
The while loop contains one switch statement that controls the different modes of the LED.
The condition of the switch statement is the character LEDMode. Based on the value of LEDMode, 
a case will be chosen that sets the PWM values for each LED. 
case 0x00 will turn on the red LED alone, producing the color red. 
case 0x01 will turn on the red and green LEDs, producing the color orange. 
case 0x02 will turn on the green LED alone, producing the color green. 
case 0x03 will turn on the green and blue LEDs, producing the color cyan. 
case 0x04 will turn on the blue LED alone, producing the color blue. 
case 0x05 will turn on the red, green and 
## LEDSetup()
Port 6 is used as it allows the three RGB LEDs to be actuated by the timer 3 module, controlling their brightness. 
To do this, P6.0, P6.1, and P6.2 are all initialized as outputs with their secondary modes enabled. 

## TimerB3Setup()
Timer 3 is used to initialize the RGB LEDs in their first mode, where red is on and the other LEDs are off. This is done 
in the same way as case 0x00. 
`TB3CCTL0 = CCIE` is used to enable interrupts for the timer 3 CCR0 register. The TB3CCR0 register is initialized as 1000-1, so that the PWM
period is 999. The control registers for each CCRX register is set so that the CCRX register acts in a set/reset procedure. 
This is done because the RGB LED has a common anode, so the LEDs are always connected to power. When set, the power is able
to be transmitted through the LED. After the interrupt is triggered and the pin is reset, the power is kept from being transmitted, 
disallowing power to be transmitted. 
The control register for timer 3 is configured so that timer 3 is connected to the SMCLK in upmode with a clk divider of /8. 
 
## TimerB2Setup()
Timer 2 is used to control the transition between the different colors of the RGB LED. 
`TB2CCTL0 = CCIE` is used to enable interrupts for the timer 2 CCR0 register. The TB2CCR0 is initialized as 2000-1, or 1999.
This is larger than TB3CCR0 because if they were the same, the colors would change faster than desired. Extending the period
slows the transition speed between the colors. 
The timer 2 control register is configured so that timer 2 runs off of ACLK in upmode with a clk divider of /8. The auxillary
clock is chosen instead of the SMCLK to further slow the transitions. 

## Timer3B0 interrupt vector
There is no function of the Timer 3 CCR0 interrupt vector. It is there to make sure the interrupt runs and may be redundant.

## Timer2B0 interrupt vector
The timer 2 CCR0 interrupt vector is used to control the LEDMode character value. 
This is handled simply by a switch case with LEDMode as the argument. If the value of LEDMode is 0x00, it will be set to 
0x01. If the value of LEDMode is 0x05, it will be set to 0x00, so on and so forth, infinitely cycling.




