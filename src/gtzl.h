/*! \file gtzl.h
* \author Contributor: G C 
* \brief Determines the amplitude for each DTMF frequency from an array of voltage levels
*/

#ifndef GTZL_H_
#define GTZL_H_

#include "complex_g3.h"

/*! \def DTMF_FREQ_AMT
* \brief The amount of frequencies used in DTMF
* 
* It is defined here in order to facilitate the creation of arrays with this amount of elements before function execution commences.
*/
#define DTMF_FREQ_AMT 8

extern const unsigned short DTMF_FRQS[DTMF_FREQ_AMT];

/*! \fn int goertzel (complex_g3_t* signal, double amplitudes[DTMF_FREQ_AMT]);
* \param *signal A pointer to an array of voltage levels
* \param amplitudes An array of amplitudes for each DTMF frequency
* \return The outcome of a run of the function: 0 indicates that no errors have occurred.
* \brief Calculates the amplitude of DTMF frequencies from an array of voltage levels.
*/
int
goertzel
(
complex_g3_t* signal,
double amplitudes[DTMF_FREQ_AMT]
);
#endif
