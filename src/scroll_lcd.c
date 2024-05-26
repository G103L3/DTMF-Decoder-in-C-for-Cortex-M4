/*! \file scroll_lcd.c
* \author Gioele Giunta
* \version 1.0
* \since 8<sup>th</sup> May 2024
* \brief Provides functions for managing the scrolling of the LCD display.
*/

/* Header */
#include "scroll_LCD.h"

/* Variable Declarations */
int last_tick_counter = 0;

/*! \fn int scroll_LCD(void)
* \param *sequence The char Array
* \returns 1 if the scrolling operation was successful, -1 if an error occurred, 0 if auto-scrolling mode is enabled / Nothing has changed.
* \brief Handles the scrolling of the LCD display based on user input.
*
* This function manages the scrolling of the LCD display when the left or right switch is pressed.
* It updates the g_scroll_offset variable accordingly and ensures the value stays within the valid range.
* If no switch is pressed for 3 seconds, the function enables the auto-scrolling mode.
*/
int scroll_LCD (char *sequence)
{
	/* In order to avoid continuos pushing of the switch, so switching too fast */
	/* Considering a systick timed with 8000 hertz, g_systick_counter will be 8000 every second,*/
	/* I want a break of 333ms between the pushings so systick need to be >= 2666 */
	if((g_systick_counter - 2666) >= last_tick_counter)
	{
		if (switch_get(P_SW_LT))
		{
			last_tick_counter = g_systick_counter;
			
			if(g_scroll_offset > 16)
			{
				g_scrolling = 0;
				g_scroll_offset--;
				print_bottom("<-              ");
				/* No errors occured case */
				return 1; 
			}
			else
			{
				strcpy(error_text, "OUT BOUNDS!   ");
				/* Errors occured case */
				return -1;
			}
		}
		if (switch_get(P_SW_RT))
		{
			
			last_tick_counter = g_systick_counter;
			
			if(g_scroll_offset < (strlen(sequence)-1) && g_scroll_offset > 16)
			{
				g_scrolling = 0;
				g_scroll_offset++; 
				print_bottom("              ->");
				/* No errors occured case */
				return 1;
			}
			else
			{
				strcpy(error_text, "OUT BOUNDS!   ");
				/* Errors occured case */
				return -1;
			}
		}
	}
	/* After 3 seconds so 24000 scrolling goes in AUTO scrolling mode */
	if((g_systick_counter - 24000) >= last_tick_counter && g_scrolling == 0)
	{
		print_bottom("<-    AUTO    ->");
		g_scrolling = 1;
		return 0;
	}

	return 0;
}
