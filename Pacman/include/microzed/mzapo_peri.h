#ifndef MZAPO_PERI_H
#define MZAPO_PERI_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "../microzed/mzapo_regs.h"
#include "../microzed/mzapo_phys.h"

/**
 * @brief Number of clicks per full turn of the knob.
 */
#define KNOB_CLICKS_PER_TURN 4

/**
 * @brief Total number of positions for the knob.
 */
#define KNOB_POSITIONS 256

/**
 * @brief Size of each click in terms of knob positions.
 */
#define KNOB_CLICK_SIZE (KNOB_POSITIONS / KNOB_CLICKS_PER_TURN)

/**
 * @brief Sets up the peripherals for the MZAPO board.
 */
void mzapo_setup(void);

/**
 * @brief Retrieves the SPI LED peripheral.
 *
 * @return Pointer to the SPI LED peripheral.
 */
void *get_mzapo_spiled(void);

/**
 * @brief Retrieves the parallel LCD peripheral.
 *
 * @return Pointer to the parallel LCD peripheral.
 */
void *get_mzapo_parlcd(void);

/**
 * @brief Sets the RGB1 LED color.
 *
 * @param r Red component (0-255).
 * @param g Green component (0-255).
 * @param b Blue component (0-255).
 */
void set_rgb1(uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief Retrieves the current RGB1 LED color.
 *
 * @return Current RGB1 LED color as a 32-bit value.
 */
volatile uint32_t get_rgb1(void);

/**
 * @brief Checks if the RGB1 LED is on.
 *
 * @return true if the RGB1 LED is on, false otherwise.
 */
bool rgb1_is_on(void);

/**
 * @brief Sets the RGB2 LED color.
 *
 * @param r Red component (0-255).
 * @param g Green component (0-255).
 * @param b Blue component (0-255).
 */
void set_rgb2(uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief Retrieves the current RGB2 LED color.
 *
 * @return Current RGB2 LED color as a 32-bit value.
 */
volatile uint32_t get_rgb2(void);

/**
 * @brief Checks if the RGB2 LED is on.
 *
 * @return true if the RGB2 LED is on, false otherwise.
 */
bool rgb2_is_on(void);

/**
 * @brief Turns on a specific line LED.
 *
 * @param index Index of the LED to turn on.
 */
void set_led_on(uint8_t index);

/**
 * @brief Turns on all line LEDs.
 */
void set_all_leds_on(void);

/**
 * @brief Turns off a specific line LED.
 *
 * @param index Index of the LED to turn off.
 */
void set_led_off(uint8_t index);

/**
 * @brief Turns off all line LEDs.
 */
void set_all_leds_off(void);

/**
 * @brief Checks if a specific line LED is on.
 *
 * @param index Index of the LED to check.
 * @return true if the LED is on, false otherwise.
 */
bool led_is_on(uint8_t index);

/**
 * @brief Updates the LCD with the given framebuffer.
 *
 * @param fb Pointer to the framebuffer.
 */
void lcd_update(uint16_t *fb);

/**
 * @brief Retrieves the current rotation of the red knob.
 *
 * @return Current rotation value of the red knob.
 */
uint8_t get_red_knob_rotation(void);

/**
 * @brief Retrieves the current rotation of the green knob.
 *
 * @return Current rotation value of the green knob.
 */
uint8_t get_green_knob_rotation(void);

/**
 * @brief Retrieves the current rotation of the blue knob.
 *
 * @return Current rotation value of the blue knob.
 */
uint8_t get_blue_knob_rotation(void);

/**
 * @brief Computes the delta in rotation for the red knob.
 *
 * @param last_red Pointer to the last recorded rotation value.
 * @return Delta in rotation.
 */
int8_t get_red_knob_delta(int16_t *last_red);

/**
 * @brief Computes the delta in rotation for the green knob.
 *
 * @param last_green Pointer to the last recorded rotation value.
 * @return Delta in rotation.
 */
int8_t get_green_knob_delta(int16_t *last_green);

/**
 * @brief Computes the delta in rotation for the blue knob.
 *
 * @param last_blue Pointer to the last recorded rotation value.
 * @return Delta in rotation.
 */
int8_t get_blue_knob_delta(int16_t *last_blue);

/**
 * @brief Checks if the red knob is pressed.
 *
 * @return true if the red knob is pressed, false otherwise.
 */
bool red_knob_is_pressed(void);

/**
 * @brief Checks if the green knob is pressed.
 *
 * @return true if the green knob is pressed, false otherwise.
 */
bool green_knob_is_pressed(void);

/**
 * @brief Checks if the blue knob is pressed.
 *
 * @return true if the blue knob is pressed, false otherwise.
 */
bool blue_knob_is_pressed(void);

/**
 * @brief Computes the delta in rotation between the last and current values.
 *
 * @param last_value Pointer to the last recorded value.
 * @param current Current rotation value.
 * @return Delta in rotation.
 */
uint8_t compute_rotation_delta(uint8_t *last_value, uint8_t current);

#endif // MZAPO_PERI_H