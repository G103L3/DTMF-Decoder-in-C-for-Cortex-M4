/*! \file mask_calibrate.c
 * \author Gioele Giunta
 * \version 1.0
 * \since 22<sup>th</sup> April 2024
 * \brief Functions for mask_calibrate.h
 */

/* Header */
#include "mask_calibrate.h"

/* Variable Declarations */
double mask_value;
double standards_masks[] = {-2.0, -5.0, -50.0, -100.0, -320.0, 320.0, 100.0, 50.0, 7.0, 4.0};
int standards_flags[] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
double vadc_;
double last_mask = 0.0;

void show_mask_settings(double mask_value);
void clean_flags(void);

int scrolling_counter = 0;
int k = 16;
char instructions[70] =  " Center * using ADC, when it is ready, click the joystick";
char instructions_[70] = " Center * using ";

/*! \fn void mask_calibrate(double max_amplitude, double half_amplitude, double res, double vadc)
* \brief Calibrates the mask value based on the input parameters.
*
* This function takes in the maximum amplitude, half amplitude, resolution, and ADC voltage reference, and calculates the mask value. 
* It then compares the mask value to a set of predefined standard values and displays the appropriate mask settings on the LCD. 
* If the "Center" switch of the joystick is pressed, the function sets the calibrated flag and returns.
* @param max_amplitude The maximum amplitude of the signal.
* @param half_amplitude The half amplitude of the signal.
* @param res The resolution of the ADC.
* @param vadc The ADC voltage reference.
*/
void mask_calibrate(double max_amplitude, double half_amplitude, double res, double vadc)
{
	int i;
	
	/* Title Scrolling Section */
	/*G_SAMPLE_RATE / 3 IS ALWAYS 333ms using current systick*/
	scrolling_counter++;
	if (((scrolling_counter % (G_SAMPLE_RATE/3)) == 0) && (k < strlen(instructions)))
	{
		/* Add one character from title[] to title_[] to facilitate g_scrolling to the left by one character */
		instructions_[k] = instructions[k];
		k++;
		print_top(instructions_);
  }

	
	/* Calibration System Section */
	vadc_ = vadc;
	mask_value = max_amplitude*(double)res/(half_amplitude*(double)vadc);
	
	/* Tell to the user to STOP THE AUDIO TOO MUCH DIFFERENCE BETWEEN MASKS */
	if(((last_mask >= 0.0 && last_mask <= 300.0) && mask_value <= 0.0 && mask_value >= -300.0) || ((last_mask <= 0.0 && last_mask >= -300.0) && mask_value >= 0.0 && mask_value <= 300.0)){
			lcd_set_cursor(0, 0);
			lcd_print(" STOP THE TRACK!");
			last_mask = mask_value;
			return;
	}
	
	last_mask = mask_value;
	
	/*
	It creates different voltage levels, with the goal of calling the show_mask_settings function only when
	a change in voltage has occurred that results in a level change. 
	PS. These levels differ from the levels in the show_mask_settings
	*/
	for(i = 0; i < 10; i++)
	{
		if(i == 0)
		{
			if(0.0 > mask_value && mask_value >= standards_masks[i] && standards_flags[i])
			{
				clean_flags();
				standards_flags[i] = 0;
				show_mask_settings(mask_value);
				return;
			}
		}
		else if(i == 9)
		{
			if(2.0 < mask_value && mask_value <= standards_masks[i] && standards_flags[i-1])
			{
				clean_flags();
				standards_flags[i-1] = 0;
				show_mask_settings(mask_value);
				return;
			}
		}
		else if(i == 4 || i == 5)
		{
			if((mask_value < standards_masks[4] || mask_value > standards_masks[5]) && standards_flags[4])
			{
				clean_flags();
				standards_flags[4] = 0;
				show_mask_settings(mask_value);
				return;
			}
		}
		else if(i < 4)
		{
			if(standards_masks[i-1] > mask_value && mask_value >= standards_masks[i] && standards_flags[i])
			{
				clean_flags();
				standards_flags[i] = 0;
				show_mask_settings(mask_value);
				return;
			}
		}
		else if(i > 5)
		{
			if(standards_masks[i+1] < mask_value && mask_value <= standards_masks[i] && standards_flags[i-1])
			{
				clean_flags();
				standards_flags[i-1] = 0;
				show_mask_settings(mask_value);
				return;
			}
		}
	}
	
	/* Joystick Click Handler */
		if (switch_get(P_SW_CR)) 
		{
      print_bottom("   CALIBRATED!  ");
			delay_ms(500);
			calibrated = 1;
			lcd_clear();
			return;
   }
}


/*! \fn void clean_flags(void)
* \brief Cleans the standards_flags array.
*
* This function sets all the flags in the standards_flags array to 1, indicating that all spots are free.
*/
void clean_flags(void)
{
	int i;
	for(i = 0; i < 9; i++)
	{
		standards_flags[i] = 1;
	}
}

/*! \fn void show_mask_settings(double mask_value)
* \param mask_value The current mask value.
* \brief Displays the current mask settings on the LCD.
*
* This function takes the current mask value and displays it on the LCD screen using the predefined standards_masks array.
*/
void show_mask_settings(double mask_value) 
{
	/* Setting the Display */
	lcd_set_cursor(0, 1);
	lcd_print("LT");
	lcd_set_cursor(14, 1);
	lcd_print("RT");
	lcd_set_cursor(2, 1);
	
	/* Checks which level is active and prints its calibration on the screen */
	if(mask_value < 0 && mask_value >= -2.0)
	{
		lcd_print("*    O     ");
	}
	if(mask_value < -2.0 && mask_value >= -5.0)
	{
		lcd_print(" *   O     ");
	}
	if(mask_value < -5.0 && mask_value >= -50.0)
	{
		lcd_print("  *  O     ");
	}
	if(mask_value < -50.0 && mask_value >= -150.0)
	{
		lcd_print("   * O     ");
	}
	if(mask_value < -150.0 && mask_value >= -400.0)
	{
		lcd_print("    *O     ");
	}
	if(mask_value < -400.0 || mask_value > 400.0)
	{
		lcd_print("     *     ");
	}
	if(mask_value > 150.0 && mask_value <= 400.0)
	{
		lcd_print("     O*    ");
	}
	if(mask_value > 50.0 && mask_value <= 150.0)
	{
		lcd_print("     O *   ");
	}
	if(mask_value > 5.0 && mask_value <= 50.0)
	{
		lcd_print("     O  *  ");
	}
	if(mask_value > 4.0 && mask_value <= 7.0)
	{
		lcd_print("     O   * ");
	}
	if(mask_value > 0 && mask_value <= 4.0)
	{
		lcd_print("     O    *");
	}
}
