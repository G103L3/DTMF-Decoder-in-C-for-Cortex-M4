/*! \file lcd.h
 * \copyright ARM University Program &copy; ARM Ltd 2014.
 *
 * Modified for use with LPC4088 experiment bundle;
 * Copyright 2016-2017 Johann A. Briffa
 *
 * Modified by Group 3 to fit the group's needs
 * \brief Driver for Hitachi HD44780 LCD controller in 4-bit mode.
 */
#ifndef LCD_H
#define LCD_H

/*! \brief Initialises the LCD module.
 */
void lcd_init(void);

/*! \brief Moves the cursor position to the specified location.
 *  \param column  Destination column for the cursor.
 *  \param row     Destination row for the cursor.
 */
void lcd_set_cursor(int column, int row);

/*! \brief Clears the LCD and relocates the cursor to {0,0}.
 */
void lcd_clear(void);

/*! \brief Prints the null terminated string to the LCD and
 *         increments the cursor.
 *  \param string  Null terminated string to be printed.
 */
void lcd_print(char *string);

/*! \brief Prints the specified character to the LCD and
 *         increments the cursor.
 *  \param c  Character to be printed.
 */
void lcd_put_char(char c);

/*! \brief Enables or disables visibility of the cursor.
 *  \param visible New visibility of the cursor.
 */
void lcd_set_cursor_visibile(int visible);

/** \brief Prints a string to the top row of the LCD.
 *  \param string Pointer to the null terminated string to be printed.
*/
void print_top(char *string);

/**
 * \brief Prints a string to the top row of the LCD, with an offset.
 * \param string Pointer to the null terminated string to be printed.
 * \param offset The offset to be applied to the string.
 */
void print_top_dynamic(char *string, int offset);

/** \brief Prints a string to the bottom row of the LCD.
 *  \param string Pointer to the null terminated string to be printed.
*/
void print_bottom(char *string);

#endif // LDC_H
