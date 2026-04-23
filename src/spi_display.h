// Code that deals with initialising and displaying (and switching between?) information on the SPI display

#ifndef SPI_DISPLAY_H
#define SPI_DISPLAY_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    Raw = 0,
    Biometrics = 1,
    MIDI = 2
} ScreenState;

typedef struct {
    // raw inputs
    uint16_t pulse_raw;
    uint16_t pressure_raw;
    uint16_t emg_raw;
    uint16_t gsr_raw;

    // processed biometrics
    uint16_t pulse_proc;
    uint16_t pressure_proc;
    uint16_t emg_proc;
    uint16_t gsr_proc;

    // midi messages
    uint8_t pulse_midi;
    uint8_t pressure_midi;
    uint8_t emg_midi;
    uint8_t gsr_midi;
} ScreenInfo;

void spi_init(void);
void spi_display_init(void);
void spi_display_clear(uint16_t colour);

void spi_display_draw_pixel(uint16_t x, uint16_t y, uint16_t colour);
void spi_display_draw_char(uint16_t x, uint16_t y, char c, uint16_t fg, uint16_t bg);
void spi_display_draw_string(uint16_t x, uint16_t y, const char* s, uint16_t fg, uint16_t bg);

void spi_display_draw_raw(const ScreenInfo* s_info);
void spi_display_draw_biometrics(const ScreenInfo* s_info);
void spi_display_draw_midi(const ScreenInfo* s_info);

#endif