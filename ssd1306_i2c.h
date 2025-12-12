/*
 * Plataforma: detecção automática
 *  - RP Pico / RP2040: macros do Pico SDK
 *  - STM32F411RE / STM32F429ZI: macros do HAL STM32
 */

#ifndef ssd1306_i2c_h
#define ssd1306_i2c_h

#include <stdlib.h>
#include <stdint.h>

/* Detecção de plataforma baseada em macros do SDK */
#if defined(PICO_SDK_VERSION_MAJOR) || defined(PICO_BOARD) || defined(RP2040)
  #define SSD1306_PLATFORM_PICO
#elif defined(STM32F411xE) || defined(STM32F429xx)
  #define SSD1306_PLATFORM_STM32
#endif

/* Includes específicos de plataforma */
#if defined(SSD1306_PLATFORM_PICO)
  #include "pico/stdlib.h"
  #include "hardware/i2c.h"
#elif defined(SSD1306_PLATFORM_STM32)
  /*
   * Para STM32, assumimos uso do HAL.
   * O handle de I2C (ex.: hi2c1) é declarado em main.c ou em outro módulo
   * de inicialização gerado pelo STM32CubeMX.
   */
  #include "stm32f4xx_hal.h"
  #include "stm32f4xx_hal_i2c.h"
#endif

/*
 * Macros auxiliares usados originalmente pelo código do Pico (_u, count_of).
 * Em ambientes sem Pico SDK, definimos versões genéricas.
 */
#if !defined(SSD1306_PLATFORM_PICO)
  #ifndef _u
    #define _u(x) (x)
  #endif
  #ifndef count_of
    #define count_of(x) (sizeof(x) / sizeof((x)[0]))
  #endif
#endif

#ifndef ssd1306_height
#define ssd1306_height 64 // Define a altura do display (32 pixels)
#endif
#ifndef ssd1306_width
#define ssd1306_width 128 // Define a largura do display (128 pixels)
#endif
#if ssd1306_height == 64
#define max_text_lines 8
#elif  ssd1306_height == 32
#define max_text_lines 4
#endif
#define max_text_columns 17
#define ssd1306_line_height 8 // Define a altura de uma linha (8 pixels) - 

void ssd1306_draw_string(uint8_t *ssd, int16_t x, int16_t y, char *string);
void ssd1306_draw_big_char(uint8_t *ssd, int16_t x, int16_t y, uint8_t character);
bool ssd1306_set_i2c_address(uint8_t address);
uint8_t ssd1306_get_i2c_address(void);

#define ssd1306_i2c_address ssd1306_get_i2c_address() // Define o endereço do i2c do display

#define ssd1306_i2c_clock 400 // Define o tempo do clock (pode ser aumentado)

// Comandos de configuração (endereços)
#define ssd1306_set_memory_mode _u(0x20)
#define ssd1306_set_column_address _u(0x21)
#define ssd1306_set_page_address _u(0x22)
#define ssd1306_set_horizontal_scroll _u(0x26)
#define ssd1306_set_scroll _u(0x2E)

#define ssd1306_set_display_start_line _u(0x40)

#define ssd1306_set_contrast _u(0x81)
#define ssd1306_set_charge_pump _u(0x8D)

#define ssd1306_set_segment_remap _u(0xA0)
#define ssd1306_set_entire_on _u(0xA4)
#define ssd1306_set_all_on _u(0xA5)
#define ssd1306_set_normal_display _u(0xA6)
#define ssd1306_set_inverse_display _u(0xA7)
#define ssd1306_set_mux_ratio _u(0xA8)
#define ssd1306_set_display _u(0xAE)
#define ssd1306_set_common_output_direction _u(0xC0)
#define ssd1306_set_common_output_direction_flip _u(0xC0)

#define ssd1306_set_display_offset _u(0xD3)
#define ssd1306_set_display_clock_divide_ratio _u(0xD5)
#define ssd1306_set_precharge _u(0xD9)
#define ssd1306_set_common_pin_configuration _u(0xDA)
#define ssd1306_set_vcomh_deselect_level _u(0xDB)

#define ssd1306_page_height _u(8)
#define ssd1306_n_pages (ssd1306_height / ssd1306_page_height)
#define ssd1306_buffer_length (ssd1306_n_pages * ssd1306_width)

#define ssd1306_write_mode _u(0xFE)
#define ssd1306_read_mode _u(0xFF)

struct render_area {
    uint8_t start_column;
    uint8_t end_column;
    uint8_t start_page;
    uint8_t end_page;

    int buffer_length;
};

/*
 * Estrutura auxiliar para modo bitmap.
 * O tipo do ponteiro de porta I2C depende da plataforma:
 *  - Pico: i2c_inst_t*
 *  - STM32: I2C_HandleTypeDef* (uso típico: &hi2c2 como porta padrão)
 */
#if defined(SSD1306_PLATFORM_PICO)
typedef struct {
  uint8_t width, height, pages, address;
  i2c_inst_t * i2c_port;
  bool external_vcc;
  uint8_t *ram_buffer;
  size_t bufsize;
  uint8_t port_buffer[2];
} ssd1306_t;
#elif defined(SSD1306_PLATFORM_STM32)
typedef struct {
  uint8_t width, height, pages, address;
  I2C_HandleTypeDef * i2c_port;  /* exemplo de uso: &hi2c1 */
  bool external_vcc;
  uint8_t *ram_buffer;
  size_t bufsize;
  uint8_t port_buffer[2];
} ssd1306_t;
#endif

#endif // ssd1306_i2c_h