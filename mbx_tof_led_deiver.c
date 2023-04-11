/*
 * mbx_tof_led_deiver.c
 *
 *  Created on: 9 kwi 2023
 *      Author: Mariusz Bak
 *       Title: Driver for 7 segment LED display TOF5462BS_6B
 */

#include <avr/pgmspace.h>

#include "mbx_tof_led_deiver.h"

#define PIN_ON(port,pin) port |= (1<<pin)
#define PIN_OFF(port,pin) port &= ~(1<<pin)

const uint8_t led_char_array[] PROGMEM = {
//        _gfedcba <- segments
		0b11000000,// 0
		0b11111001, // 1
		0b10100100, // 2
		0b10110000, // 3
		0b10011001, // 4
		0b10010010, // 5
		0b10000010, // 6
		0b11111000, // 7
		0b10000000, // 8
		0b10010000, // 9
		0b10001000, // A (10)
		0b10000011, // B (11)
		0b11000110, // C (12)
		0b10100001, // D (13)
		0b10000110, // E (14)
		0b10001110, // F (15)
		0b11111111, // blank (16)
		0b11000111, // L (17)
		0b10111111, // - (18)
		};

volatile uint8_t digits_buffer[] = { 18, 18, 18, 18 };

volatile struct {
	uint8_t active_digit :2;
	uint8_t dot_up :1;
	uint8_t dot_down :1;
} led_register;

void digits_off() {
	PIN_ON(TOF_DIG_1_PORT, TOF_DIG_1_PIN);
	PIN_ON(TOF_DIG_2_PORT, TOF_DIG_2_PIN);
	PIN_ON(TOF_DIG_3_PORT, TOF_DIG_3_PIN);
	PIN_ON(TOF_DIG_4_PORT, TOF_DIG_4_PIN);
}

void digit_on(uint8_t digit_id) {
	digits_off();
	switch (digit_id) {
	case 0:
		PIN_OFF(TOF_DIG_1_PORT, TOF_DIG_1_PIN);
		break;
	case 1:
		PIN_OFF(TOF_DIG_2_PORT, TOF_DIG_2_PIN);
		break;
	case 2:
		PIN_OFF(TOF_DIG_3_PORT, TOF_DIG_3_PIN);
		break;
	case 3:
		PIN_OFF(TOF_DIG_4_PORT, TOF_DIG_4_PIN);
		break;
	}
}

void segments_set(uint8_t segments) {
	if (segments & (1 << 0))
		PIN_ON(TOF_SEG_A_PORT, TOF_SEG_A_PIN);
	else
		PIN_OFF(TOF_SEG_A_PORT, TOF_SEG_A_PIN);
	if (segments & (1 << 1))
		PIN_ON(TOF_SEG_B_PORT, TOF_SEG_B_PIN);
	else
		PIN_OFF(TOF_SEG_B_PORT, TOF_SEG_B_PIN);
	if (segments & (1 << 2))
		PIN_ON(TOF_SEG_C_PORT, TOF_SEG_C_PIN);
	else
		PIN_OFF(TOF_SEG_C_PORT, TOF_SEG_C_PIN);
	if (segments & (1 << 3))
		PIN_ON(TOF_SEG_D_PORT, TOF_SEG_D_PIN);
	else
		PIN_OFF(TOF_SEG_D_PORT, TOF_SEG_D_PIN);
	if (segments & (1 << 4))
		PIN_ON(TOF_SEG_E_PORT, TOF_SEG_E_PIN);
	else
		PIN_OFF(TOF_SEG_E_PORT, TOF_SEG_E_PIN);
	if (segments & (1 << 5))
		PIN_ON(TOF_SEG_F_PORT, TOF_SEG_F_PIN);
	else
		PIN_OFF(TOF_SEG_F_PORT, TOF_SEG_F_PIN);
	if (segments & (1 << 6))
		PIN_ON(TOF_SEG_G_PORT, TOF_SEG_G_PIN);
	else
		PIN_OFF(TOF_SEG_G_PORT, TOF_SEG_G_PIN);
	if (segments & (1 << 7))
		PIN_ON(TOF_DOTS_PORT, TOF_DOTS_PIN);
	else
		PIN_OFF(TOF_DOTS_PORT, TOF_DOTS_PIN);
}

void digit_set(uint8_t value) {
	uint8_t segments = pgm_read_byte(&led_char_array[value]);
	if (led_register.active_digit == 2 && led_register.dot_up)
		segments &= ~(1 << 7);
	if (led_register.active_digit == 3 && led_register.dot_down)
		segments &= ~(1 << 7);
	segments_set(segments);
}

void tof_led_init() {
	// Set dipsplay select ports
	TOF_DIG_1_DDR |= (1 << TOF_DIG_1_PIN);
	TOF_DIG_2_DDR |= (1 << TOF_DIG_2_PIN);
	TOF_DIG_3_DDR |= (1 << TOF_DIG_3_PIN);
	TOF_DIG_4_DDR |= (1 << TOF_DIG_4_PIN);
	// Set segments ports
	TOF_SEG_A_DDR |= (1 << TOF_SEG_A_PIN);
	TOF_SEG_B_DDR |= (1 << TOF_SEG_B_PIN);
	TOF_SEG_C_DDR |= (1 << TOF_SEG_C_PIN);
	TOF_SEG_D_DDR |= (1 << TOF_SEG_D_PIN);
	TOF_SEG_E_DDR |= (1 << TOF_SEG_E_PIN);
	TOF_SEG_F_DDR |= (1 << TOF_SEG_F_PIN);
	TOF_SEG_G_DDR |= (1 << TOF_SEG_G_PIN);
	// Set dots port
	TOF_DOTS_DDR |= (1 << TOF_DOTS_PIN);
	// Digits off
	digits_off();
}

void tof_led_refresh() {
	digits_off();
	led_register.active_digit++;
	digit_set(digits_buffer[led_register.active_digit]);
	digit_on(led_register.active_digit);
}

void tof_led_set(uint8_t buffer[4]) {
	digits_buffer[0] = buffer[0];
	digits_buffer[1] = buffer[1];
	digits_buffer[2] = buffer[2];
	digits_buffer[3] = buffer[3];
}

void tof_led_set_int(uint16_t value) {
	if (value > 9999) {
		// Overload
		digits_buffer[0] = 16;
		digits_buffer[1] = 16;
		digits_buffer[2] = 0;
		digits_buffer[3] = 17;
	} else {
		digits_buffer[0] = value / 1000;
		uint16_t rest = value % 1000;
		digits_buffer[1] = rest / 100;
		rest = rest % 100;
		digits_buffer[2] = rest / 10;
		rest = rest % 10;
		digits_buffer[3] = rest;
	}
}

void tof_led_digit_set(uint8_t digit_id, uint8_t value) {
	digits_buffer[digit_id] = value;
}

void tof_led_dots_set(tof_dots_state_t dots_state) {
	switch (dots_state) {
	case off:
		led_register.dot_up = 0;
		led_register.dot_down = 0;
		break;
	case on:
		led_register.dot_up = 1;
		led_register.dot_down = 1;
		break;
	case up:
		led_register.dot_up = 1;
		led_register.dot_down = 0;
		break;
	case down:
		led_register.dot_up = 0;
		led_register.dot_down = 1;
		break;
	}
}

