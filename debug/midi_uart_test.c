#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 7372800UL
#define BAUD_RATE 31250

void init_UART(void) {
    uint16_t ubrr = (F_CPU / (16UL * BAUD_RATE)) - 1;

    UBRR0H = (ubrr >> 8);
    UBRR0L = ubrr;

    UCSR0B = (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void send_UART(uint8_t data) {
    while(!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

int main(void) {
    init_UART();

    while(1) {
        // send_UART(0xF8); // MIDI Clock

        // MIDI CC
        // send_UART(0xB0);
        // send_UART(0x01);
        // send_UART(0x40);

        // _delay_ms(100);

        send_UART(0x90);
        send_UART(60);
        send_UART(100);

        _delay_ms(1000);

        send_UART(0x80);
        send_UART(60);
        send_UART(0);

        _delay_ms(1000);
    }
}