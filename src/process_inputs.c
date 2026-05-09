// take analogue sensor input values and process them (filter and process biometric inputs then map to MIDI CC message)

#include "process_inputs.h"
#include <avr/io.h>
#include <util/delay.h>

uint8_t MIDI_map_value(uint8_t sensor, uint16_t sensor_val) {
    switch (sensor) {
        case PRESSURE:
            if (sensor_val < MIN_PRESSURE_THRESHOLD) {
                sensor_val = MIN_PRESSURE_THRESHOLD;
            }
            break;
        case SKIN:
            if (sensor_val < MIN_SKIN_THRESHOLD) {
                sensor_val = MIN_SKIN_THRESHOLD;
            }
            break;
        default:
            break;
    }
    
    // proportional conversion from 0-1023 to 0-127
    return (uint8_t)((sensor_val * 127UL) / 1023UL);
}