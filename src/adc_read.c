// read values of biometric signals
#include "adc_read.h"
#include <avr/io.h>
#include <util/delay.h>

void adc_init() {
    ADMUX = (1 << REFS0);
    ADMUX &= ~(1 << REFS1);
    ADCSRA = (1 << ADEN) | (7 << ADPS0); // enable ADC and set prescalar to 128
    DIDR0 |= (1 << ADC1D) | (1 << ADC2D) | (1 << ADC3D) | (1 << ADC4D); // disable digital input buffers
}

uint16_t get_adc(uint8_t channel) {
    // ADMUX = (channel << MUX0);
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);

    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    (void)ADC;

    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return ADC;
}