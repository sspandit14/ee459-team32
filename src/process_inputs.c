// take analogue sensor input values and process them (filter and process biometric inputs then map to MIDI CC message)

#include <avr/io.h>
#include <util/delay.h>

#define PRESSURE 1
#define SKIN 2
#define EMG 3

#define MIDI_CLOCK 0xF8
#define MIDI_CC 0xB0
#define PRESSURE_CC_CH 1
#define SKIN_CC_CH 2
#define PRESSURE_CC_CONTROLLER 1
#define SKIN_CC_CONTROLLER 2
#define MIDI_NOTE 0x90
#define MIDI_NOTE_NUM 1

struct MIDI_Message {
    uint_8t length; // if length > 1, then not MIDI CLOCK
    uint_8t type;
    uint_8t data[2]; // unused in MIDI CLOCK
}

MIDI_Message get_MIDI_message(int sensor, int val) {
    MIDI_Message msg;

    switch (sensor) {
        case PRESSURE:
            msg.length = 3;
            msg.type = MIDI_CC + PRESSURE_CC_CH;
            msg.data[0] = PRESSURE_CC_CONTROLLER;
            msg.data[1] = val;
            break;
        case SKIN:
            msg.length = 3;
            msg.type = MIDI_CC + SKIN_CC_CH;
            msg.data[0] = SKIN_CC_CONTROLLER;
            msg.data[1] = val;
            break;
        case EMG:
            msg.length = 3;
            msg.type = MIDI_NOTE;
            msg.data[0] = MIDI_NOTE_NUM;
            msg.data[1] = val;
            break;
        default:
            printf("Not a Valid Input Sensor Channel");
            break;
    }

    return msg;
}

uint_8t MIDI_map_value(int sensor, int sensor_val) {
    switch (sensor) {
        case PRESSURE:
            break;
        case SKIN:
            break;
        case EMG:
            break;
        case default:
            break;
    }
}