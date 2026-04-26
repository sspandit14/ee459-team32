#include <avr/io.h>
#include <util/delay.h>

#define SPI_CLK PB5
#define SPI_MOSI PB3
#define SPI_DC PB2
#define SPI_CS PB1
#define SPI_RST PB0

#define HIGH 1
#define LOW 0

// 16 bit colour display
#define BLACK 0x0000
#define WHITE 0xFFFF
#define RED   0xF800
#define GREEN 0x07E0
#define BLUE  0x001F

static void display_set_addr_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

static void cs_low(void) {
    PORTB &= ~(1 << SPI_CS);
}

static void cs_high(void) {
    PORTB |= (1 << SPI_CS);
}

static void dc_low(void) {
    PORTB &= ~(1 << SPI_DC);
}

static void dc_high(void) {
    PORTB |= (1 << SPI_DC);
}

static void rst_low(void) {
    PORTB &= ~(1 << SPI_RST);
}

static void rst_high(void) {
    PORTB |= (1 << SPI_RST);
}
static void spi_init(void) {
    DDRB |= (1 << SPI_CLK) | (1 << SPI_MOSI) | (1 << SPI_DC) | (1 << SPI_CS) | (1 << SPI_RST);
    DDRB |= (1 << PB2); // enable SS as output
    PORTB |= (1 << SPI_CS) | (1 << SPI_DC) | (1 << SPI_RST);
    SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

static void display_reset(void) {
    rst_low();
    _delay_ms(100);
    rst_high();
    _delay_ms(100);
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

void spi_display_clear(uint16_t colour) {
    display_set_addr_window(0, 0, 319, 171); // 320 x 172 resolution screen

    cs_low();
    dc_high();

    uint32_t i;
    for (i = 0; i < (320UL * 172UL); i++) {
        spi_tx((uint8_t)(colour >> 8));
        spi_tx((uint8_t)(colour & 0xFF));
    }
    cs_high();
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

    // memory write
    display_write_command(0x2C);
}

static void spi_display_init(void) {
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

int main(void) {
    spi_display_init();

    while (1) {
        spi_display_clear(RED);
        _delay_ms(1000);
        spi_display_clear(BLUE);
    }
}