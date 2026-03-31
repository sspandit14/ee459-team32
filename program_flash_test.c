#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    // make every pin an output
    DDRB = 0xFF;
    DDRC = 0xFF;
    DDRD = 0xFF;

    // set all outputs to 0
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;

    while (1) {
        PORTC |= (1 << PC0);
        _delay_ms(1000);
        PORTC &= ~(1 << PC0);
        _delay_ms(1000);
    }
}