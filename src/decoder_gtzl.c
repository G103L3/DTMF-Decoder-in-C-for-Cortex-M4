/*! \file decoder_gtzl.c
* \author Contributor: G C 
* \brief Functions for decoder_gtzl.h
*/

/* Our Headers */
#include "global_parameters.h"
#include "gtzl.h"
#include "decoder_gtzl.h"

const double AMPL_THRS = 1200000000;	/* The amplitude threshold for the amplitude of a DTMF frequency */
const unsigned short DTMF_FREQ_AMT_D2 = DTMF_FREQ_AMT / 2;

struct_tone_frequencies
decoder_gtzl
(
double amplitudes[DTMF_FREQ_AMT]
)
{
	/* This contains the dominant frequencies from the received array of frequency amplitudes.
	* Values of -1 indicate an error or that a DTMF tone has not been identified.
	*/
	struct_tone_frequencies frqs_tone_return = {-1,-1};

	/* The below two booleans indicate if a low and high frequency has been found, which is above the desired threshold.
	* These will also be used to check if there are multiple DTMF tone pairs, which results in a signal that is not DTMF.
	* Finally, unsigned char is used to signify a boolean, since stdbool.h is not available in C89
	*/
	unsigned char found_lo = 0;
	unsigned char found_hi = 0;

	unsigned short frq_tmp_lo = 0;
	unsigned short frq_tmp_hi = 0;

	unsigned short i = 0;	/* Generic Iterator */

	/* Ideal threshold for amps probably in the range [500, 800] */
	for (i = 0; i < DTMF_FREQ_AMT_D2; i++)
	{
		if (amplitudes[i] > AMPL_THRS)
		/* This signifies the low frequencies.
		* The amplitudes of the following frequencies are held between i = 0 and i = 3: {697, 770, 852, 941}
		*/
		{
			if (found_lo)
			/* A low frequency with an amplitude higher than the threshold has been found */
			{
				/* There are more than two dominant low DTMF frequencies, which no longer remains a DTMF signal. */
				return frqs_tone_return;
			}
			else
			{
				frq_tmp_lo = DTMF_FRQS[i];
				found_lo = 1;
			}
		}
		
		if (amplitudes[4 + i] > AMPL_THRS)
		/* This signifies the high frequencies.
		* The amplitudes of the following frequencies are held between i = 4 and i = 7: {1209, 1336, 1477, 1633}
		*/
		{
			if (found_hi)
			/* A high frequency with an amplitude higher than the threshold has been found */
			{
				/* There are more than two dominant high DTMF frequencies, which no longer remains a DTMF signal. */
				return frqs_tone_return;
			}
			else
			{
				frq_tmp_hi = DTMF_FRQS[4 + i];
				found_hi = 1;
			}
		}
	}

	frqs_tone_return.low = (int) frq_tmp_lo;
	frqs_tone_return.high = (int) frq_tmp_hi;

	return frqs_tone_return;
}
