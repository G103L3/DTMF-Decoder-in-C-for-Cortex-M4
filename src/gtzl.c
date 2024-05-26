/*! \file gtzl.c
* \author Contributor: G C 
* \sa gtzl.h
* \brief Functions for gtzl.h
*
* The code here has been adapted from the algorithm defined in <a href="https://www.embedded.com/the-goertzel-algorithm/" target="_blank" rel="nofollow noreferrer noopener">`The Goertzel Algorithm' at www.embedded.com</a>.
*/

#include "global_parameters.h"
#include "complex_g3.h"
#include "gtzl.h"

/* After working out with each frequency and taking average, a Goertzel array of 508 elements will be used.
* Note that the input will still be 512 however, the last 4 elements will be ignored.
*/

#ifndef M_PI
	const double M_PI = 3.141592654;
#endif

const double G3_2PI = 2 * M_PI;

const unsigned short FS = G_SAMPLE_RATE;	/* Sample Rate */
const unsigned short GTZ_AR_SZ = 508;	/* # elements to be used by the Goertzel algorithm */
const unsigned short DTMF_FRQS[DTMF_FREQ_AMT] = {697, 770, 852, 941, 1209, 1336, 1477, 1633};

/* k = round((GTZ_AR_SZ * freq) / FS) */
/* An integer is required, as shown in the reference webpage */
const unsigned short K_0697 = 44;	/* = 42.2595 */
const unsigned short K_0770 = 49;	/* = 48.8950 */
const unsigned short K_0852 = 54;	/* = 54.1020 */
const unsigned short K_0941 = 60;	/* = 59.7535 */
const unsigned short K_1209 = 77;	/* = 76.7715 */
const unsigned short K_1336 = 85;	/* = 84.8360 */
const unsigned short K_1477 = 94;	/* = 93.7895 */
const unsigned short K_1633 = 104;	/* = 103.700 */

/* w = ((2 * G3_PI) / GTZ_AR_SZ) * K_XXXX  = (G3_2PI / GTZ_AR_SZ) * K_XXXX */
const float W_0697 = (G3_2PI / GTZ_AR_SZ) * K_0697;
const float W_0770 = (G3_2PI / GTZ_AR_SZ) * K_0770;
const float W_0852 = (G3_2PI / GTZ_AR_SZ) * K_0852;
const float W_0941 = (G3_2PI / GTZ_AR_SZ) * K_0941;
const float W_1209 = (G3_2PI / GTZ_AR_SZ) * K_1209;
const float W_1336 = (G3_2PI / GTZ_AR_SZ) * K_1336;
const float W_1477 = (G3_2PI / GTZ_AR_SZ) * K_1477;
const float W_1633 = (G3_2PI / GTZ_AR_SZ) * K_1633;

#ifndef _MATH_H_
	/* Values have been computed manually beforehand */

	/* COS_XXXX = cos(W_XXXX) */
	const float COS_0697 = 0.8555;
	const float COS_0770 = 0.8219;
	const float COS_0852 = 0.7851;
	const float COS_0941 = 0.7370;
	const float COS_1209 = 0.5798;
	const float COS_1336 = 0.4964;
	const float COS_1477 = 0.3969;
	const float COS_1633 = 0.2807;

	/* SIN_xxxx = sin(w_xxxx) */
	const float SIN_0697 = 0.5177;
	const float SIN_0770 = 0.5696;
	const float SIN_0852 = 0.6193;
	const float SIN_0941 = 0.6758;
	const float SIN_1209 = 0.8148;
	const float SIN_1336 = 0.8681;
	const float SIN_1477 = 0.9179;
	const float SIN_1633 = 0.9598;
#else
	/* COS_xxxx = cos(w_xxxx) */
	const float COS_0697 = cos(W_0697);
	const float COS_0770 = cos(W_0770);
	const float COS_0852 = cos(W_0852);
	const float COS_0941 = cos(W_0941);
	const float COS_1209 = cos(W_1209);
	const float COS_1336 = cos(W_1336);
	const float COS_1477 = cos(W_1477);
	const float COS_1633 = cos(W_1633);

	/* SIN_xxxx = sin(w_xxxx) */
	const float SIN_0697 = sin(W_0697);
	const float SIN_0770 = sin(W_0770);
	const float SIN_0852 = sin(W_0852);
	const float SIN_0941 = sin(W_0941);
	const float SIN_1209 = sin(W_1209);
	const float SIN_1336 = sin(W_1336);
	const float SIN_1477 = sin(W_1477);
	const float SIN_1633 = sin(W_1633);
#endif

/* Coefficient eqn: CEF_XXXX = 2 * COS_XXXX */
const float CEF_0697 = 2 * COS_0697;
const float CEF_0770 = 2 * COS_0770;
const float CEF_0852 = 2 * COS_0852;
const float CEF_0941 = 2 * COS_0941;
const float CEF_1209 = 2 * COS_1209;
const float CEF_1336 = 2 * COS_1336;
const float CEF_1477 = 2 * COS_1477;
const float CEF_1633 = 2 * COS_1633;

/* Function Declarations */
double mag_eqn_optimised ( double q1, double q2, double cef);

int
goertzel
(
complex_g3_t* signal,
double amplitudes[DTMF_FREQ_AMT]
)
{
	/* q_0 for each DTMF frequency */
	double	q0_0697 = 0,
		q0_0770 = 0,
		q0_0852 = 0,
		q0_0941 = 0,
		q0_1209 = 0,
		q0_1336 = 0,
		q0_1477 = 0,
		q0_1633 = 0;
	
	/* q_1 for each DTMF frequency */
	double	q1_0697 = 0,
		q1_0770 = 0,
		q1_0852 = 0,
		q1_0941 = 0,
		q1_1209 = 0,
		q1_1336 = 0,
		q1_1477 = 0,
		q1_1633 = 0;

	/* q_2 for each DTMF frequency */
	double	q2_0697 = 0,
		q2_0770 = 0,
		q2_0852 = 0,
		q2_0941 = 0,
		q2_1209 = 0,
		q2_1336 = 0,
		q2_1477 = 0,
		q2_1633 = 0;

	unsigned short i = 0;	/* Iterator */

	/* Variables holding the Magnitude squared for each frequency */
	double	mag_sqd_0697 = 0,
		mag_sqd_0770 = 0,
		mag_sqd_0852 = 0,
		mag_sqd_0941 = 0,
		mag_sqd_1209 = 0,
		mag_sqd_1336 = 0,
		mag_sqd_1477 = 0,
		mag_sqd_1633 = 0;

#ifdef _MATH_H_
	/* Variables holding the Magnitude for each frequency */
	double	mag_0697 = 0,
		mag_0770 = 0,
		mag_0852 = 0,
		mag_0941 = 0,
		mag_1209 = 0,
		mag_1336 = 0,
		mag_1477 = 0,
		mag_1633 = 0;
#endif

	for (i = 0; i < GTZ_AR_SZ; i++)
	{
		/* Calculation for q0 values */
		q0_0697 = (CEF_0697 * q1_0697) - q2_0697 + signal[i].re;
		q0_0770 = (CEF_0770 * q1_0770) - q2_0770 + signal[i].re;
		q0_0852 = (CEF_0852 * q1_0852) - q2_0852 + signal[i].re;
		q0_0941 = (CEF_0941 * q1_0941) - q2_0941 + signal[i].re;
		q0_1209 = (CEF_1209 * q1_1209) - q2_1209 + signal[i].re;
		q0_1336 = (CEF_1336 * q1_1336) - q2_1336 + signal[i].re;
		q0_1477 = (CEF_1477 * q1_1477) - q2_1477 + signal[i].re;
		q0_1633 = (CEF_1633 * q1_1633) - q2_1633 + signal[i].re;

		/* Calculation for q2 values */
		q2_0697 = q1_0697;
		q2_0770 = q1_0770;
		q2_0852 = q1_0852;
		q2_0941 = q1_0941;
		q2_1209 = q1_1209;
		q2_1336 = q1_1336;
		q2_1477 = q1_1477;
		q2_1633 = q1_1633;

		/* Calculation for q1 values */
		q1_0697 = q0_0697;
		q1_0770 = q0_0770;
		q1_0852 = q0_0852;
		q1_0941 = q0_0941;
		q1_1209 = q0_1209;
		q1_1336 = q0_1336;
		q1_1477 = q0_1477;
		q1_1633 = q0_1633;
	}

	/* Obtaining the Magnitude squared for each frequency */
	mag_sqd_0697 = mag_eqn_optimised(q1_0697, q2_0697, CEF_0697);
	mag_sqd_0770 = mag_eqn_optimised(q1_0770, q2_0770, CEF_0770);
	mag_sqd_0852 = mag_eqn_optimised(q1_0852, q2_0852, CEF_0852);
	mag_sqd_0941 = mag_eqn_optimised(q1_0941, q2_0941, CEF_0941);
	mag_sqd_1209 = mag_eqn_optimised(q1_1209, q2_1209, CEF_1209);
	mag_sqd_1336 = mag_eqn_optimised(q1_1336, q2_1336, CEF_1336);
	mag_sqd_1477 = mag_eqn_optimised(q1_1477, q2_1477, CEF_1477);
	mag_sqd_1633 = mag_eqn_optimised(q1_1633, q2_1633, CEF_1633);

/* Computes the square root of the Magnitude squared */
/*
#ifdef _MATH_H_
	mag_0697 = sqrt(mag_sqd_0697);
	mag_0770 = sqrt(mag_sqd_0770);
	mag_0852 = sqrt(mag_sqd_0852);
	mag_0941 = sqrt(mag_sqd_0941);
	mag_1209 = sqrt(mag_sqd_1209);
	mag_1336 = sqrt(mag_sqd_1336);
	mag_1477 = sqrt(mag_sqd_1477);
	mag_1633 = sqrt(mag_sqd_1633);

	amplitudes[0] = mag_0697;
	amplitudes[1] = mag_0770;
	amplitudes[2] = mag_0852;
	amplitudes[3] = mag_0941;
	amplitudes[4] = mag_1209;
	amplitudes[5] = mag_1336;
	amplitudes[6] = mag_1477;
	amplitudes[7] = mag_1633;

	return 0;
#endif
*/
	amplitudes[0] = mag_sqd_0697;
	amplitudes[1] = mag_sqd_0770;
	amplitudes[2] = mag_sqd_0852;
	amplitudes[3] = mag_sqd_0941;
	amplitudes[4] = mag_sqd_1209;
	amplitudes[5] = mag_sqd_1336;
	amplitudes[6] = mag_sqd_1477;
	amplitudes[7] = mag_sqd_1633;

	return 0;
}

/*! \fn double mag_eqn_optimised ( double q1, double q2, double cef)
* \param q1 Q<SUB>1</SUB> of a DTMF frequency
* \param q2 Q<SUB>2</SUB> of a DTMF frequency
* \param cef The coefficient for a DTMF frequency
* \returns The result of the computation
* \brief Computes the magnitude square corresponding to a DTMF frequency
*
* The following formula is used inside the function, taken from the previously mentioned webpage reference, under the section title 'An Optimized Goertzel': 
* \f[
* 	\text{magnitude}^2 = {Q_1}^2 \cdot {Q_2}^2 - \left( {Q_1} \cdot {Q_2} \cdot \text{coeff}\right)
* \f]
*/
double
mag_eqn_optimised
(
double q1,
double q2,
double cef
)
/* Using the "Optimized" equation from the source listed at the top */
{
	return ((q1 * q1) + (q2 * q2) - (q1 * q2 * cef));
}
