#include <avr/io.h>
#include <util/delay.h>

#define DELAY 100

int main(void) {
    // set PD0 as trigger pin
    // make every pin an output
    DDRB = 0xFF;
    DDRC = 0xFF;
    DDRD = 0xFF;

    // set all outputs to 0
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;

    int i = 0;

    while (1) {
        // trigger
        PORTD |= (1 << PD0);
        _delay_ms(DELAY);
        PORTD &= ~(1 << PD0);
        _delay_ms(DELAY);

        // flip every other bit
        for (i = 0; i <= 5; ++i) {
            PORTB |= (1 << i);
            _delay_ms(DELAY);
            PORTB &= ~(1 << i);
            _delay_ms(DELAY);
        }

        PORTB |= (1 << PB7);
        _delay_ms(DELAY);
        PORTB &= ~(1 << PB7);
        _delay_ms(DELAY);

        for (i = 0; i <= 5; ++i) {
            PORTC |= (1 << i);
            _delay_ms(DELAY);
            PORTC &= ~(1 << i);
            _delay_ms(DELAY);
        }

        for (i = 1; i <= 7; ++i) {
            PORTD |= (1 << i);
            _delay_ms(DELAY);
            PORTD &= ~(1 << i);
            _delay_ms(DELAY);
        }
    }
}