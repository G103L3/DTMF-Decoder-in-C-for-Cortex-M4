/*! \file decoder.h
* \brief Identifies the dominant frequencies in a DTMF compliant signal using the FFT technique
*/

#ifndef DECODER_H_
#define DECODER_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "complex_g3.h"

#include "global_parameters.h"

#define NN G_ARRAY_SIZE  /* Define the maximum FFT size, must be a power of 2 */
#define FS G_SAMPLE_RATE

/*! \fn struct_tone_frequencies decode_dtmf(complex_g3_t *data)
* \param data Pointer to an array of complex numbers representing the frequency spectrum of a DTMF signal
* \returns A struct_tone_frequencies object containing the dominant low and high frequencies detected in the DTMF signal
* \brief Identifies the potential low and high frequencies from a given DTMF signal using the FFT output provided in the `data` array.
* 
* This function processes the FFT results stored in `data` to detect the presence of specific DTMF frequencies within the permissible 
* frequency tolerance. It calculates the magnitude of each frequency that is within a range of a valid dtmf tone in the FFT output and 
* compares it against an amplitude threshold and frequency tolerance. The function returns the dominant frequencies that are within 
* the specified thresholds. If multiple valid frequencies are found within the tolerance range or if no valid frequency is detected, the 
* function may adjust or set the values to indicate an error or ambiguity in detection.
*/

struct_tone_frequencies decode_dtmf(complex_g3_t *data);

#endif
