/*! \file global_parameters.h
 * \author Gioele Giunta, Contributor: J E
 * \version 1.3
 * \since 21<sup>st</sup> April 2024
 * \brief Project wide variables and definitions
 */
#ifndef GLOBAL_PARAMETERS_H_
#define GLOBAL_PARAMETERS_H_
#include <platform.h>

/* 8000 is the sample rate, so 8000 elements per second */
#define G_SAMPLE_RATE 8000

/* To have a good number of samples, we need a good sample time. To have a good sample time, from matlab we deduced that is 0,064 second
 * That in elements length become 0,064/(1/8000) = 512
*/
#define G_ARRAY_SIZE 512

#define G_MAX_AMPLITUDE 8000.0

#define G_SEQUENCE_LENGTH 100

#define G_PI 3.14159265358979323846

#define true 1
#define false 0
	

 typedef struct struct_tone_frequencies
{
    int low;
    int high;
} struct_tone_frequencies;

typedef int boolean;

extern int g_scrolling;
extern int g_scroll_offset; /* -> 2 for FFT, 3 for Goertzel */

extern volatile uint32_t g_systick_counter;

#endif
