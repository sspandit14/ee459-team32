// take analogue sensor input values and process them (filter and process biometric inputs then map to MIDI CC message)

#include "process_inputs.h"
#include <avr/io.h>
#include <util/delay.h>

uint8_t MIDI_map_value(uint8_t sensor, uint16_t sensor_val) {
    // placeholder conversion
    return (uint8_t)((sensor_val * 127UL) / 1023UL);

    switch (sensor) {
        case PRESSURE:
            break;
        case SKIN:
            break;
        case EMG:
            break;
        default:
            break;
    }
}