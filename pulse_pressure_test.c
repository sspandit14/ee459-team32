#include <avr/io.h>
#include <util/delay.h>

uint16_t get_adc(uint8_t channel) {
    // ADMUX = (channel << MUX0);
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);

    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));

    return ADC;
}

int main(void) {
    // use PD2 and PD3 for LED test output
    DDRD |= (1 << PD2) | (1 << PD3);

    ADMUX = (1 << REFS0);
    ADMUX &= ~(1 << REFS1);
    ADCSRA = (1 << ADEN) | (7 << ADPS0); // enable ADC and set prescalar to 128

    while (1) {
        uint16_t pulse = get_adc(0);
        uint16_t pressure = get_adc(1);

        if (pulse > 600) {
            PORTD |= (1 << PD2);
        } else {
            PORTD &= ~(1 << PD2);
        }

        if (pressure > 400) {
            PORTD |= (1 << PD3);
        } else {
            PORTD &= ~(1 << PD3);
        }
    }
}