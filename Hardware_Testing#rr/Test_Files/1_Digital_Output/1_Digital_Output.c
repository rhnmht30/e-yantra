/*

TITLE	: Digital Output
DATE	: 2019/11/12
AUTHOR	: e-Yantra Team

AIM: To individually blink each of the RGB LEDs.

CONNECTIONS:
* Red Led 	-> PIN_LED_RED
* Blue Led 	-> PIN_LED_BLUE
* Green Led -> PIN_LED_GREEN

NOTE:
* All the LEDs are Active Low.

*/

#define F_CPU 16000000UL // Define Crystal Frequency of eYFi-Mega Board
#include <avr/io.h>			 // Standard AVR IO Library
#include <util/delay.h>	// Standard AVR Delay Library

#define PIN_LED_RED PH3		// Macro for Pin Number of Red Led
#define PIN_LED_BLUE PH4	// Macro for Pin Number of Blue Led
#define PIN_LED_GREEN PH5 // Macro for Pin Number of Green Led

void init_led()
{
	DDRH |= (1 << PIN_LED_RED) | (1 << PIN_LED_BLUE) | (1 << PIN_LED_GREEN);	// initialize the pins PIN_LED_RED,PIN_LED_BLUE,PIN_LED_GREEN of port H as output pins.
	PORTH |= (1 << PIN_LED_RED) | (1 << PIN_LED_BLUE) | (1 << PIN_LED_GREEN); // set the values that all the LEDs remains off initially
}

void led_redOn(void)
{
	PORTH &= ~(1 << PIN_LED_RED); // Make PHPIN_LED_RED Low
}

void led_redOff(void)
{
	PORTH |= (1 << PIN_LED_RED); // Make PHPIN_LED_RED High
}

void led_blueOn(void)
{
	PORTH &= ~(1 << PIN_LED_BLUE); // Make PHPIN_LED_BLUE Low
}

void led_blueOff(void)
{
	PORTH |= (1 << PIN_LED_BLUE); // Make PHPIN_LED_BLUE High
}

void led_greenOn(void)
{
	PORTH &= ~(1 << PIN_LED_GREEN); // Make PHPIN_LED_GREEN Low
}

void led_greenOff(void)
{
	PORTH |= (1 << PIN_LED_GREEN); // Make PHPIN_LED_GREEN High
}

int main(void)
{

	init_led(); // initialize led pins

	while (1)
	{

		// turn ON red led for 1000 ms and OFF for 1000 ms
		led_redOn();
		_delay_ms(1000);
		led_redOff();
		_delay_ms(1000);

		// turn ON green led for 1000 ms and OFF for 1000 ms
		led_greenOn();
		_delay_ms(1000);
		led_greenOff();
		_delay_ms(1000);

		// turn ON blue led for 1000 ms and OFF for 1000 ms
		led_blueOn();
		_delay_ms(1000);
		led_blueOff();
		_delay_ms(1000);
	}
}
