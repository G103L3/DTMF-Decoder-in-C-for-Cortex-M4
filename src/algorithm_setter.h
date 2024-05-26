/*! \file algorithm_setter.h
 * \author Gioele Giunta
 * \version 1.0
 * \since 8<sup>th</sup> May 2024
 * \brief Selector for the signal decoding algorithm to be used
 *
 * The options are between FFT and Goertzel.
 */

#ifndef ALGORITH_MSETTER_H_
#define ALGORITH_MSETTER_H_

/* C Library Headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Driver Headers */
#include <gpio.h>
#include <delay.h>
#include <lcd.h>
#include <leds.h>

/* Our Headers */
#include "global_parameters.h"
#include "switches.h"


/*! \fn int algorithm_setter (int selection)
* \param selection Current algorithm in use
* \returns User selected algorithm
* \brief Allows the user to select between the two available signal decoding algorithm
*
* The number '2' indicates FFT while '3' indicates 'Goertzel' .
*/
int algorithm_setter (int selection);

#endif // ALGORITHM_SETTER_H_
