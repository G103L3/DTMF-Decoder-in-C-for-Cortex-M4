# DTMF-Decoder-in-C-for-Cortex-M4

This document provides a comprehensive overview of the DTMF decoder project designed for the LPC4088 microcontroller with Cortex M4 CPU.

## Abstract

The system described processes Dual-Tone Multi-Frequency (DTMF) signals from audio sources to display the corresponding characters on an LCD screen. Users can calibrate the system for optimal performance and select the processing algorithm (Goertzel or FFT) through a dedicated settings page. The LCD supports automatic scrolling for character display, with manual navigation enabled via joystick controls.

## Introduction

DTMF signals offer a reliable method for transmitting a limited set of information over noisy and bandwidth-constrained channels. This project demonstrates a proof-of-concept DTMF decoder system, ideal for environments where traditional phone networks are unavailable or impractical. The ARM MDK served as the primary development environment, utilizing various libraries and drivers provided by the MDK, Labs, and occasionally GitHub. Code was written in C, adhering to C standards for naming conventions and file/function/variable syntax.

This document delves into each system component, providing a detailed explanation of its functionality and contribution to the overall system.

## System Design

    * Circuit
        * The circuit design references a handbook schematic (Figure 1) with adjustments for readily available components (Figure 2).
        * The left channel of the AUX input serves as the input signal, grounded to the board's ground (GROUND PIN 0V).
        * A 100Ω resistor and 100nF capacitor form a low-pass filter at the input to condition the signal.
        * Two 1N4148 diodes and 10kΩ resistors shift the signal upward for positive ADC readings.
        * The ADC on the LPC4088 board can only read positive values. Negative portions of the signal would be lost, resulting in inaccurate processing.

    * Reading
        * An essential module, the data collection module acquires and stores analog signals at a specific sampling rate. The main application receives a continuous stream of data from this module for further processing.
        * The module operates at a sampling rate of 8000 Hz (SAMPLE_RATE) to comply with the Nyquist-Shannon theorem, ensuring sufficient capturing of all frequency components in DTMF signals.
        * The size of the data arrays (main_array and secondary_array) is set to 512 elements for efficient FFT processing and a sampling window of 0.064 seconds, proven sufficient through MATLAB simulations (Figure 5).
        * Analog data is acquired from the ADC, reading the voltage level provided by the circuit. The read() function utilizes a double-buffering technique for uninterrupted data acquisition (Figure 6).
        * The module manages initialization and configuration of calibration settings for the analog mask. The calibration process and user interface are covered in the "User Interface" section.
        Performance measurements indicate minimal time cost for reading and saving operations within this module.

    * Fast Fourier Transform (FFT)
        * The Cooley-Tukey algorithm was used for the FFT implementation, with adaptations for the board.
        * Python scripts were developed for testing and verification of the FFT (Figures 7 & 8).
        * The final FFT implementation was measured at 412 ticks, translating to 52 ms.

    * Goertzel Algorithm
        * This approach calculates the magnitudes of DTMF frequencies to determine the dominant frequencies for decoding.
        * The implementation is exceptionally fast and avoids external libraries, making it suitable for resource-constrained hardware.
        * The provided reference (https://www.embedded.com/the-goertzel-algorithm/) offers a detailed explanation of the algorithm.
        * The implementation successfully decoded tones with a signal-to-noise ratio as low as 50% (measured through crafted audio files).
        * Both Goertzel functions return before a window of samples is read from the input signal, ensuring a seamless sampling process.
        * Execution time measurements indicate that both functions require approximately 15 ms, significantly faster than the reader, guaranteeing no decoding delays.
        * An example (Figure 9) showcases the effectiveness of the Goertzel algorithm in identifying the correct DTMF frequencies even with added noise.

    * Frequencies Comparator
        * The frequencies_comparator module receives the tone frequencies (high and low) from the decoding stage. Its primary function involves determining the corresponding key by comparing these frequencies with the DTMF matrix.
        * The module can identify various error scenarios, including no DTMF signal detected or multiple DTMF signals present simultaneously.
        * It implements duplicate key prevention and sequence management to ensure accurate character sequence construction.
        * The DTMF matrix (Figure 10) is used for key recognition based on high and low frequency comparisons.


# Contributors

 - G103L3 (Gioele Giunta)
 - G C (Anonym)
 - J E (Anonym)

# Copyright Notice

Copyright (c) 2024 Gioele Giunta

All rights reserved. No part of this work may be reproduced, distributed, or transmitted in any form or by any means, including photocopying, recording, or other electronic or mechanical methods, without the prior written permission of the author.
