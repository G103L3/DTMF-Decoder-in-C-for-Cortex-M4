/*! \file algorithm_setter.c
 * \author Gioele Giunta
 * \version 1.0
 * \since 8<sup>th</sup> May 2024
 * \brief Functions for algorithm_setter.h
 */

/* Header */
#include "algorithm_setter.h"


/*! \fn int algorithm_setter (int selection)
* \param selection Current algorithm in use
* \returns User selected algorithm
* \brief Allows the user to select between the two available signal decoding algorithm
*
* The number '2' indicates FFT while '3' indicates 'Goertzel' .
*/
int algorithm_setter (int selection)
{
	int scrollingCounterA = 0;
	int g = 16;
	char title[70] =  " Select the decoding algorithm, using the joystick";
	char title_[70] = " Select the deco";
	
	leds_set(0, 1, 0);
	
	if (selection == 2)
	{
		print_bottom("|FFT|      GTZL ");
	}
	else if (selection == 3)
	{
		print_bottom(" FFT      |GTZL|");
	}
	else
	{
		print_bottom(" FFT       GTZL ");
	}
	
	
	while(1)
	{
		/* Title Scrolling Section */
		scrollingCounterA++;

		
		if (((scrollingCounterA % (200000)) == 0) && (g < strlen(title)))
		{
			/* Add one character from title[] to title_[] to facilitate g_scrolling to the left by one character */
			title_[g] = title[g];
			g++;
			print_top(title_);
		}
		
		/* Selection System Section */
		if (switch_get(P_SW_LT))
		{
			print_bottom("|FFT|      GTZL ");
			selection = 2;
		}

		if (switch_get(P_SW_RT))
		{
			print_bottom(" FFT      |GTZL|");
			selection = 3;
		}

		if (switch_get(P_SW_CR) && selection != 0)
		{
			print_bottom("      SET!      ");
			delay_ms(500);
			lcd_clear();

			return selection;
		}
	}
}
