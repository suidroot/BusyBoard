/*
 *  Test code for BusyBoard v1
 *  Ben Mason
 */

#include <avr/io.h>
#include <util/delay.h>
#include "includes/ADC.h"
#include "includes/USART.h"

#define DEBOUNCE_TIME  1000                            /* microseconds */

// Pot input
#define ADCPIN 0

#define LED0PIN PB0
#define LED1PIN PB1
#define LED2PIN PB2
#define LED3PIN PB3

#define BUTTON0PIN PB4
#define BUTTON1PIN PB5
#define BUTTON2PIN PC1
#define BUTTON3PIN PC2

#define SWITCH0PIN PC3
#define SWITCH1PIN PC4
#define SWITCH2PIN PC5
#define SWITCH3PIN PD2
#define SWITCH4PIN PD3
#define SWITCH5PIN PD4
#define SWITCH6PIN PD5
#define SWITCH7PIN PD6

// #define ROTENCBUTTON
// #define ROTENC0
// #define ROTENC1

void anykey(void);
void blinkled(uint8_t port);
void readswitch(uint8_t port, uint8_t pin);
void readbutton(uint8_t port, uint8_t pin);
// uint8_t debounce(void);


int main(void) {


	initUSART();
	initSingleADC0(ADCPIN, ADC_PRESCALE_32);

	printString("\r\nWelcome to BusyBoard Tester\r\n");
	anykey();

	while (1) {

		printString("Start LED Test\r\n");
		// Set LEDs to output
		DDRB = ((1 << LED0PIN) | (1 << LED1PIN) | (1 << LED2PIN) | (1 << LED3PIN));
		blinkled(LED0PIN);
		blinkled(LED1PIN);
		blinkled(LED2PIN);
		blinkled(LED3PIN);
		printString("LEDs tested\r\n");
		// anykey();

		printString("Start Button Test\r\n");
		// Setup Button Ports
		PORTB |= ((1 << BUTTON0PIN) | (1 << BUTTON1PIN));
		PORTC |= ((1 << BUTTON2PIN) | (1 << BUTTON3PIN));

		DDRB &= ~((1 << BUTTON0PIN) | (1 << BUTTON1PIN));
		DDRC |= ((0 << BUTTON2PIN) | (0 << BUTTON3PIN));

		// PINB &= ~((1 << BUTTON0PIN) | (1 << BUTTON1PIN));
		// PINC &= ~((1 << BUTTON2PIN) | (1 << BUTTON3PIN));
		// Set PullUps

		readbutton(PINB, PB4);
		anykey();
		readbutton(PINB, BUTTON1PIN);
		anykey();
		readbutton(PINC, BUTTON2PIN);
		anykey();
		readbutton(PINC, BUTTON3PIN);
		anykey();

		printString("Start Switch Test\r\n");
		PORTC |= ((1 << SWITCH0PIN) | (1 << SWITCH1PIN) | (1 << SWITCH2PIN));
		PORTD |= ((1 << SWITCH3PIN) | (1 << SWITCH4PIN) | (1 << SWITCH5PIN) | (1 << SWITCH6PIN) | (1 << SWITCH7PIN));

		readswitch(PORTC, SWITCH0PIN);
		anykey();

	}

	return(0);
}

void anykey(void) {

	printString("\r\nPress Enter to Continue");
	receiveByte();
	printString("\r\n");

}

// uint8_t debounce(uint8_t port, uint8_t pin) {
//   if (bit_is_clear(pin, port)) {      /* button is pressed now */
//     _delay_us(DEBOUNCE_TIME);
//     if (bit_is_clear(pin, port)) {            /* still pressed */
//       return (1);
//     }
//   }
//   return (0);
// }


void readbutton(uint8_t portin, uint8_t pin) {

	uint8_t buttonpressed = 0;

	printString("Press button connected to ");
	printByte(pin);
	printString("\r\n");

	do {
		if (bit_is_clear(portin, pin)) {
			buttonpressed = 1;
			printString("ping");
		} 
	} while (buttonpressed == 0);

	printString("Button press detected\r\n");
}

void readswitch(uint8_t port, uint8_t pin) {

	printString("Flip Switch connected to ");
	printByte(pin);
	printString("\r\n");
	loop_until_bit_is_clear(pin, port);
	printString("Switch turned on\r\n");
	printString("Turn switch off\r\n");
	loop_until_bit_is_set(port, pin);
	printString("Switch turned off\r\n");

}


void blinkled(uint8_t port) {

		printByte(port);
		printString(" On\r\n");
		PORTB |= (1 << port);
		_delay_ms(1000);
		printByte(port);
		printString(" Off\r\n");
		PORTB &= ~(1 << port);
		_delay_ms(1000);

}

