/*! \file decoder.c
* \brief Functions for decoder.h
*/

#include "decoder.h"
#include "string.h"
#include "lcd.h"

double const T = 1.0 / FS;  /* Sampling interval */
/* Amplitude thresholds for now */
double const amplitude_threshold_low = 40000.0; 
double const amplitude_threshold_high = 40000.0;

/* DTMF frequencies and their typical ranges due to FFT resolution */
int const dtmf_low_freqs[] = {697, 770, 852, 941};
int const dtmf_high_freqs[] = {1209, 1336, 1477, 1633};
int const freq_tolerance = 30; /* Frequency tolerance due to FFT resolution */

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
struct_tone_frequencies decode_dtmf(complex_g3_t *data) 
{
	struct_tone_frequencies result;
	double max_low_freq_amp = 0, max_high_freq_amp = 0;
	int i, j;
	result.low = 0, result.high = 0;

	/* Check only up to Nyquist frequency */
	for (i = 0; i < (NN / 2); i++)
	{ 		
		double amp = sqrt(data[i].re * data[i].re + data[i].im * data[i].im);
	}

	/* Check only up to Nyquist frequency */
	for (i = 0; i < (NN / 2); i++) 
	{ 
		double freq = (double)(FS * i) / NN;
		double amp = complex_magnitude(data[i]);

		/* Check low group frequencies */
		for (j = 0; j < 4; j++) 
		{
			if (fabs(freq - dtmf_low_freqs[j]) <= freq_tolerance && amp > amplitude_threshold_low) 
			{
				if (result.low == 0 || (amp > max_low_freq_amp && fabs(freq - result.low) <= freq_tolerance)) 
				{
					max_low_freq_amp = amp;
					result.low = dtmf_low_freqs[j];
				}
				else if (result.low != dtmf_low_freqs[j] && fabs(freq - result.low) > freq_tolerance)
				{
					result.low = -1;
				}
			}
		}
		/* Check high group frequencies */
		for (j = 0; j < 4; j++) 
		{
			if (fabs(freq - dtmf_high_freqs[j]) <= freq_tolerance && amp > amplitude_threshold_high) 
			{
				if (result.high == 0 || (amp > max_high_freq_amp && fabs(freq - result.high) <= freq_tolerance)) 
				{
					max_high_freq_amp = amp;
					result.high = dtmf_high_freqs[j];
				}
				else if (result.high != dtmf_high_freqs[j] && fabs(freq - result.high) > freq_tolerance)
				{
					result.high = -1;
				}
			}
		}
	}

	return result;
}
