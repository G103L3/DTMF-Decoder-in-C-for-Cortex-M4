/*! \file eeprom_g3.h
* \brief Custom interface for communicating with the board's EEPROM 
* This was adpated from NXP MCU SW Application Team's EEPROM firmware library this firmware library was adapted 
* slightly to work with the given LPC_4088 board and the custom interface was created to simplify the usage.
*/

#ifndef __EEPROM3_H_
#define __EEPROM3_H_

/**
 * \brief Writes a value to the EEPROM
 * \param value The value to be written to the EEPROM
 * \returns void
 * 
 * This function simplifies writing to EEPROM, it takes an integer value and before writing Erases the EEPROM page we are writing to.
 * Then it writes to a predefined page and address in EEPROM.
 */
void write_eeprom_g3(int value);

/**
 * \brief Reads a value from the EEPROM
 * \returns The value read from the EEPROM
 * 
 * This function also simplifies reading from EEPROM as it just reads the value we care about from a predefined page and address in EEPROM.
 */
int read_eeprom_g3(void);

#endif 
