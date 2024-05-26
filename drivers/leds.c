#include <platform.h>
#include <gpio.h>
#include "leds.h"

//Edited Gioele Giunta
int red_flag = 0; //Edited, when red_flag is on the other leds_set can't use the led

void leds_init(void) {
	// Set 3 led pins to outputs.
	gpio_set_mode(P_LED_R, Output);
	gpio_set_mode(P_LED_G, Output);
	gpio_set_mode(P_LED_B, Output);
	
	leds_set(0, 0, 0);
}

void leds_set(int red_on, int green_on, int blue_on) {
	// Boolean operation to decide on state for both active
	// high and low LEDs.
	if(red_flag == 0){
		gpio_set(P_LED_R, !red_on != LED_ON);
		gpio_set(P_LED_G, !green_on != LED_ON);
		gpio_set(P_LED_B, !blue_on != LED_ON);
		if(red_on == 1){
			red_flag = 1;
		}
	}
}

void red_stop() {
	// Remove the red_flag
	red_flag = 0;
	leds_set(0, 0, 0);
}

// *******************************ARM University Program Copyright © ARM Ltd 2014*************************************   
