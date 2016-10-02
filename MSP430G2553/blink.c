#include <msp430.h>

#define LED1 BIT0
#define BUTTON1 BIT3

#define BLINK_DELAY 12500

// LED1 status
int enabled = 1;

void main()
{
	WDTCTL = WDTPW | WDTHOLD; // Stop watchdog

	// Cleaning everything
	P1OUT &= 0x0;
	P1DIR &= 0x0;

	// Enable LED1
	P1DIR |= LED1; 	// set P1.0 to output
	P1OUT |= BIT0;

	// Configure BUTTON1
	P1REN |= BUTTON1;
	P1OUT |= BUTTON1;
	P1IE |= BUTTON1; // BUTTON1 interrupt enable
	P1IES |= BUTTON1;
	P1IFG &= ~BUTTON1;

	// Configure timer
	TACCTL0 |= CCIE;		// enable interrupts on timer
	TACCR0 = BLINK_DELAY;	// number of delay cycles
	TACTL |= TASSEL_1;
	TACTL |= MC_1;

	// shut down CPU while interrupts
	__bis_SR_register(CPUOFF | GIE);
}


#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0(void)
{
	TACCTL1 &= ~CCIFG;
	if (enabled)
		P1OUT ^= LED1;	// blinking the LED
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
	// changing LED1 status
	if (enabled == 1)
	{
		enabled = 0;
		P1OUT &= ~LED1; // toggle LED1
	}
	else if (enabled == 0)
	{
		enabled = 1;
		P1OUT &= ~LED1; //  LED1
	}
	// delay while
	__delay_cycles(1000);
	P1IFG = 0x0;
}
