/*************************************************************
*       at328-2.c - Demonstrate simple I/O functions of ATmega328P
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
* 09/05/05 A. Weber    Modified for JL8 processor
* 01/13/06 A. Weber    Modified for CodeWarrior 5.0
* 08/25/06 A. Weber    Modified for JL16 processor
* 04/23/07 A. Weber    Split example 2 into 2a and 2b
* 08/17/07 A. Weber    Incorporated changes to demo board
* 04/22/08 A. Weber    Added "one" variable to make warning go away
* 04/23/08 A. Weber    Adjusted wait_100ms for 9.8304MHz clock
* 04/03/11 A. Weber    Adapted for ATmega168
* 06/06/13 A. Weber    Cleaned it up a bit
* 11/18/13 A. Weber    Renamed for ATmega328P
*************************************************************/

#include <avr/io.h>
#include <util/delay.h>

unsigned char digit_segs[16] = {
    0x7e,0x30,0x6d,0x79,0x33,0x5b,0x5f,0x70,
    0x7f,0x73,0x77,0x1f,0x4e,0x3d,0x4f,0x47 };

void display_digit(unsigned char);

#define SWITCH         (1 << PC1)
#define SEG_DATA_B     0x03     // Bits in Port B for LED display
#define SEG_DATA_D     0xfc     // Bits in Port D for LED display
#define SEG_CLOCK      (1 << PC2)

int main(void) {
    unsigned char up;
    unsigned char cnt = 0;

    PORTC |= SWITCH;            // Turn on pull-up on PC1 for switch

    DDRC |= SEG_CLOCK;          // Set PORTC bit 2 for output
    DDRB |= SEG_DATA_B;         // Set PORTB bits 0-1 for output
    DDRD |= SEG_DATA_D;         // Set PORTD bits 2-6 for output

    while(1) {                  // Loop forever
        display_digit(cnt);

        _delay_ms(500);         // wait 500 ms

        up = PINC & SWITCH;     // read the button
        if (up) {               // if button is not pressed, up = 1
            if (++cnt > 15)     // and we count up in hex
                cnt = 0;
        }
        else {                          // if button is pressed, up = 0
            if (--cnt < 0 || cnt > 9)   // and we count down in decimal
                cnt = 9;
        }
    }
    return 0;   /* never reached */
}

void display_digit(unsigned char digit)
{
    unsigned char x;
    x = digit_segs[digit] ^ 0xff;  // invert bits (active low outputs)
    PORTB |= x & SEG_DATA_B;    // put low two bits in B
    PORTB &= (x | ~SEG_DATA_B);
    PORTD |= x & SEG_DATA_D;    // put high five bits in D
    PORTD &= (x | ~SEG_DATA_D);
    PORTC |= SEG_CLOCK;         // toggle the clock bit to 1
    PORTC &= ~SEG_CLOCK;        // toggle the clock bit to 0
}