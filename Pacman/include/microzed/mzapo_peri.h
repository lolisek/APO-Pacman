#ifndef MZAPO_PERI_H
#define MZAPO_PERI_H

#include <stdint.h>
#include <stdbool.h>
#include "../microzed/mzapo_regs.h"
#include "../microzed/mzapo_phys.h"
#include <stdlib.h>

#define KNOB_CLICK_DELAY_MS 100000
#define KNOB_ROTATION_DELAY_MS 100000


#define KNOB_CLICKS_PER_TURN 4
#define KNOB_POSITIONS 256
#define KNOB_CLICK_SIZE (KNOB_POSITIONS/KNOB_CLICKS_PER_TURN)

void mzapo_setup(void);

void *get_mzapo_spiled(void);
void *get_mzapo_parlcd(void);

void set_rgb1(uint8_t r, uint8_t g, uint8_t b);
volatile uint32_t get_rgb1(void);
bool rgb1_is_on(void);

void set_rgb2(uint8_t r, uint8_t g, uint8_t b);
volatile uint32_t get_rgb2(void);
bool rgb2_is_on(void);

void set_led_on(uint8_t index);
void set_all_leds_on(void);
void set_led_off(uint8_t index);
void set_all_leds_off(void);
bool led_is_on(uint8_t index);

void lcd_update(uint16_t *fb);

uint8_t get_red_knob_rotation(void);
uint8_t get_green_knob_rotation(void);
uint8_t get_blue_knob_rotation(void);

int8_t get_red_knob_delta(int16_t *last_red);
int8_t get_green_knob_delta(int16_t *last_green);
int8_t get_blue_knob_delta(int16_t *last_blue);

bool red_knob_is_pressed(void);
bool green_knob_is_pressed(void);
bool blue_knob_is_pressed(void);

uint8_t compute_rotation_delta(uint8_t *last_value, uint8_t current);

void set_knob_pressed(uint8_t index);
void set_knob_released(uint8_t index);

#endif