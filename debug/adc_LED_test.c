#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    // make every pin an output
    DDRB |= (1 << PB7) | (1 << PB0);
    DDRD |= (1 << PD5) | (1 << PD6) | (1 << PD7);

    // set all outputs to 0
    PORTB &= ~((1 << PB7) | (1 << PB0));
    PORTD &= ~((1 << PD5) | (1 << PD6) | (1 << PD7));

    while (1) {
        PORTB |= (1 << PB7) | (1 << PB0);
        PORTD |= (1 << PD5) | (1 << PD6) | (1 << PD7);
        _delay_ms(1000);
        PORTB &= ~((1 << PB7) | (1 << PB0));
        PORTD &= ~((1 << PD5) | (1 << PD6) | (1 << PD7));
        _delay_ms(1000);
    }
}