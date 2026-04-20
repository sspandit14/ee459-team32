#include "../src/adc_read.h"
#include <avr/io.h>
#include <util/delay.h>


#define PRESSURE_LED PB7
#define SKIN_LED PD5
#define EMG_LED PD6
#define PULSE_LED PD7

int main(void) {
    // set LED outputs
    DDRD |= (1 << PRESSURE_LED) | (1 << EMG_LED) | (1 << SKIN_LED);
    DDRB |=  (1 << PULSE_LED);

    ADMUX = (1 << REFS0);
    ADMUX &= ~(1 << REFS1);
    ADCSRA = (1 << ADEN) | (7 << ADPS0); // enable ADC and set prescalar to 128

    while (1) {
        uint16_t pressure = get_adc(PRESSURE_CH);
        uint16_t emg = get_adc(EMG_CH);
        uint16_t skin = get_adc(SKIN_CH);
        uint16_t pulse = get_adc(PULSE_CH);

        if (pressure > 500) {
            PORTD |= (1 << PRESSURE_LED);
        } else {
            PORTD &= ~(1 << PRESSURE_LED);
        }

        if (emg > 500) {
            PORTD |= (1 << EMG_LED);
        } else {
            PORTD &= ~(1 << EMG_LED);
        }

        if (skin > 500) {
            PORTD |= (1 << SKIN_LED);
        } else {
            PORTD &= ~(1 << SKIN_LED);
        }

        if (pulse > 500) {
            PORTB |= (1 << PULSE_LED);
        } else {
            PORTB &= ~(1 << PULSE_LED);
        }

        _delay_ms(50);
    }
}