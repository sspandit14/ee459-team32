/*************************************************************
*       at328-3.c - Demonstrate simple I/O functions of ATmega328P
*           and the use of an internal timer and interrupt
*
*       This program will cause a 7-segment display to either count up in
*       hexadecimal (0,1,2,...,E,F,0,1,...) or count down in decimal
*       (9,8,...,1,0,9,8,..) depending on whether or not a switch is
*       pressed.
*
*       Port C, bit 1 - input from switch (0 = pressed, 1 = not pressed)
*               When the switch is not pressed, the 7-segment display
*               counts up in hexadecimal. When the switch is pressed,
*               the 7-segment display counts down in decimal.
*       Port B, bits 0-1 and Port D, bits 2-6 - Outputs to data inputs of
*               the 74LS374 register.
*               Bit 6 -> segment A, 5->B, ... , 1->F, 0->G
*               A low output bit will cause the LED segment to light up.
*       Port C, bit 2 - Output to positive edge-triggered clock input
*               of 74LS374 register.
*
* Revision History
* Date     Author      Description
* 01/03/05 M. Redekopp Initial Release for MC68HC908JL3
* 10/07/05 A. Weber    Modified for JL8 processor
* 01/13/06 A. Weber    Modified for CodeWarrior 5.0
* 08/25/06 A. Weber    Modified for JL16 processor
* 11/30/06 A. Weber    Add struct for multiple bits
* 05/21/07 A. Weber    Some edits for clarity
* 08/17/07 A. Weber    Incorporated changes to demo board
* 04/22/08 A. Weber    Added "one" variable to make warning go away
* 04/25/08 A. Weber    Adjusted timing delay for new clock rate
* 04/08/11 A. Weber    Adapted for ATmega168
* 10/10/11 A. Weber    Fixed an error in timer prescaler comment
* 11/10/11 A. Weber    Clarifications to timer setup code.
* 11/28/11 A. Weber    More clarifications to timer setup code.
* 06/06/13 A. Weber    Cleaned it up a bit
* 11/18/13 A. Weber    Renamed for ATmega328P
*************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>

uint8_t digit_segs[16] = {
    0x7e,0x30,0x6d,0x79,0x33,0x5b,0x5f,0x70,
    0x7f,0x73,0x77,0x1f,0x4e,0x3d,0x4f,0x47 };
uint8_t cnt;

void display_digit(uint8_t);

#define SWITCH         (1 << PC1)
#define SEG_DATA_B     0x03     // Bits in Port B for LED display
#define SEG_DATA_D     0xfc     // Bits in Port D for LED display
#define SEG_CLOCK      (1 << PC2)

int main(void) {

    PORTC |= SWITCH;            // Turn on pull-up on PC1 for switch

    DDRC |= SEG_CLOCK;          // Set PORTC bit 2 for output
    DDRB |= SEG_DATA_B;         // Set PORTB bits 0-1 for output
    DDRD |= SEG_DATA_D;         // Set PORTD bits 2-6 for output

    /*
       The demo board has a 9.8304 MHz clock.  We want the timer to
       interrupt every half second (2 Hz) so we need to count clocks to
       9.8304MHz/2Hz = 4,915,200.  This is too big for the 16 bit counter
       register so use the prescaler to divide the clock by 256 and then
       count that clock to 19,200.
    */

    // Reset clears register bits to zero so only set the 1's
    TCCR1B |= (1 << WGM12);     // Set for CTC mode.  OCR1A = modulus
    TIMSK1 |= (1 << OCIE1A);    // Enable CTC interrupt
    sei();                      // Enable global interrupts
    OCR1A = 19200;              // Set the counter modulus
    TCCR1B |= (1 << CS12);      // Set prescaler for divide by 256,
                                // also starts timer

    display_digit(cnt);

    while(1) {                  // Loop forever while interrupts occur
    }

    return 0;   /* never reached */
}

void display_digit(uint8_t digit)
{
    uint8_t x;
    x = digit_segs[digit] ^ 0xff;  // invert bits (active low outputs)
    PORTB |= x & SEG_DATA_B;    // put low two bits in B
    PORTB &= (x | ~SEG_DATA_B);
    PORTD |= x & SEG_DATA_D;    // put high five bits in D
    PORTD &= (x | ~SEG_DATA_D);
    PORTC |= SEG_CLOCK;         // toggle the clock bit to 1
    PORTC &= ~SEG_CLOCK;        // toggle the clock bit to 0
}

ISR(TIMER1_COMPA_vect)
{
    uint8_t up;

    up = PINC & SWITCH;         // read the button
    if (up) {                   // if button is not pressed, up = 1
        if (++cnt > 15)         // and we count up in hex
            cnt = 0;
    }
    else {                      // if button is pressed, up = 0
        if (--cnt < 0 || cnt > 9)   // and we count down in decimal
            cnt = 9;
    }

    display_digit(cnt);
}

