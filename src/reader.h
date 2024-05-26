/*! \file reader.h
* \author Gioele Giunta
* \version 1.3
* \since 16<sup>th</sup> April 2024
* \brief Functions and data structures for the analog data reader module
*
* This header file defines the functions and data structures used by the analog data reader module. The main responsibilities of this module are:
* 1. Periodically read the analog value from a specific pin (assumed to be pin 15).
* 2. Store the read values in a global data array.
* 3. Manage the swapping of the main and secondary data arrays to provide the latest data to the main application.
* 4. Handle the calibration of the analog mask, if necessary.
* 5. Provide a mechanism to synchronize the reading process with the main application.
*/

#ifndef READER_H_
#define READER_H_

/* C Library Headers */
#include <stdio.h>
#include <stdlib.h>

/* Driver Headers */
#include <adc.h>
#include <timer.h>
#include <platform.h>
#include <gpio.h>

/* Our Headers */
#include <lcd.h>
#include <switches.h>
#include <leds.h>
#include "complex_g3.h"
#include "mask_calibrate.h"
#include "global_parameters.h"

#define ARRAY_ELEMENTS G_ARRAY_SIZE

/* 8000 is the sample rate, so 8000 elements per second */
#define SAMPLE_RATE G_SAMPLE_RATE //In Hertz
#define VREF (3.3) //Maximum Voltage

/* Global Variable Declarations */
extern int data_ready; //This is shared between the main and the reader.h is the flag to determine when an array is full 
extern complex_g3_t *array_ready; //This is shared between the main and the reader.h is the previous Array compiled 
extern int status_flag; //To block the reading execution
extern int calibrated; //Give information about the state of the calibration

/*! \fn void reader_init(void)
* \brief Initializes the ADC, LCD, SysTick, and sets the initial memory state.
*
* This function initializes all the initial values of the reader.
* By initializing the systick, it causes the read function to be called as a handler of the systick.
*/
void reader_init(void);

#endif // READER_H
