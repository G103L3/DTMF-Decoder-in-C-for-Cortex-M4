/*! \file frequencies_comparator.c
 * \author Gioele Giunta
 * \version 1.4
 * \since 21<sup>st</sup> March 2024
 * \brief Functions for frequencies_comparator.h
 */
 
/* Header */
#include "frequencies_comparator.h"

/* Function Headers */
/* + Assembly */
__asm int compare_higher_frequency(int *higher_frequency);
__asm int compare_lower_frequency(int *higher_frequency);
/* + C */
int add_char_to_sequence(char *sequence, char *recognized_key);

/*! \fn int frequencies_comparator(struct_tone_frequencies tone_frequencies, char *last_char, char *sequence)
* \param tone_frequencies The struct containing the high and low frequencies
* \param *last_char The pointer to the last_char, used to determine if there is a repetition or not
* \param *sequence The char Array where character will be saved
* \returns The status of operation 1 -> Tone detected, 0 -> Noise Detected/No operation by main required, <0 -> Error detected Operations from main Required
* \brief Compares the input frequencies to the DTMF frequency matrix and determines the corresponding key.
*
* This function takes in a struct containing the high and low frequencies of a DTMF signal, pointers to the last character detected and the character sequence. 
* It then determines the key that corresponds to the input frequencies and adds it to the character sequence, if it is a new character, also handles the case where OVERFLOW is detected. 
* The function also handles the case where no DTMF signal is detected or multiple DTMF signals are detected.
*/
int frequencies_comparator(struct_tone_frequencies tone_frequencies, char *last_char, char *sequence)
{
    char DTMF_keys_matrix[4][4] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
    };
    int column, row;
    char recognized_key;
		
    if(tone_frequencies.high == 0 || tone_frequencies.low == 0)
		/* Noise Detected */
		{
      *last_char = 'N'; 
			/* Case where no DTMF signal has been detected */
			return 0;
		}
		if(tone_frequencies.high == -1 || tone_frequencies.low == -1)
		/* Multitone Detected */
		{ 
      *last_char = 'N'; 
			/* Set MULTITONE ERROR */
			strcpy(error_text, "MULTITONE!");
			/* Case where more than one DTMF signal has been detected */
			return -2;
		}
		
		/* Call the Assembly Functions to obtain the keys of the DTMF matrix */
    column = compare_higher_frequency(&tone_frequencies.high);
    row = compare_lower_frequency(&tone_frequencies.low);

    if(column == -1 || row == -1)
		/* Noise Detected */
		{
        *last_char = 'N';
        return 0;
    }
		else
		{
				/* Find in the matrix the right tone using row and column */
        recognized_key = DTMF_keys_matrix[row][column];
        if(recognized_key != *last_char)
				{
						if(add_char_to_sequence(sequence, &recognized_key))
						{
							/* Character successfully added to sequence, proceede memorizing last_char */
							*last_char = recognized_key;
							return 1;
						}
						else
						{
							/* Throw error the addChar returned 0 so Overflow occured Error: -1 */
							return -1; 
						}
        }
				else
				{
            /* In case of last_char repetition no operations! */
            return 0;
        }
    }
}

/*! \fn int add_char_to_sequence(char *sequence, char *recognized_key)
* \brief Adds a character to the character sequence.
*
* This function takes in a pointer to the character sequence and a pointer to the character to be added. It then adds the character to the sequence if there is space available, and returns the status of the operation.
* @param sequence The Array Address
* @param *recognized_key The pointer to the charater to add to the array
* @returns The status of operation 1 -> true, 0 -> Overflow Occured
*/
int add_char_to_sequence(char *sequence, char *recognized_key) 
{
    int array_length = strlen(sequence);
    if (array_length < G_SEQUENCE_LENGTH-2) 
		{
			sequence[array_length] = *recognized_key;
			sequence[array_length + 1] = '\0';
      return 1;
    }
		else
		{
			/* Set OVERFLOW ERROR */
			strcpy(error_text, "OVERFLOW!");
			/* Case where the sequence is in Overflow */
			return 0;
		}
}

/*! \fn int compare_higher_frequency(int *higher_frequency)
* \brief Compares the higher frequency [1209, 1336, 1477, 1633] of keys [1,2,3,4] and returns the key.
*
* This function takes in a pointer to the higher frequency value and compares it to the predefined DTMF higher frequencies. It then returns the corresponding key.
*
* @param higher_frequency The address of the higher frequency value.
* @returns The key corresponding to the higher frequency value.
*/
__asm int compare_higher_frequency(int *higher_frequency)
{
	  /*
        In ASSEMBLY an INT is a 32BIT -> 4BYTE variable As a 32 bit signed integer the 2^32 values range from 
        -2,147,483,648 thru 2,147,483,647 so  will take one register.
    */

    LDR r2, [r0] // Load all 32bit into r2 from memory pointed to by r0 (higher_frequency)

START_SWITCH_H

    /* 
	  ___________________________________________________________________________________________________________
    |   For this switch case, I preferred the use of ITT, based on the Cortex M4 manual, over JUMP (BEQ),      |
    |   to perform fewer jumps in the program counter.																											   |
    |__________________________________________________________________________________________________________|
    |  From Cortex M4 manual I denote how each constant (decimal, hexadecimal) preceded by # has a size 8 bits,|
    |  so there is just 0-255 possible numbers. This forced me to have to use register 3, which has 32 bits		 |
	  |__________________________________________________________________________________________________________|
    */


    //case 1209
    MOV r3, #0x000004B9 //Assignment of the appropriate constant to r3
    CMP r2, r3 //To being true I need r2 = 1209
    ITT EQ //I indicate that there are 2 lines for true
    MOVEQ r0, #0 //If true r0 is 0 -> return will be 0 
    BEQ END_SWITCH_H //Go to the end of this switch

    //case 1336
    MOV r3, #0x00000538 //Assignment of the appropriate constant to r3
    CMP r2, r3 //To being true I need r2 = 1336
    ITT EQ //I indicate that there are 2 lines for true
    MOVEQ r0, #1 //If true r0 is 1 -> return will be 1
    BEQ END_SWITCH_H //Go to the end of this switch

    //case 1477 
    MOV r3, #0x000005C5 //Assignment of the appropriate constant to r3
    CMP r2, r3 //To being true I need r2 = 1477
    ITT EQ //I indicate that there are 2 lines for true
    MOVEQ r0, #2 //If true r0 is 2 -> return will be 2
    BEQ END_SWITCH_H //Go to the end of this switch

    //case 1633 
    MOV r3, #0x00000661 //Assignment of the appropriate constant to r3
    CMP r2, r3 //To being true I need r2 = 1633
    ITT EQ //I indicate that there are 2 lines for true
    MOVEQ r0, #3 //If true r0 is 3 -> return will be 3
    BEQ END_SWITCH_H //Go to the end of this switch

    //default
    MOV r0, #-1 //By default r0 is -1, it means that there are no correspondeces 
    
END_SWITCH_H

    BX lr //Return to the caller (lr = caller address)

/*
    The Return variable is found in r0
*/

}


/*! \fn int compare_lower_frequency(int *lower_frequency)
* \brief Compares the lower frequency [697, 770, 852, 941] of keys [1,2,3,4] and returns the key.
* This function takes in a pointer to the lower frequency value and compares it to the predefined DTMF lower frequencies. It then returns the corresponding key.
*
* @param lower_frequency The address of the lower frequency value.
* @returns The key corresponding to the lower frequency value.
*/
__asm int compare_lower_frequency(int *lower_frequency)
{
    /*
    ___________________________________________________________________________________________________________________
    |   All Design choices in this code are the same as the Design choices made in the code: compare_higher_frequency  |
    |__________________________________________________________________________________________________________________|
    */

    LDR r2, [r0] // Load all 32bit into r2 from memory pointed to by r0 (lower_frequency)

START_SWITCH_L

    //case 697
    MOV r3, #0x000002B9 //Assignment of the appropriate constant to r3
    CMP r2, r3 //To being true I need r2 = 697
    ITT EQ //I indicate that there are 2 lines for true
    MOVEQ r0, #0 //If true r0 is 0 -> return will be 0 
    BEQ END_SWITCH_L //Go to the end of this switch

    //case 770
    MOV r3, #0x00000302 //Assignment of the appropriate constant to r3
    CMP r2, r3 //To being true I need r2 = 770
    ITT EQ //I indicate that there are 2 lines for true
    MOVEQ r0, #1 //If true r0 is 1 -> return will be 1
    BEQ END_SWITCH_L //Go to the end of this switch

    //case 852 
    MOV r3, #0x00000354 //Assignment of the appropriate constant to r3
    CMP r2, r3 //To being true I need r2 = 852
    ITT EQ //I indicate that there are 2 lines for true
    MOVEQ r0, #2 //If true r0 is 2 -> return will be 2
    BEQ END_SWITCH_L //Go to the end of this switch

    //case 941
    MOV r3, #0x000003AD //Assignment of the appropriate constant to r3
    CMP r2, r3 //To being true I need r2 = 941
    ITT EQ //I indicate that there are 2 lines for true
    MOVEQ r0, #3 //If true r0 is 3 -> return will be 3
    BEQ END_SWITCH_L //Go to the end of this switch

    //default
    MOV r0, #-1 //By default r0 is -1, it means that there are no correspondeces 
    
END_SWITCH_L

		BX lr //Return to the caller (lr = caller address)
/*
    The Return variable is found in r0
*/

}


// ******************************* Gioele Giunta University Of Malta *************************************
