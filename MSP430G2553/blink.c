#include <msp430.h>

#define LED1 BIT0
#define BUTTON1 BIT3


static void sleep(volatile unsigned int t); // for delay

void main()
{
	WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog
	P1DIR |= LED1; 					// Set P1.0 to output
	P1REN |= BUTTON1; 				// Set P1.3 to input


	int enabled = 1; // LED1 status
	for(;;)
	{

		// if BUTTON1 pressed
		while ((P1IN & BUTTON1) == 0)
		{
			if (enabled == 1)
			{
				P1OUT &= ~LED1; // toggle LED1
				enabled = 0; // change LED1 status
				sleep(40000); // waiting anti-bounce
				break;
			}
			else
			{
				enabled = 1; // change LED1 status to enable
				sleep(40000);
				break;
			}
		}
		if (enabled)
			P1OUT ^= BIT0; // Toggle P1.0 using exclusive-OR
		sleep(10000);
	}
}

void sleep(volatile unsigned int t)
{
	do t--;
	while(t != 0);
}
