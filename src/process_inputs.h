#ifndef PROC_INPUT_H
#define PROC_INPUT_H

#include <stdint.h>

#define PRESSURE 1
#define SKIN 2
#define EMG 3

#define MIN_PRESSURE_THRESHOLD 100
#define MIN_SKIN_THRESHOLD 300

uint8_t MIDI_map_value(uint8_t sensor, uint16_t sensor_val);

#endif