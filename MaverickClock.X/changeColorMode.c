/* Filename: colorChangeMode.c
 *
 * Date:  12 September 2022
 *
 * Author: Henry Gilbert
 *
 * Description: Mode entered when a double click on rotary encoder is detected. Allows the user
 *      to update the RGB color values of both background, digits, and colon.
 *
 *
 * Mode requirements:
 *      Rotary encoder changes RGB value of "color segment".
 *      Color segments all start at white,
 *
 *
 */

/*********************** Included File(s) ********************************/
#include "changeColorMode.h"
#include "rotaryEncoder.h"
#include <string.h>
#include "clockLEDs.h"
#include "ws2812b.h"
#include "CRC16bit.h"
#include "mcc_generated_files/memory.h"
#include <xc.h>
#include "timeCalculation.h"

/*********************** Macro Definition (s) ****************************/
#define NUM_COLOR_ARRAY_ROWS 16u
#define limit(val, low, high) ( (val < low) ? high : (val > high) ? low : val)
#define NUM_BYTES_IN_RGB_PIXEL 3u

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 32000000u
#endif


/*********************** Local Variables (s) ****************************/
/* Create local arrays stack allocated for color choices. */
static const uint8_t colorRGBArrays[NUM_COLOR_ARRAY_ROWS][NUM_BYTES_IN_RGB_PIXEL] = {
    { 255u, 0u, 0u}, // Pure red
    { 0u, 255u, 0u}, // Pure green
    { 0u, 0u, 255u}, // Pure blue
    {255u, 0, 255u}, // Magenta
    {255u, 255u, 0u}, // Yellow
    {0xFFu, 0xA5u, 0x0u}, // Orange
    {0x0u, 0xFFu, 0xFFu}, // Cyan
    {0xFFu, 0xFFu, 0xFFu}, // White
    {0u, 0u, 0u}, // Black
    {0u, 80u, 239u}, // Cobalt
    {240u, 163u, 10u}, // Auburn
    {161u, 210u, 105u}, // Easter Green
    {153u, 76u, 0u}, // Burnt orange
    {127u, 255u, 0u}, // Lime green
    {255u, 69u, 0u}, // Orange red
};

/************************** Functions ************************************/


bool STATE_ChangeColorMode( void )
{
    Clock_DoubleFlashWithClearScreen();
 
    /* Clear LEDs and render clear */
    Clock_SetBackgroundRGBArray( 0u, 0u, 0u );
    const TimeInDigits dColorTime = {
        .digit1 = 0,
        .digit2 = 0,
        .digit3 = 0,
        .digit4 = 0,
    };
    Clock_PrerenderPixelAndBackgroundValues( );
    Clock_ForceTimeRender( &dColorTime );

    RotaryEncoder_HasButtonPressOccurred( ); // Clear a possible button pressed flag

    int32_t rotCounts;
    int8_t arrayRowIdx = 0; // TODO should this be size_t?.

    /* Digits */
    do
    {
        rotCounts = RotaryEncoder_GetShaftCounts( );
        // Increment color source based on counts.
        if( 0 != rotCounts )
        {
            arrayRowIdx += rotCounts;
            arrayRowIdx = limit( arrayRowIdx, 0, (int8_t) NUM_COLOR_ARRAY_ROWS );
            Clock_SetDigitRGBArray( colorRGBArrays[arrayRowIdx][0],
                                    colorRGBArrays[arrayRowIdx][1],
                                    colorRGBArrays[arrayRowIdx][2] );
            Clock_PrerenderPixelAndBackgroundValues( );
            Clock_ForceTimeRender( &dColorTime );
        }
    }
    while( false == RotaryEncoder_HasButtonPressOccurred( ) );

    /* Once a button press is detected, save the current arrayRowIdx for color selection */

    uint8_t digitSelection[NUM_BYTES_IN_RGB_PIXEL] = {
        colorRGBArrays[arrayRowIdx][0],
        colorRGBArrays[arrayRowIdx][1],
        colorRGBArrays[arrayRowIdx][2]
    };

    RotaryEncoder_HasButtonPressOccurred( ); // Clear a possible button pressed flag initially.
    /* Background selection */
    do
    {
        rotCounts = RotaryEncoder_GetShaftCounts( );
        // Increment color source based on counts.
        if( 0 != rotCounts )
        {
            arrayRowIdx += rotCounts;
            arrayRowIdx = limit( arrayRowIdx, 0, (int8_t) NUM_COLOR_ARRAY_ROWS );
            Clock_SetBackgroundRGBArray( colorRGBArrays[arrayRowIdx][0],
                                         colorRGBArrays[arrayRowIdx][1],
                                         colorRGBArrays[arrayRowIdx][2] );
            Clock_PrerenderPixelAndBackgroundValues( );
            Clock_ForceTimeRender( &dColorTime );
        }
    }
    while( false == RotaryEncoder_HasButtonPressOccurred( ) );

    uint8_t backgroundSelection[NUM_BYTES_IN_RGB_PIXEL] = {
        colorRGBArrays[arrayRowIdx][0],
        colorRGBArrays[arrayRowIdx][1],
        colorRGBArrays[arrayRowIdx][2]
    };

    /* Save the results to EEPROM and recalculate CRC of color region */
    uint16_t crc = CRC16_Calculate16bitCRC( digitSelection, NUM_BYTES_IN_RGB_PIXEL, 0xFFFF );
    crc = CRC16_Calculate16bitCRC( backgroundSelection, NUM_BYTES_IN_RGB_PIXEL, crc ); // Second part.

    /* Read back and verify the CRC */
    size_t maxRetries = 3;
    uint8_t eepromReadbackData[NUM_BYTES_EEPROM_BLOCK];
    bool wasReadbackValid = false;
    while( maxRetries )
    {
        /* Write to EEPROM */
        DATAEE_WriteByte( EEPROM_ADDR_DIGIT_RED, digitSelection[0] );
        DATAEE_WriteByte( EEPROM_ADDR_DIGIT_GREEN, digitSelection[1] );
        DATAEE_WriteByte( EEPROM_ADDR_DIGIT_BLUE, digitSelection[2] );
        DATAEE_WriteByte( EEPROM_ADDR_BGND_RED, backgroundSelection[0] );
        DATAEE_WriteByte( EEPROM_ADDR_BGND_GREEN, backgroundSelection[1] );
        DATAEE_WriteByte( EEPROM_ADDR_BGND_BLUE, backgroundSelection[2] );
        DATAEE_WriteByte( EEPROM_ADDR_CRC_HB, (uint8_t) ( ( crc & 0xFF00u ) >> 8u ) );
        DATAEE_WriteByte( EEPROM_ADDR_CRC_LB, (uint8_t) ( crc & 0xFFu ) );

        __delay_ms( 200 );

        eepromReadbackData[0] = DATAEE_ReadByte( EEPROM_ADDR_DIGIT_RED );
        eepromReadbackData[1] = DATAEE_ReadByte( EEPROM_ADDR_DIGIT_GREEN );
        eepromReadbackData[2] = DATAEE_ReadByte( EEPROM_ADDR_DIGIT_BLUE );
        eepromReadbackData[3] = DATAEE_ReadByte( EEPROM_ADDR_BGND_RED );
        eepromReadbackData[4] = DATAEE_ReadByte( EEPROM_ADDR_BGND_GREEN );
        eepromReadbackData[5] = DATAEE_ReadByte( EEPROM_ADDR_BGND_BLUE );
        eepromReadbackData[6] = DATAEE_ReadByte( EEPROM_ADDR_CRC_HB );
        eepromReadbackData[7] = DATAEE_ReadByte( EEPROM_ADDR_CRC_LB );

        if( CRC16_Calculate16bitCRC( eepromReadbackData, NUM_BYTES_EEPROM_BLOCK, 0xFFFF ) == 0u )
        {
            wasReadbackValid = true;
            break;
        }
        maxRetries--;
    }

    if( wasReadbackValid )
    {
        /* TODO flash screen/colon green. Prerender screen routines. */
    }
    else
    {
        /* TODO flash screen red. return false; */
    }

    /* Rewrite time with previous valid time */
    uint32_t currentTime = Time_GetCurrentTimeInSeconds( );
    Time_RenderInputTime( currentTime, false );
    return wasReadbackValid;
}
