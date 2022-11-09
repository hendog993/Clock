/* 
 * Filename: CRC16bit.h
 * 
 * Author: Henry  Gilbert
 * 
 * Date: 8 April 2022 
 * 
 * Description: Function declaration for 16bit CRC.
 * 
 * 
 */

#ifndef CRC16BIT_H
#define CRC16BIT_H

/**************  Included File(s) **************************/
#include <stdint.h>
#include <stddef.h>

/**************  Function Prototype(s) *********************/
uint16_t CRC16_Calculate16bitCRC(const uint8_t* const data, /* Pointer to data array */
                                 const size_t size, /* Size of input array */
                                 const uint16_t seed); /* CRC seed value */

#endif
/* End of CRC16bit.h header file */