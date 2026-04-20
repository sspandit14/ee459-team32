#ifndef ADC_READ_H
#define ADC_READ_H

#include <stdint.h>

#define PRESSURE_CH 1
#define SKIN_CH 2
#define EMG_CH 3
#define PULSE_CH 4

void adc_init();
uint16_t get_adc(uint8_t channel);

#endif