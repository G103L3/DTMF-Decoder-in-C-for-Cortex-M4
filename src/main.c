/*! \file main.c
* \brief Main file of the project
*/

/* C Library Headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Driver Headers */
#include <platform.h>
#include <gpio.h>
#include <adc.h>
#include <timer.h>
#include <delay.h>
#include <switches.h>
#include <leds.h>

/* Our Headers */
#include "global_parameters.h"
#include "complex_g3.h"
#include "lcd.h"
/* + EEPROM Inclusions */
#include "eeprom_g3.h"
#include "eeprom.h"
/* + Reader Inclusions */
#include "reader.h"
/* + FFT Inclusions */
#include "fft.h"
#include "decoder.h"
/* + Goertzel Inclusions */
#include "gtzl.h"
#include "decoder_gtzl.h"
/* + Comparator Inclusions */
#include "frequencies_comparator.h"
/* + User Helper Interfaces Inclusions */
#include "errorm.h"
#include "algorithm_setter.h"
#include "scroll_LCD.h"

/* Variable Declarations */
char last_char;
char sequence[G_SEQUENCE_LENGTH];
int algorithm;
int g_scrolling;
int g_scroll_offset;

void setting_interrupt(int sources) {
	status_flag = 0;
	algorithm = algorithm_setter(algorithm);
	write_eeprom_g3(algorithm);
	status_flag = 1;
}

void decoder_operations(){
	struct_tone_frequencies tone_frequencies;
	/* FFT Variables */
	complex_g3_t* out;
	/* Goertzel Variables */
	double amplitudes_gtzl[DTMF_FREQ_AMT];
	/* Comparator Variables */
	int comparator_return;

	
	if(algorithm == 2){
		/* FFT Code */
		out = FFT_simple(array_ready, G_ARRAY_SIZE); // Change this to out = FFT_simple(array_ready, G_ARRAY_SIZE);
		tone_frequencies = decode_dtmf(out);
		leds_set(0, 0, 1);
	}
	if(algorithm == 3){
		/* Goertzel Code */
		goertzel(array_ready, amplitudes_gtzl);
		tone_frequencies = decoder_gtzl(amplitudes_gtzl);
		leds_set(0, 1, 1);
	}

	comparator_return = frequencies_comparator(tone_frequencies, &last_char, sequence);

	if(comparator_return)
	{
		if(g_scrolling){
			print_top(sequence);
			g_scroll_offset = strlen(sequence);
		}
	}
	if(comparator_return == -1)
	/* Overflow detected */
	{
		status_flag = 0; //Stop the reader to give time to emptying the sequence
		lcd_clear();
		strcpy(sequence, ""); //Emptying string
		status_flag = 1;
		g_scroll_offset = 0;
		printError();
	}
	if(comparator_return == -2)
	/* MultiTone detecting */
	{
		printError();
	}
}

int main (void)
{
	int scroll_LCD_value = 0;
	g_scroll_offset = 0;
	g_scrolling = 1;
	/* frequencesComparator Variables */
	last_char = 'N';
	strcpy(sequence, "");
	/* Initializing output */
	lcd_init();
	lcd_clear();
	leds_init();
	/* Set up debug signals */
	gpio_set_mode(P_DBG_ISR, Output);
	gpio_set_mode(P_DBG_MAIN, Output);
	/* Set up on-board switch */
	gpio_set_mode(P_SW, PullUp);
	gpio_set_trigger(P_SW, Falling);
	gpio_set_callback(P_SW, setting_interrupt);
	__enable_irq();
	/* Initializing the EEPROM */
	EEPROM_Init();
	algorithm = read_eeprom_g3();
	if(algorithm != 2 && algorithm != 3){
		algorithm = 2; //FFT
		write_eeprom_g3(algorithm);
	}
	/*Initializing Reader Thread*/
	reader_init();
	
	while (1)
	{
		/* Scroll LCD call every cycle */
		scroll_LCD_value = scroll_LCD(sequence);
		if(scroll_LCD_value == 1)
		{
			lcd_clear();
			print_top_dynamic(sequence, g_scroll_offset);
		}
		else if(scroll_LCD_value == -1)
		{	
			//Out of Bounds detecting
			printError();
		}
		
		
		/* Array fill progress check */
		if (data_ready)
		/* Checks if the array swapper has switched the array to be used */
		{
			/* Disable data_ready to prevent the same array being sent to the decoder */
			data_ready = 0;	
			/* Call the Decoding Operations */
			decoder_operations();
		}
	}
}
