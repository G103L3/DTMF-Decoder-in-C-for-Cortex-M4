/*! \file reader.c
 * \author Gioele Giunta
 * \version 1.3
 * \since 16<sup>th</sup> april 2024
 * \brief Functions for reader.h
 */

/* Header */
#include "reader.h"

/* Function Headers */
void read(void);
void swap_array(void);
void SysTick_Handler(void);

/* Variable Declarations */
complex_g3_t *current_data; //Pointer to the operative array
complex_g3_t *array_ready; //Array to transmit back to the main
int current_array;  // Even for main_array, Odd for secondary_array
complex_g3_t main_array[ARRAY_ELEMENTS];
complex_g3_t secondary_array[ARRAY_ELEMENTS];
volatile double vadc;
volatile int res;
int counter, current_array, status_flag, data_ready; //data_ready is the flag
volatile uint32_t g_systick_counter;
double max_amplitude;
double half_amplitude;
int calibrated;

/* Function Headers */
void read(void);

/*! \fn void reader_init(void)
* \brief Initializes the ADC, LCD, SysTick, and sets the initial memory state.
*
* This function initializes all the initial values of the reader.
* By initializing the systick, it causes the read function to be called as a handler of the systick.
*/
void reader_init(void)
{
	/* Initializing Variables */
	current_array = 0;
	counter = 0;
	data_ready = 0;
	status_flag = 1;
	g_systick_counter = 0;
	calibrated = 0;
	max_amplitude = G_MAX_AMPLITUDE;
	half_amplitude = max_amplitude/2.0;
	adc_init();
	/* Set initial array to the main_array */
	swap_array();
	/* Initializing Systick -> Systick instead of Timer to reduce Code Complexity */
	SysTick_Config(SystemCoreClock / SAMPLE_RATE);
}

/*! \fn void SysTick_Handler(void)
* \brief Interrupt handler for the SysTick timer.
*
* This function is the interrupt handler for the SysTick timer. 
* It increments the g_systick_counter and calls the read() function. 
* Prefer to use SysTick instead of Timer to reduce complexity of the assembly.
*/
void SysTick_Handler(void)
{
  g_systick_counter++;
	if(status_flag)
	{
		read();
	}
	else
	{
		leds_set(1, 0, 0);
	}
}

/*! \fn void read(void)
* \brief Reads the analog value from the ADC, stores it in the current data array, and checks if the array is full.
*
* This function reads the analog value from the ADC, stores it in the current data array, and checks if the array is full. 
* If so, it sets the global flag to true and the global array with the actual array. It also swaps the actual arrays and resets the counter.
* If the mask is not calibrated, the function calls the mask_calibrate() function to calibrate the mask. 
* Otherwise, it stores the analog value in the current data array.
* Using the SysTick timer can lead to a misalignment between the advancement of the reader function and the while reading of the caller function outside.
* To avoid this issue, the function sets the data_ready flag to true, which will be disabled by the main function, and sets the array_ready pointer to the current array,
* allowing the modification of current_data without affecting the main function.
*/
void read(void)
{
	/* Read from adc.c */
	res = adc_read(); 
	vadc = (((double)res * (double)max_amplitude) / ADC_MASK)-half_amplitude; //Read Removed the mask to have more detailed data
	
	/*If Mask is not calibrated call mask_calibrate */
	if(!calibrated)
	{
		mask_calibrate(max_amplitude, half_amplitude, (double)res, (double)vadc);
		leds_set(0, 1, 0);
	}
	else
	{
		/* Push inside the linked current_data array */
		current_data[counter].re = (double)vadc; 
		current_data[counter].im = (double)0.0;
		counter++;
	}
	  
	if (counter >= ARRAY_ELEMENTS)
	/* If the Array is Full swap and reset the counter */
	{
		/* Using a timing-based array fill makes the filling process asynchronous.
		* If there's a while loop in another function constantly monitoring the current_data array, the asynchronous filling
		* might lead to a situation where the array is full but the while loop misses that moment.
		* Consequently, the asynchronous filling loop assigns the current_data to an empty array, halting the reading process.
		* To address this issue, two variables are introduced: array_ready and data_ready.
		* array_ready points to the array referenced by current_data and remains set until current_data reaches the full state again,
		* pointing then to the new array referenced by current_data.
		*/
		data_ready = 1;
		array_ready = current_data;
		current_array++;
		swap_array();
		counter = 0;
    }
}

/*! \fn void swap_array(void)
* \brief Swaps the main and secondary data arrays.
*
* This function swaps the main and secondary data arrays.
*/
void swap_array (void)
{
	current_data = (current_array % 2 == 0) ? main_array : secondary_array; //Check witch array link to current_data base on current_array
}
