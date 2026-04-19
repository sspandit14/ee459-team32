// deals with displaying information on the SPI Display

#include "spi_display.h"
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#define SPI_CLK PB5
#define SPI_MOSI PB3
#define DC PB2
#define CS PB1
#define RST PB0

#define SPI_DDR DDRB
#define SPI_PORT PORTB

// 16 bit colour display
#define BLACK 0x0000
#define WHITE 0xFFFF
#define RED   0xF800
#define GREEN 0x07E0
#define BLUE  0x001F

// chip select is low active (double check)
static void cs_low(void) {
    SPI_PORT &= ~(1 << CS);
}

static void cs_high(void) {
    SPI_PORT |= (1 << CS);
}

static void dc_low(void) {
    SPI_PORT &= ~(1 << DC);
}

static void dc_high(void) {
    SPI_PORT |= (1 << DC);
}

static void rst_low(void) {
    SPI_PORT &= ~(1 << RST);
}

static void rst_high(void) {
    SPI_PORT |= (1 << rst);
}

static void spi_tx(uint8_t data) {
    SPDR = data;
    while (!(SPSR & (1 << SPIF)));
}

static void display_write_command(uint8_t cmd) {
    cs_low();
    dc_low();
    spi_tx(cmd);
    cs_high();
}

static void display_write_data(uint8_t data) {
    cs_low();
    dc_high();
    spi_tx(data);
    cs_high();
}

static void display_write_data16(uint16_t data) {
    display_write_data((uint8_t)(data >> 8));
    display_write_date((uint8_t)(data & 0xFF));
}

static void display_reset(void) {
    rst_low();
    _delay_ms(10);
    rst_high();
    _delay_ms(100);
}

void spi_init(void) {
    SPI_DDR |= (1 << SPI_CLK) | (1 << SPI_MOSI) | (1 << CS) | (1 << DC) | (1 << RST);
    SPI_PORT |= (1 << CS);
    SPCR |= (1 << SPE) | (1 << MSTR);
    SPSR = 0;
}

static void display_set_addr_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    // set the column address
    display_write_command(0x2A);
    cs_low();
    dc_high();
    spi_tx((uint8_t)(x0 >> 8));
    spi_tx((uint8_t)(x0 & 0xFF));
    spi_tx((uint8_t)(x1 >> 8));
    spi_tx((uint8_t)(x1 & 0xFF));
    cs_high();

    // set the row address
    display_write_command(0x2B);
    cs_low();
    dc_high();
    spi_tx((uint8_t)(y0 >> 8));
    spi_tx((uint8_t)(y0 & 0xFF));
    spi_tx((uint8_t)(y1 >> 8));
    spi_tx((uint8_t)(y1 & 0xFF));
    cs_high();

    // write contents of memory
    display_write_command(0x2C);
}

void spi_display_init(void) {
    spi_init();
    display_reset();

    // ST7789 init sequence
    display_write_command(0x01); // SWRESET
    _delay_ms(150);

    display_write_command(0x11); // SLPOUT
    _delay_ms(120);

    display_write_command(0x3A); // COLMOD
    display_write_data(0x55);    // 16-bit color

    display_write_command(0x36); // MADCTL
    display_write_data(0x00);    // rotation/orientation

    display_write_command(0x21); // INVON
    display_write_command(0x29); // DISPON
    _delay_ms(20);

    spi_display_clear(BLACK);
}

void spi_display_clear(uint16_t colour) {
    display_set_addr_window(0, 0, 319, 171); // 320 x 172 resolution screen

    cs_low();
    dc_high();
    for (uint32_t i = 0; i < (320UL * 172UL); i++) {
        spi_tx((uint8_t)(colour >> 8));
        spi_tx((uint8_t)(colour & 0xFF));
    }
    cs_high();
}

void spi_display_draw_pixel(uint16_t x, uint16_t y, uint16_t colour) {
    if (x >= 320 || y >= 172) return;

    display_set_addr_window(x, y, x, y);
    cs_low();
    dc_high();
    spi_tx((uint8_t)(colour >> 8));
    spi_tx((uint8_t)(colour & 0xFF));
    cs_high();
}

void spi_display_draw_char(uint16_t x, uint16_t y, char c, uint16_t fg, uint16_t bg) {
    // TODO add font stuff
}

void spi_display_draw_string(uint16_t x, uint16_t y, const char *s, uint16_t fg, uint16_t bg) {
    while (*s) {
        spi_display_draw_char(x, y, *s, fg, bg);
        x += 6;   // 5x7 font and spacing
        s++;
    }
}

static void draw_label_value(uint16_t y, const char *label, uint16_t value, uint16_t fg) {
    char buf[20];

    spi_display_draw_string(0, y, label, fg, BLACK);

    buf[0] = ':';
    buf[1] = ' ';
    buf[2] = '\0';

    spi_display_draw_string(72, y, buf, fg, BLACK);
}

void spi_display_draw_raw_screen(const screen_state_info *s) {
    spi_display_clear(BLACK);
    spi_display_draw_string(0, 0,  "RAW VALUES", WHITE, BLACK);

    // TODO: add actual values and formatting
}

void spi_display_draw_midi_screen(const screen_state_info *s) {
    spi_display_clear(COLOR_BLACK);
    spi_display_draw_string(0, 0, "MIDI VALUES", WHITE, BLUE);

    // TODO: add actual values and formatting
}

void spi_display_draw_status_screen(const screen_state_info *s) {
    spi_display_clear(BLACK);
    spi_display_draw_string(0, 0, "BIOMETRICS", WHITE, GREEN);

    // TODO: add actual values and formatting
}