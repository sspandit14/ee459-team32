#include <avr/io.h>
#include <util/delay.h>


#define ADC_LED PD7
#define SENSOR_CHANNEL 4

static void adc_init(uint8_t channel) {
    ADMUX |= (1 << REFS0); // reference voltage to AVCC
    ADMUX &= ~(1 << REFS1);
    ADCSRA |= (7 << ADPS0); // set ADC prescalar to 128 (i.e. divide our MCU clock by 128)
    ADMUX |= (channel << MUX0);
    ADCSRA |= (1 << ADEN); // enable the ADC
}

int main(void) {
    // set ADC debug LED output
    DDRB |= (1 << ADC_LED);    

    adc_init(SENSOR_CHANNEL);

    uint16_t sensor_in;

    while (1) {
        ADCSRA |= (1 << ADSC); // start ADC conversion
        while (ADCSRA & (1 << ADSC)); // wait for conversion to finish
        sensor_in = ADCH;

        if (sensor_in > 300) {
            PORTB |= (1 << ADC_LED);
        } else {
            PORTB &= ~(1 << ADC_LED);
        }

        _delay_ms(100);
    }
}