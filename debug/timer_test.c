#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


#define TEST_LED PB0
#define TARGET_FREQ 100
#define PRESCALAR 256
#define F_CLOCK 7372800

volatile uint32_t ms_count = 0;

static void timer1_init(void) {
    TCCR1B |= (1 << WGM12); // set CTC mode
    TIMSK1 |= (1 << OCIE1A); // enable CTC interrupt
    sei(); // enable global interrupt
    OCR1A = (F_CLOCK / (PRESCALAR * TARGET_FREQ));
    TCCR1B |= (1 << CS12); // set prescalar to 256 and start timer
}

ISR(TIMER1_COMPA_vect) {
    ms_count += 10;
}

int main(void) {
    // set ADC debug LED output
    DDRB |= (1 << TEST_LED);    

    timer1_init();

    uint16_t sensor_in;

    while (1) {
        if (ms_count > 1000) {
            PORTB |= (1 << TEST_LED);
            ms_count = 0;
        } else {
            PORTB &= ~(1 << TEST_LED);
        }
    }
}