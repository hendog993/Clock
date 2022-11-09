/* Filename: changeColorMode.h
 *
 * Author: Henry Gilbert
 *
 * Date: 7 October  2022
 *
 * Description:  Public interface to change color mode state functions
 *
 *
 *
 */


#ifndef CHANGE_COLOR_MODE_H
#define CHANGE_COLOR_MODE_H

/**************************** EEPROM Memory Addresses ********************************/
#define EEPROM_ADDR_DIGIT_RED 0x7000u
#define EEPROM_ADDR_DIGIT_GREEN 0x7001u
#define EEPROM_ADDR_DIGIT_BLUE 0x7002u
#define EEPROM_ADDR_BGND_RED 0x7003u
#define EEPROM_ADDR_BGND_GREEN 0x7004u
#define EEPROM_ADDR_BGND_BLUE 0x7005u
#define EEPROM_ADDR_CRC_HB  0x7006u
#define EEPROM_ADDR_CRC_LB  0x7007u

#define NUM_BYTES_EEPROM_BLOCK 8u

#include <stdbool.h>


/* Function:
 *      COLOR_MODE_ChangeColor
 *
 * Description:
 *      Iterates through the colors and allows the user to cycle through the available
 *      colors. Pushing the rotary encoder button allows the user to save the current
 *      color to the current option (background or digit).
 */
bool STATE_ChangeColorMode(void);

#endif

/* end changeColorMode.h header file*/
