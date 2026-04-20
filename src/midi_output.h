#ifndef MIDI_OUTPUT_H
#define MIDI_OUTPUT_H

#include <stdint.h>
#include <stdbool.h>

#define BAUD_RATE 31250

#define PRESSURE 1
#define SKIN 2
#define EMG 3

#define MIDI_CLOCK 0xF8
#define MIDI_CC 0xB0
#define MIDI_NOTE 0x90
#define MIDI_NOTE_NUM 60

typedef struct {
    uint8_t length; // if length > 1, then not MIDI CLOCK
    uint8_t data[3]; // 0: type, 1: data1, 2: data2
} MIDI_Message;

void init_UART(void);
void send_UART(uint8_t);
MIDI_Message get_MIDI_message(int sensor, int val);
void send_MIDI(const MIDI_Message* msg);

#endif