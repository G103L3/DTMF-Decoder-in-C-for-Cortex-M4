/*! \file errorm.c
 * \author Gioele Giunta
 * \version 1.3
 * \since 26<sup>th</sup> April 2024
 * \brief Functions for errorm.h 
 */

/* Header */
#include "errorm.h"


char p[16];
char error_text[12];

/*! \fn void printError(void)
* \brief Prints the error message on the LCD and sets the appropriate LED.
*
* This function prints the error message stored in the error_text global variable on the second row of the LCD.
* It also sets the red LED to indicate the error condition.
*/
void printError (void)
{
	leds_set(1, 0, 0);
	lcd_set_cursor(0, 1);	/* Clean only the second row */
	lcd_print("                 ");

	strcpy(p, "ER: ");
	lcd_set_cursor(0, 1);	/* Error are shown in the second row */
	strcat(p, error_text);
	lcd_print(p);

	leds_set(1, 0, 0);
	delay_ms(500);
	lcd_set_cursor(0, 1);	/* Clean only the second row */
	lcd_print("                 ");

	red_stop();
}
