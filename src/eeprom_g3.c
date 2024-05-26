/*! \file eeprom_g3.c
* \brief Functions for eeprom_g3.h
*/

#include "eeprom_g3.h"
#include "lpc_types.h"
#include "eeprom.h"

/**
 * \brief Writes a value to the EEPROM
 * \param value The value to be written to the EEPROM
 * \returns void
 * 
 * This function simplifies writing to EEPROM, it takes an integer value and before writing Erases the EEPROM page we are writing to.
 * Then it writes to a predefined page and address in EEPROM.
 */
void write_eeprom_g3 (int value)
{
	uint8_t data = value & 0xFF;	/* Ensure the value is within 8-bit range */

	EEPROM_Erase(0);
	EEPROM_Write(0, 0, &data, MODE_8_BIT, 1);
}

/**
 * \brief Reads a value from the EEPROM
 * \returns The value read from the EEPROM
 * 
 * This function also simplifies reading from EEPROM as it just reads the value we care about from a predefined page and address in EEPROM.
 */
int read_eeprom_g3 (void)
{
    uint8_t data;

    EEPROM_Read(0, 0, &data, MODE_8_BIT, 1);	/* Reading from page offset 0, page address 0 */

    return data;	/* Return the read value */
}
