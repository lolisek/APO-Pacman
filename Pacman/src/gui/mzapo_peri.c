#include "../../include/gui/mzapo_peri.h"

static void *mzapo_spiled = NULL;
static void *mzapo_parlcd = NULL;

void *get_mzapo_spiled(void) { return mzapo_spiled; }
void *get_mzapo_parlcd(void) { return mzapo_parlcd; }

void mzapo_setup(void) {
    mzapo_spiled = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
    mzapo_parlcd = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
}

/* LED DIODES */

void set_rgb1(uint8_t r, uint8_t g, uint8_t b) {
    volatile uint32_t *adress = (volatile uint32_t *)(mzapo_spiled + SPILED_REG_LED_RGB1_o);
    *adress = r << 16 | g << 8 | b;
}

volatile uint32_t get_rgb1(void) {
    return *(volatile uint32_t *)(mzapo_spiled + SPILED_REG_LED_RGB1_o);
}

bool rgb1_is_on(void) {
    volatile uint32_t *adress = (volatile uint32_t *)(mzapo_spiled + SPILED_REG_LED_RGB1_o);
    return (bool)(*adress) > 0;
}


void set_rgb2(uint8_t r, uint8_t g, uint8_t b) {
    volatile uint32_t *adress = (volatile uint32_t *)(mzapo_spiled + SPILED_REG_LED_RGB2_o);
    *adress = r << 16 | g << 8 | b;
}

volatile uint32_t get_rgb2(void) {
    return *(volatile uint32_t *)(mzapo_spiled + SPILED_REG_LED_RGB2_o);
}

bool rgb2_is_on(void) {
    volatile uint32_t *adress = (volatile uint32_t *)(mzapo_spiled + SPILED_REG_LED_RGB2_o);
    return (bool)(*adress) > 0;
}


void set_led_on(uint8_t index) {
    volatile uint32_t *adress = (volatile uint32_t *)(mzapo_spiled + SPILED_REG_LED_LINE_o);
    *adress = *adress | (0x1 << index);
}

void set_all_leds_on() {
    volatile uint32_t *adress = (volatile uint32_t *)(mzapo_spiled + SPILED_REG_LED_LINE_o);
    *adress = 0xFFFFFFFF;
}


void set_led_off(uint8_t index) {
    volatile uint32_t *adress = (volatile uint32_t *)(mzapo_spiled + SPILED_REG_LED_LINE_o);
    *adress = !(*adress | (0x1 << index));
}

void set_all_leds_off() {
    volatile uint32_t *adress = (volatile uint32_t *)(mzapo_spiled + SPILED_REG_LED_LINE_o);
    *adress = 0;
}

bool led_is_on(uint8_t index) {
    volatile uint32_t *adress = (volatile uint32_t *)(mzapo_spiled + SPILED_REG_LED_LINE_o);
    return (bool) (*adress & (0x1 << index));
}

/* LCD DISPLAY*/

void lcd_update(uint16_t *fb) {
    volatile uint8_t  *cmd  =  (uint8_t *)(mzapo_parlcd + PARLCD_REG_CR_o);
    volatile uint16_t *data = (uint16_t*)(mzapo_parlcd + PARLCD_REG_DATA_o);

    //refresh command
    *cmd = 0x2C;                            

    for (uint32_t i = 0; i < 480*320; ++i)
        *data = *fb++;  

}

/* KNOBS*/

uint8_t get_red_knob_rotation(void) {
    volatile uint32_t *adress = (volatile uint32_t *)(mzapo_spiled + SPILED_REG_KNOBS_8BIT_o);
    return (uint8_t)(*adress >> 16) & 0xFF;
}

uint8_t get_green_knob_rotation(void) {
    volatile uint32_t *adress = (volatile uint32_t *)(mzapo_spiled + SPILED_REG_KNOBS_8BIT_o);
    return (uint8_t)(*adress >> 8) & 0xFF;
}

uint8_t get_blue_knob_rotation(void) {
    volatile uint32_t *adress = (volatile uint32_t *)(mzapo_spiled + SPILED_REG_KNOBS_8BIT_o);
    return (uint8_t)(*adress) & 0xFF;
}

uint8_t compute_rotation_delta(uint8_t *last_value, uint8_t current) {
    int16_t diff = (int16_t)current - (int16_t)(*last_value);

    *last_value = current;
    return (int8_t)diff;
}

int8_t get_red_knob_delta(int16_t *last_red) {   
    return compute_rotation_delta((uint8_t *)last_red, get_red_knob_rotation());
}

int8_t get_green_knob_delta(int16_t *last_green) {
    return compute_rotation_delta((uint8_t *)last_green, get_green_knob_rotation());
}

int8_t get_blue_knob_delta(int16_t *last_blue) {
    return compute_rotation_delta((uint8_t *)last_blue, get_blue_knob_rotation());
}

bool red_knob_is_pressed(void) {
    volatile uint32_t *adress = (volatile uint32_t *)(mzapo_spiled + SPILED_REG_KNOBS_8BIT_o);
    return (uint8_t)(*adress >> 26) & 0x1;
}

bool green_knob_is_pressed(void) {
    volatile uint32_t *adress = (volatile uint32_t *)(mzapo_spiled + SPILED_REG_KNOBS_8BIT_o);
    return (uint8_t)(*adress >> 25) & 0x1;
}

bool blue_knob_is_pressed(void) {
    volatile uint32_t *adress = (volatile uint32_t *)(mzapo_spiled + SPILED_REG_KNOBS_8BIT_o);
    return (uint8_t)(*adress >> 24) & 0x1;
}
