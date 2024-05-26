/*! \file mask_calibrate.h
 * \author Gioele Giunta
 * \version 1.0
 * \since 22<sup>th</sup> April 2024
 * \brief Additional function to Reader that helps at software startup to calibrate the sampling middle point.
 */

#ifndef MASK_CALIBRATE_H_
#define MASK_CALIBRATE_H_

/* C Library Headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Driver Headers */
#include <gpio.h>
#include <delay.h>
#include <lcd.h>

/* Our Headers */
#include "reader.h"
#include "global_parameters.h"
#include "switches.h"

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
void mask_calibrate(double max_amplitude, double half_amplitude, double res, double vadc);

#endif // MASK_CALIBRATE_H_
