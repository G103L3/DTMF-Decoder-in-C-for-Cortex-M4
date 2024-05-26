/*! \file errorm.h
 * \author Gioele Giunta
 * \version 1.3
 * \since 26<sup>th</sup> April 2024
 * \brief Plugin to manage error printing
 */

#ifndef ERRORM_H_
#define ERRORM_H_

/* C Library Headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Driver Headers */
#include <platform.h>
#include <gpio.h>
#include <delay.h>

/* Our Headers */
#include <lcd.h>
#include <leds.h>

/* Global Variable Declaration */
extern char error_text[12];

/*! \fn void printError(void)
* \brief Prints the error message on the LCD and sets the appropriate LED.
*
* This function prints the error message stored in the error_text global variable on the second row of the LCD.
* It also sets the red LED to indicate the error condition.
*/
void printError(void);

#endif // READER_H
