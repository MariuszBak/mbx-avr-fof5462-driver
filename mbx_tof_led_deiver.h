/*
 * mbx_tof_led_deiver.h
 *
 *  Created on: 9 kwi 2023
 *      Author: Mariusz Bak
 *       Title: Driver for 7 segment LED display TOF5462BS_6B
 */

#ifndef MBX_TOF_LED_DRIVER_MBX_TOF_LED_DEIVER_H_
#define MBX_TOF_LED_DRIVER_MBX_TOF_LED_DEIVER_H_

#define TOF_DIG_1_PORT	PORTC
#define TOF_DIG_1_DDR		DDRC
#define TOF_DIG_1_PIN		PC0
#define TOF_DIG_2_PORT	PORTC
#define TOF_DIG_2_DDR		DDRC
#define TOF_DIG_2_PIN		PC1
#define TOF_DIG_3_PORT	PORTC
#define TOF_DIG_3_DDR		DDRC
#define TOF_DIG_3_PIN		PC2
#define TOF_DIG_4_PORT	PORTC
#define TOF_DIG_4_DDR		DDRC
#define TOF_DIG_4_PIN		PC3

#define TOF_SEG_A_PORT	PORTB
#define TOF_SEG_A_DDR		DDRB
#define TOF_SEG_A_PIN		PB0
#define TOF_SEG_B_PORT	PORTB
#define TOF_SEG_B_DDR		DDRB
#define TOF_SEG_B_PIN		PB1
#define TOF_SEG_C_PORT	PORTD
#define TOF_SEG_C_DDR		DDRD
#define TOF_SEG_C_PIN		PD2
#define TOF_SEG_D_PORT	PORTD
#define TOF_SEG_D_DDR		DDRD
#define TOF_SEG_D_PIN		PD3
#define TOF_SEG_E_PORT	PORTD
#define TOF_SEG_E_DDR		DDRD
#define TOF_SEG_E_PIN		PD4
#define TOF_SEG_F_PORT	PORTD
#define TOF_SEG_F_DDR		DDRD
#define TOF_SEG_F_PIN		PD5
#define TOF_SEG_G_PORT	PORTD
#define TOF_SEG_G_DDR		DDRD
#define TOF_SEG_G_PIN		PD6
#define TOF_DOTS_PORT		PORTD
#define TOF_DOTS_DDR		DDRD
#define TOF_DOTS_PIN		PD7

enum TOF_DOTS_STATES {
	off,
	on,
	up,
	down
};
typedef enum TOF_DOTS_STATES tof_dots_state_t;

void tof_led_init();
void tof_led_refresh();
void tof_led_set(uint8_t buffer[4]);
void tof_led_set_int(uint16_t value);
void tof_led_digit_set(uint8_t digit_id, uint8_t value);
void tof_led_dots_set(tof_dots_state_t dots_state);

#endif /* MBX_TOF_LED_DRIVER_MBX_TOF_LED_DEIVER_H_ */
