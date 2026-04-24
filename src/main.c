// main code/integration script
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>

#include "adc_read.h"
#include "process_inputs.h"
#include "midi_output.h"
#include "spi_display.h"

#define DEFAULT_RAW_VAL 200
#define MIN_VALID_SENSOR_VAL 100
#define MAX_VALID_SENSOR_VAL 1000

#define PULSE_HIGH 600
#define PULSE_LOW 400

#define SENSOR_READ_MS 10
#define DISPLAY_UPDATE_MS 100
#define MIDI_UPDATE_MS 10

static ScreenState current_screen = Raw;
static ScreenInfo screen_info;

volatile uint32_t ms_counter = 0;
static uint32_t last_sensor_read = 0;
static uint32_t last_display_update = 0;
static uint32_t last_midi_update = 0;

static uint32_t last_pulse_time = 0;
static uint32_t midi_clk_spacing_ms = 0; // time between MIDI clock messages

static uint16_t last_pulse_val = 0;
static uint16_t bpm = 60;

static uint32_t last_clock_time = 0;

static void timer1_init(void) {
    // Clear Timer on Compare Match (CTC) mode
    TCCR1A = 0;
    TCCR1B = (1 << WGM12);

    TCCR1B |= (1 << CS11) | (1 << CS10); // Prescalar  64
    OCR1A = 153; // (F_CPU = 9830400 / 64 / 1000) - 1

    TIMSK1 = (1 << OCIE1A);
}

ISR(TIMER1_COMPA_vect) {
    ++ms_counter;
}

static bool is_valid_sensor_read(uint16_t val) {
    return (MIN_VALID_SENSOR_VAL <= val && val <= MAX_VALID_SENSOR_VAL);
}

static uint16_t fix_sensor_read(uint16_t val) {
    if (!is_valid_sensor_read(val)) {
        return DEFAULT_RAW_VAL;
    }

    return val;
}

static void update_raw_vals(void) {
    screen_info.pressure_raw = fix_sensor_read(get_adc(PRESSURE_CH));
    screen_info.gsr_raw = fix_sensor_read(get_adc(SKIN_CH));
    screen_info.emg_raw = fix_sensor_read(get_adc(EMG_CH));
    screen_info.pulse_raw = fix_sensor_read(get_adc(PULSE_CH));
}

static void update_processed_vals(void) {
    screen_info.pressure_proc = MIDI_map_value(PRESSURE, screen_info.pressure_raw);
    screen_info.gsr_proc = MIDI_map_value(SKIN, screen_info.gsr_raw);
    screen_info.emg_proc = MIDI_map_value(EMG, screen_info.emg_raw);
    screen_info.pulse_proc = bpm;
}

static void update_pulse(uint16_t pulse_val, uint32_t now) {
    // Rising edge
    if (pulse_val > PULSE_HIGH && last_pulse_val <= PULSE_HIGH) {
        uint32_t t_interval = now - last_pulse_time;
        last_pulse_time = now;

        if (t_interval > 300 && t_interval < 2000) {
            uint16_t updated_bpm = 60000 / t_interval;

            bpm = (bpm * 3 + updated_bpm) / 4; // interpolate between old and new bpm to reduce jumps

            midi_clk_spacing_ms = 60000 / (bpm * 24);
        }
    }

    last_pulse_val = pulse_val;
}

static void update_midi_clk(uint32_t now) {
    if (now - last_clock_time >= midi_clk_spacing_ms) {
        last_clock_time = now;
        send_UART(MIDI_CLOCK);
    }
}

static void update_midi_out(void) {
    screen_info.pressure_midi = screen_info.pressure_proc;
    screen_info.gsr_midi = screen_info.gsr_proc;
    screen_info.emg_midi = screen_info.emg_proc;

    MIDI_Message msg;

    msg = get_MIDI_message(PRESSURE, screen_info.pressure_midi);
    send_MIDI(&msg);
    msg = get_MIDI_message(SKIN, screen_info.gsr_midi);
    send_MIDI(&msg);
    msg = get_MIDI_message(EMG, screen_info.emg_midi);
    send_MIDI(&msg);

    // TODO: figure out pulse logic
}

static void update_display(void) {
    switch (current_screen) {
        case Raw:
            spi_display_draw_raw(&screen_info);
            break;
        case Biometrics:
            spi_display_draw_biometrics(&screen_info);
            break;
        case MIDI:
            spi_display_draw_midi(&screen_info);
            break;
        default:
            spi_display_draw_raw(&screen_info);
            break;
    }
}

int main(void) {
    adc_init();
    init_UART();
    spi_display_init();
    timer1_init();
    sei();

    current_screen = Raw;

    while (1) {
        uint32_t now;
        cli();
        now = ms_counter;
        sei();

        if (now - last_sensor_read >= SENSOR_READ_MS) {
            last_sensor_read = now;
            update_raw_vals();
            update_processed_vals();
            update_pulse(screen_info.pulse_raw, now);
        }

        if (now - last_midi_update >= MIDI_UPDATE_MS) {
            last_midi_update = now;
            update_midi_out();
        }

        update_midi_clk(now);

        if (now - last_display_update >= DISPLAY_UPDATE_MS) {
            last_display_update = now;
            update_display();
        }
    }
}