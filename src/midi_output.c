// take MIDI messages and send them out via UART

#include "midi_output.h"
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

MIDI_Message get_MIDI_message(int sensor, int val) {
    MIDI_Message msg;

    switch (sensor) {
        case PRESSURE:
            msg.length = 3;
            msg.data[0] = MIDI_CC | 0x00;
            msg.data[1] = 1;
            msg.data[2] = val;
            break;
        case SKIN:
            msg.length = 3;
            msg.data[0] = MIDI_CC | 0x01;
            msg.data[1] = 2;
            msg.data[2] = val;
            break;
        case EMG:
            msg.length = 3;
            msg.data[0] = MIDI_NOTE | 0x00;
            msg.data[1] = MIDI_NOTE_NUM;
            msg.data[2] = val;
            break;
        default:
            msg.length = 1;
            msg.data[0] = MIDI_CLOCK;
            break;
    }

    return msg;
}

void send_MIDI(const MIDI_Message* msg) {
    int i;
    for (i = 0; i < msg->length; ++i) {
        send_UART(msg->data[i]);
    }
}