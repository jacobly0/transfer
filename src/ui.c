#include "font.h"
#include "spi.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <tice.h>

#define STATIC_ROWS 4

static uint8_t row, col, swap;
#define buffer(n) (*(uint8_t (*)[4][LCD_WIDTH][LCD_HEIGHT >> 1])lcd_Ram)[(n) ^ swap]

void ui_Init(void) {
    os_DisableAPD();
    spi(DISPLAY_OFF);
    spi(SET_COL_ADDR,  spi16(0), spi16(LCD_HEIGHT - 1));
    spi(SET_PAGE_ADDR, spi16(0), spi16(LCD_WIDTH - 1));
    spi(MEM_ACC_CTL,   0x2C);
    *(volatile uint8_t *)&lcd_Control = 0x25;
    *(volatile uint8_t *volatile *)&lcd_UpBase = &buffer(0)[0][0];
    for (uint8_t i = 0; i <= 15; ++i) {
        uint8_t component = 63 * (i ^ 15) / 15;
        lcd_Palette[i] =
            (component &  1) << 15 |
            (component >> 1) << 10 |
            (component >> 1) <<  5 |
            (component >> 1) <<  0;
    }
    memset(buffer(0), 0, sizeof(buffer(0)));
    spi(DISPLAY_ON);
}

void ui_Cleanup(void) {
    spi(DISPLAY_OFF);
    delay(1000 / 64);
    *(volatile uint16_t *volatile *)&lcd_UpBase = lcd_Ram;
    *(volatile uint8_t *)&lcd_Control = 0x2D;
    spi(SET_COL_ADDR,  spi16(0), spi16(LCD_WIDTH - 1));
    spi(SET_PAGE_ADDR, spi16(0), spi16(LCD_HEIGHT - 1));
    spi(MEM_ACC_CTL,   0x08);
    memset((uint16_t *)lcd_Ram, 0xFF, LCD_SIZE);
    spi(DISPLAY_ON);
}

void outchar(char c) {
    switch (c) {
        case '\n':
            break;
        default:
            for (uint8_t x = 0; x != FONT_WIDTH; ++x)
                memcpy(&buffer(0)[col * FONT_WIDTH + x + LCD_WIDTH % FONT_WIDTH / 2]
                       [row * FONT_HEIGHT_BYTES], font[(uint8_t)c][x], FONT_HEIGHT_BYTES);
            if (++col != LCD_WIDTH / FONT_WIDTH)
                return;
            break;
    }
    if (++row == LCD_HEIGHT / FONT_HEIGHT) {
        --row;
        swap ^= 1;
        while (!(lcd_IntStatus & 1 << 2));
        memcpy(&buffer(0)[0][0], &buffer(1)[0][0] + FONT_HEIGHT_BYTES,
               sizeof(buffer(1)) - FONT_HEIGHT_BYTES);
        for (unsigned x = 0; x != LCD_WIDTH; ++x) {
            memcpy(&buffer(0)[x][0], &buffer(1)[x][0],
                   FONT_HEIGHT_BYTES * STATIC_ROWS);
            memset(&buffer(0)[x][((LCD_HEIGHT / FONT_HEIGHT - 1) *
                                  FONT_HEIGHT) >> 1], 0, FONT_HEIGHT_BYTES);
        }
        *(volatile uint8_t *volatile *)&lcd_UpBase = &buffer(0)[0][0];
        lcd_IntAcknowledge = 1 << 2;
    }
    col = 0;
}
