/*! \file scroll_lcd.h
* \author Gioele Giunta
* \version 1.0
* \since 8<sup>th</sup> May 2024
* \brief Selector for the signal decoding algorithm to be used
*
* The options are between FFT and Goertzel.
*/

#ifndef SCROLL_LCD_H_
#define SCROLL_LCD_H_
 
/* C Library Headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Driver Headers */
#include <gpio.h>
#include <delay.h>

/* Our Headers */
#include "global_parameters.h"
#include "lcd.h"
#include "errorm.h"
#include "switches.h"

/*! \fn int scroll_LCD(void)
* \param *sequence The char Array
* \returns 1 if the scrolling operation was successful, -1 if an error occurred, 0 if auto-scrolling mode is enabled / Nothing has changed.
* \brief Handles the scrolling of the LCD display based on user input.

* This function manages the scrolling of the LCD display when the left or right switch is pressed.
* It updates the g_scroll_offset variable accordingly and ensures the value stays within the valid range.
* If no switch is pressed for 3 seconds, the function enables the auto-scrolling mode.
*/
int scroll_LCD (char *sequence);

#endif // SCROLL_LCD_H_
