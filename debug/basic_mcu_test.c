#include <avr/io.h>
#include <util/delay.h>

#define DELAY 100

int main(void) {
    // set PD0 as trigger pin
    // make every pin an output
    DDRB |= (1 << PB0);

    // set all outputs to 0
    PORTB &= ~(1 << PB0);

    while (1) {
        PORTB |= (1 << PB0);
        _delay_ms(500);
        PORTB &= ~(1 << PB0);
    }
}