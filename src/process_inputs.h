#ifndef PROC_INPUT_H
#define PROC_INPUT_H

#include <stdint.h>

#define PRESSURE 1
#define SKIN 2
#define EMG 3

uint8_t MIDI_map_value(uint8_t sensor, uint16_t sensor_val);

#endif