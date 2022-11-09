/* Filename:
 *      ledArray.c
 *
 * Date:
 *      12 September 2022
 *
 * Author:
 *      Henry Gilbert
 *
 * Description:
 *      Implementation/application specific  layer built on the ws2812b.c API. This file in particular describes the
 *      LED array instance used by the clock.
 *
 *      Display time mode: Contains the static render buffer as well as the ledArray
 *      structure. Uses prerending of designated digits and backgrounds to save clock cycles at runtime.
 *      At runtime, the corresponding time struct populates the render buffer. Only digits that have
 *      changed from the previous render are written to the render buffer. A "lookup table" is used
 *      from the prerendered values to save cycles. This way, at runtime, the corresponding pixels
 *      are simply copied from the prerendered lookup table. If the user changes the color configuration,
 *      the prerender function must be called again.
 *
 *      Digit 4 explanation: Digit 4 is annoying since it has 1 less pixel than every other digit, and
 *      must be treated differently. Because of this, the up encoded pixel buffer must have 1 more value
 *      than required. After it is prerendered, it requires a modification to the written pixel buffer
 *      in order to display the correct digit.
 *
 *      Color change mode: Features a force render function. Color change mode has authority to rewrite the
 *      background colors and pixel buffers.
 *
 *      Pattern mode: Manually manipulates the screen buffer.
 *
 *
 */

/*********************** Included File(s) ********************************/
#include "clockLEDs.h"
#include "ws2812b.h"
#include <string.h>
#include <xc.h>
#include <stdlib.h>


/*********************** Macro Definition(s) *****************************/
#ifndef _XTAL_FREQ
#define _XTAL_FREQ 32000000u
#endif

#define TOTAL_NUM_DIGITS 10u
#define NUM_PIXELS_PER_DIGIT 14u // Note: digit 4 only has 13 pixels.
#define NUM_PIXELS_DIGIT_4_OFFSET 3u

#define DIGIT4_START_FIX_ADDRESS 61u
#define NUM_PIXELS_TO_FIX_DIGIT4 3u

/*********************** Local Variable(s) *******************************/
static __pack ws2812bPixel renderBuffer [NUM_CLOCK_PIXELS]; // actual render buffer
static ws2812bArray ledArray; // Local instance of led strip/array. Linked to renderBuffer



/* Start indices of each respective digit */
static const size_t digit1StartPixel = 1;
static const size_t digit2StartPixel = 16;
static const size_t digit3StartPixel = 36;
static const size_t digit4StartPixel = 51;


/* These local arrays allow the user to change RGB values of digits and background at runtime*/
static uint8_t currentDIGITrgbArray[NUM_BYTES_IN_PIXEL]; // Populated at startup
static uint8_t currentBACKGROUNDrgbArray[NUM_BYTES_IN_PIXEL]; // Populated at startup



/* Statically allocated prerendered digit buffers. This allows simply copying the desired
 * values over to the render buffer at runtime instead of calculating each RGB pixel value. If the
 * background color or digit color ever changes, these must be recalculated before time display mode starts.
 * Up encoded digits (1 and 4) are the same, as are the down encoded digits (2 and 3). Digit 4 requires
 * special modification after copying the indexed pixel buffer from the upEncodeDigitsPixelValues buffer.
 *
 * At runtime, to write to the render buffer, simply use the prerendered array as a source and enter the starting digit into the
 * function WS2812b_CopyPixelBufferArrayFromSource.
 */
static __pack ws2812bPixel upEncodedDigitsPixelValues[TOTAL_NUM_DIGITS][NUM_PIXELS_PER_DIGIT]; // Digits 1 and 4
static __pack ws2812bPixel downEncodedDigitsPixelValues[TOTAL_NUM_DIGITS][NUM_PIXELS_PER_DIGIT]; // Digits 2 and 3


/* Digit encodings:
 * Digits are encoded based on the starting pixel. Digits 1 and 4 have equal encodings, with
 * digit 4's last bit being zero. All digits are encoded MSB first.
 *
 * Digits 1 and 4 are "up". Digits 2 and 3 are "down". Last two bits are ALWAYS zero.
 */

static const uint16_t upDigitEncodings[TOTAL_NUM_DIGITS] = {
    0b1101110001010100, // zero
    0b0000010101010100, // one
    0b1110110101000100, // two
    0b1010110101010100, // three
    0b1011100110000100, // four
    0b1011110101010000, // five
    0b1110011101010000, // six
    0b1000110110000100, // seven
    0b1111110101010100, // eight
    0b1011110110000100 // nine
};

static const uint16_t downDigitEncodings[TOTAL_NUM_DIGITS] = {
    0b1101110001010100, // zero
    0b0000010101010100, // one
    0b1011110101010000, // two
    0b1010110101010100, // three
    0b1110101100010000, // four
    0b1110110101000100, // five
    0b0011110111000100, // six
    0b1000101101010000, // seven
    0b1111110101010100, // eight
    0b1110101101010000 // nine
};


/*********************** Function Prototype(s) ***************************/
static void Clock_WriteDigitPixelValuesToLocalPxlBuffers( ws2812bPixel * const pixelBuffer,
                                                          const uint16_t * encodingSource );

static void Clock_FixDigit4LastThreePixels( const uint8_t digit4Value );

/************************** Functions ************************************/

bool Clock_InitializeClockLEDs( const size_t numElements,
                                const uint8_t digitRed,
                                const uint8_t digitGreen,
                                const uint8_t digitBlue,
                                const uint8_t backgroundRed,
                                const uint8_t backgroundGreen,
                                const uint8_t backgroundBlue )
{
    Clock_SetDigitRGBArray( digitRed,
                            digitGreen,
                            digitBlue );

    Clock_SetBackgroundRGBArray( backgroundRed,
                                 backgroundGreen,
                                 backgroundBlue );
    bool returnVal;
    ledArray = WS2812b_Initialize( renderBuffer, numElements, &returnVal );
    return returnVal;
}

void Clock_SetBackgroundRGBArray( const uint8_t red,
                                  const uint8_t green,
                                  const uint8_t blue )
{
    currentBACKGROUNDrgbArray[0] = red;
    currentBACKGROUNDrgbArray[1] = green;
    currentBACKGROUNDrgbArray[2] = blue;
}

void Clock_SetDigitRGBArray( const uint8_t red,
                             const uint8_t green,
                             const uint8_t blue )
{
    currentDIGITrgbArray[0] = red;
    currentDIGITrgbArray[1] = green;
    currentDIGITrgbArray[2] = blue;
}

void Clock_WriteTimeDigitValuesAndRenderScreen( const TimeInDigits * const digits )
{
    static uint8_t lastDigit1 = 0xFF; // The digits could be zero, so initially make them 0xFF so that zeros will render accordingly.
    static uint8_t lastDigit2 = 0xFF;
    static uint8_t lastDigit3 = 0xFF;
    static uint8_t lastDigit4 = 0xFF;

    /* Only write to the render buffer if the respective digit has changed. */
    if( digits->digit1 != lastDigit1 )
    {
        lastDigit1 = digits->digit1;
        WS2812b_CopyPixelBufferArrayFromSource( &ledArray,
                                                upEncodedDigitsPixelValues[digits->digit1],
                                                digit1StartPixel,
                                                NUM_PIXELS_PER_DIGIT );
    }
    if( digits->digit2 != lastDigit2 )
    {
        lastDigit2 = digits->digit2;
        WS2812b_CopyPixelBufferArrayFromSource( &ledArray,
                                                downEncodedDigitsPixelValues[digits->digit2],
                                                digit2StartPixel,
                                                NUM_PIXELS_PER_DIGIT );
    }
    if( digits->digit3 != lastDigit3 )
    {
        lastDigit3 = digits->digit3;
        WS2812b_CopyPixelBufferArrayFromSource( &ledArray,
                                                downEncodedDigitsPixelValues[digits->digit3],
                                                digit3StartPixel,
                                                NUM_PIXELS_PER_DIGIT );
    }
    if( digits->digit4 != lastDigit4 )
    {
        lastDigit4 = digits->digit4;
        /* Digit 4 has 1 less pixel than every other digit, along with a special encoding, so a
         * helper function to modify the render buffer back to normal must be called.
         *  */
        WS2812b_CopyPixelBufferArrayFromSource( &ledArray,
                                                upEncodedDigitsPixelValues[digits->digit4],
                                                digit4StartPixel,
                                                NUM_PIXELS_PER_DIGIT - 4 ); // The last 4 pixels are written separately

        // Fix digit 4's final three pixels.
        Clock_FixDigit4LastThreePixels( digits->digit4 );
    }
    WS2812B_Render( &ledArray );
    return;
}

void Clock_PrerenderPixelAndBackgroundValues( void )
{
    /* Write the entirety of the background colors to the render buffer */
    WS2812b_SetStripConstantColor( &ledArray,
                                   currentBACKGROUNDrgbArray[WS2812B_RED_INDEX],
                                   currentBACKGROUNDrgbArray[WS2812B_GREEN_INDEX],
                                   currentBACKGROUNDrgbArray[WS2812B_BLUE_INDEX] );

    /* Prerender up encoded digits to the local pixel buffers */
    Clock_WriteDigitPixelValuesToLocalPxlBuffers( upEncodedDigitsPixelValues[0],
                                                  upDigitEncodings );

    /* Prerender down encoded digits to the local pixel buffers */
    Clock_WriteDigitPixelValuesToLocalPxlBuffers( downEncodedDigitsPixelValues[0],
                                                  downDigitEncodings );

    return;
}

/* Function:
 *      Clock_WriteDigitPixelValuesToLocalPxlBuffers
 *
 * Description:
 *      Function called by prerenderer to populate the local pixel buffers with the
 *      correct colors based on the digit encodings. Uses the pixelBuffer input pointer
 *      as the destination of writing, and the encodingSource pointer as a source for
 *      logical writes.
 *
 *      Write logic:
 *      FOR every digit:
 *          FOR all bits of encoding:
 *              IF the encoding source MSB is 1
 *                  Copy the rgb array into the pixel buffer
 *              Increment pixel buffer destination ptr by 3 bytes
 *          Increment encoding source pointer by 1 value (2 bytes).
 *
 * Precondition: currentDIGITrgbArray and currentBACKGROUNDrgbArray must have been populated.
 *
 */
static void Clock_WriteDigitPixelValuesToLocalPxlBuffers( ws2812bPixel * const pixelBuffer,
                                                          const uint16_t * encodingSource )
{
    size_t digitCounter;
    size_t encodingCounter;
    uint8_t * thisDigitStartAddress = &( pixelBuffer->red );
    for( digitCounter = 0; digitCounter < TOTAL_NUM_DIGITS; digitCounter++ )
    {
        for( encodingCounter = 0; encodingCounter < NUM_PIXELS_PER_DIGIT; encodingCounter++ )
        {
            // if the encoding has a 1 value, write the RGB values to the pixel buffer.
            if( ( *encodingSource << encodingCounter ) & 0x8000u )
            {
                memcpy( thisDigitStartAddress, currentDIGITrgbArray, NUM_BYTES_IN_PIXEL );
            }
            else // Write background color
            {
                memcpy( thisDigitStartAddress, currentBACKGROUNDrgbArray, NUM_BYTES_IN_PIXEL );
            }
            thisDigitStartAddress += NUM_BYTES_IN_PIXEL;
        }
        encodingSource++;
    }
}

/* Function:
 *      Clock_FixDigit4LastThreePixels
 *
 * Description: Annoying fix for digit 4. Not the most elegant solution, but it's the
 *      only real work around based on memory constraints. Can't have an entire pixel buffer
 *      for every digit for3 pixels that can just be hard coded like so.
 */
static void Clock_FixDigit4LastThreePixels( const uint8_t digit4Value )
{
    /* Write the block of pixels from 61-63 the background color*/
    WS2812b_SetPixelBlockConstantColor( &ledArray,
                                        DIGIT4_START_FIX_ADDRESS,
                                        NUM_PIXELS_TO_FIX_DIGIT4,
                                        currentBACKGROUNDrgbArray[0],
                                        currentBACKGROUNDrgbArray[1],
                                        currentBACKGROUNDrgbArray[2] );

    /* 11 */
    if( ( 0u == digit4Value ) ||
        ( 1u == digit4Value ) ||
        ( 3u == digit4Value ) ||
        ( 8u == digit4Value ) )
    {
        WS2812b_SetSinglePixelColor( &ledArray,
                                     61,
                                     currentDIGITrgbArray[0],
                                     currentDIGITrgbArray[1],
                                     currentDIGITrgbArray[2] );
        WS2812b_SetSinglePixelColor( &ledArray,
                                     63,
                                     currentDIGITrgbArray[0],
                                     currentDIGITrgbArray[1],
                                     currentDIGITrgbArray[2] );
    }
        /* 10*/
    else if( ( 2u == digit4Value ) ||
             ( 7u == digit4Value ) ||
             ( 9u == digit4Value ) ||
             ( 4u == digit4Value ) )
    {
        WS2812b_SetSinglePixelColor( &ledArray,
                                     63,
                                     currentDIGITrgbArray[0],
                                     currentDIGITrgbArray[1],
                                     currentDIGITrgbArray[2] );
    }
        /* 01 */
    else if( ( 5u == digit4Value ) ||
             ( 6u == digit4Value ) )
    {
        WS2812b_SetSinglePixelColor( &ledArray,
                                     61,
                                     currentDIGITrgbArray[0],
                                     currentDIGITrgbArray[1],
                                     currentDIGITrgbArray[2] );
    }
    else
    {
    }
    return;
}

/****************************************** Color Change Mode Functions ************************************/

void Clock_ForceTimeRender( const TimeInDigits * const t )
{
    WS2812b_CopyPixelBufferArrayFromSource( &ledArray,
                                            upEncodedDigitsPixelValues[t->digit1],
                                            digit1StartPixel,
                                            NUM_PIXELS_PER_DIGIT );
    WS2812b_CopyPixelBufferArrayFromSource( &ledArray,
                                            downEncodedDigitsPixelValues[t->digit2],
                                            digit2StartPixel,
                                            NUM_PIXELS_PER_DIGIT );
    WS2812b_CopyPixelBufferArrayFromSource( &ledArray,
                                            downEncodedDigitsPixelValues[t->digit3],
                                            digit3StartPixel,
                                            NUM_PIXELS_PER_DIGIT );
    WS2812b_CopyPixelBufferArrayFromSource( &ledArray,
                                            upEncodedDigitsPixelValues[t->digit4],
                                            digit4StartPixel,
                                            NUM_PIXELS_PER_DIGIT - 4 ); // The last 4 pixels are written separately
    Clock_FixDigit4LastThreePixels( t->digit4 );
    WS2812B_Render( &ledArray );
    return;
}

void Clock_DoubleFlashWithClearScreen( void )
{
    WS2812b_SetStripConstantColor( &ledArray,
                                   0x80u,
                                   0x80u,
                                   0x80u );
    WS2812B_Render( &ledArray );
    __delay_ms( 50 );
    WS2812b_SetStripConstantColor( &ledArray,
                                   0x0u,
                                   0x0u,
                                   0x0u );
    WS2812B_Render( &ledArray );
    __delay_ms( 50 );
    WS2812b_SetStripConstantColor( &ledArray,
                                   0x80u,
                                   0x80u,
                                   0x80u );
    WS2812B_Render( &ledArray );
    __delay_ms( 50 );
    WS2812b_SetStripConstantColor( &ledArray,
                                   0x0u,
                                   0x0u,
                                   0x0u );
    WS2812B_Render( &ledArray );
    __delay_ms( 50 );
}

void Clock_SetScreenBlankAndRender( void )
{
    WS2812b_SetStripConstantColor( &ledArray,
                                   0x0u,
                                   0x0u,
                                   0x0u );
    WS2812B_Render( &ledArray );
}

/*********************************************************************************************************/
/***************************************** Pattern Mode Functions ****************************************/
/*********************************************************************************************************/

/* Note! All pattern mode functions must be designed as non-blocking. Each function should only render once
 * per function call. Use static variables to save the most recent state. This way, a pattern can be exited
 * at any point between renders and isn't stuck in the pattern if the user tries to change pattern. 
 */


void Clock_IterateSinglePixelByIndex( void )
{
#define MAX_IDX_VALUE 63
    static size_t idx = 0;

    WS2812b_SetSinglePixelColor( &ledArray,
                                 idx,
                                 0xFF,
                                 0xFF,
                                 0xFF );
    WS2812B_Render( &ledArray );

    __delay_ms( 50 );
    if( MAX_IDX_VALUE == idx )
    {
        idx = 0;
        WS2812b_SetStripConstantColor( &ledArray,
                                       0u,
                                       0u,
                                       0u );
    }
    else
    {
        idx++;
    }
    return;
}


/******************************** LED Profile ******************************/
static const uint8_t row1[12] = {5, 6, 15, 16, 25, 26, 35, 36, 45, 46, 55, 56};
static const uint8_t row2[13] = {4, 7, 14, 17, 24, 27, 34, 37, 44, 47, 54, 57, 63};
static const uint8_t row3[14] = {0, 3, 8, 13, 18, 23, 28, 33, 38, 43, 48, 53, 58, 62};
static const uint8_t row4[13] = {2, 9, 12, 19, 22, 29, 32, 39, 42, 49, 52, 59, 61};
static const uint8_t row5[12] = {1, 10, 11, 20, 21, 30, 31, 40, 41, 50, 51, 60};

#define NUM_COLORS_IN_RAINBOW 6u
#define NUM_COLORS_PER_RGB_PIXEL 3u

static const uint8_t rainbowRGBValues[NUM_COLORS_IN_RAINBOW][NUM_COLORS_PER_RGB_PIXEL] = {
    {0xFFu, 0x00u, 0x00u}, // Red
    {255u, 69u, 0u}, // Orange
    {0xFFu, 0xFFu, 0x00u}, // Yellow
    {0x00u, 0xFFu, 0x00u}, // Green
    {0x00u, 0x00u, 0xFFu}, // Blue
    {0xFFu, 0x00u, 0xFFu} // Magenta
};

void Clock_CrossingRainbowPattern( )
{

#define MAX_LED_INDEX_RAINBOW 13
#define RAINBOW_INDEX_MAX 5
    static size_t thisIndex = 0;
    static size_t rainbowRowIdx = 0;


    /* thisIndex values of 0 only iterate middle row. */
    if( 0 == thisIndex )
    {
        WS2812b_SetSinglePixelColor( &ledArray,
                                     row3[thisIndex],
                                     rainbowRGBValues[rainbowRowIdx][0],
                                     rainbowRGBValues[rainbowRowIdx][1],
                                     rainbowRGBValues[rainbowRowIdx][2] );
    }

        /* thisIndex values of 1 iterate middle three rows. */
    else if( 1 == thisIndex )
    {
        WS2812b_SetSinglePixelColor( &ledArray,
                                     row2[thisIndex - 1],
                                     rainbowRGBValues[rainbowRowIdx][0],
                                     rainbowRGBValues[rainbowRowIdx][1],
                                     rainbowRGBValues[rainbowRowIdx][2] );
        WS2812b_SetSinglePixelColor( &ledArray,
                                     row3[thisIndex],
                                     rainbowRGBValues[rainbowRowIdx][0],
                                     rainbowRGBValues[rainbowRowIdx][1],
                                     rainbowRGBValues[rainbowRowIdx][2] );
        WS2812b_SetSinglePixelColor( &ledArray,
                                     row4[thisIndex - 1],
                                     rainbowRGBValues[rainbowRowIdx][0],
                                     rainbowRGBValues[rainbowRowIdx][1],
                                     rainbowRGBValues[rainbowRowIdx][2] );
    }

        /* thisIndex values of 2 and greater iterate all rows. */
    else
    {
        WS2812b_SetSinglePixelColor( &ledArray,
                                     row1[thisIndex - 2],
                                     rainbowRGBValues[rainbowRowIdx][0],
                                     rainbowRGBValues[rainbowRowIdx][1],
                                     rainbowRGBValues[rainbowRowIdx][2] );
        WS2812b_SetSinglePixelColor( &ledArray,
                                     row2[thisIndex - 1],
                                     rainbowRGBValues[rainbowRowIdx][0],
                                     rainbowRGBValues[rainbowRowIdx][1],
                                     rainbowRGBValues[rainbowRowIdx][2] );
        WS2812b_SetSinglePixelColor( &ledArray,
                                     row3[thisIndex],
                                     rainbowRGBValues[rainbowRowIdx][0],
                                     rainbowRGBValues[rainbowRowIdx][1],
                                     rainbowRGBValues[rainbowRowIdx][2] );
        WS2812b_SetSinglePixelColor( &ledArray,
                                     row4[thisIndex - 1],
                                     rainbowRGBValues[rainbowRowIdx][0],
                                     rainbowRGBValues[rainbowRowIdx][1],
                                     rainbowRGBValues[rainbowRowIdx][2] );
        WS2812b_SetSinglePixelColor( &ledArray,
                                     row5[thisIndex - 2],
                                     rainbowRGBValues[rainbowRowIdx][0],
                                     rainbowRGBValues[rainbowRowIdx][1],
                                     rainbowRGBValues[rainbowRowIdx][2] );
    }

    /* Limits thisIndex between 0-14. Every reset of thisIndex triggers a rainbowIdx increment. rainbowIdx
     * is limited from 0-6.
     */
    if( MAX_LED_INDEX_RAINBOW == thisIndex )
    {
        thisIndex = 0;
        rainbowRowIdx = ( RAINBOW_INDEX_MAX == rainbowRowIdx ) ? 0 : rainbowRowIdx + 1;
    }
    else
    {
        thisIndex++;
    }
    WS2812B_Render( &ledArray );
    return;

}

void Clock_CrossingRainbowPatternwithDelays( )
{

#define MAX_LED_INDEX_RAINBOW 13
#define RAINBOW_INDEX_MAX 5
    static size_t thisIndex = 0;
    static size_t rainbowRowIdx = 0;


    /* thisIndex values of 0 only iterate middle row. */
    if( 0 == thisIndex )
    {
        WS2812b_SetSinglePixelColor( &ledArray,
                                     row3[thisIndex],
                                     rainbowRGBValues[rainbowRowIdx][0],
                                     rainbowRGBValues[rainbowRowIdx][1],
                                     rainbowRGBValues[rainbowRowIdx][2] );
    }

        /* thisIndex values of 1 iterate middle three rows. */
    else if( 1 == thisIndex )
    {
        WS2812b_SetSinglePixelColor( &ledArray,
                                     row2[thisIndex - 1],
                                     rainbowRGBValues[rainbowRowIdx][0],
                                     rainbowRGBValues[rainbowRowIdx][1],
                                     rainbowRGBValues[rainbowRowIdx][2] );
        WS2812b_SetSinglePixelColor( &ledArray,
                                     row3[thisIndex],
                                     rainbowRGBValues[rainbowRowIdx][0],
                                     rainbowRGBValues[rainbowRowIdx][1],
                                     rainbowRGBValues[rainbowRowIdx][2] );
        WS2812b_SetSinglePixelColor( &ledArray,
                                     row4[thisIndex - 1],
                                     rainbowRGBValues[rainbowRowIdx][0],
                                     rainbowRGBValues[rainbowRowIdx][1],
                                     rainbowRGBValues[rainbowRowIdx][2] );
    }

        /* thisIndex values of 2 and greater iterate all rows. */
    else
    {
        WS2812b_SetSinglePixelColor( &ledArray,
                                     row1[thisIndex - 2],
                                     rainbowRGBValues[rainbowRowIdx][0],
                                     rainbowRGBValues[rainbowRowIdx][1],
                                     rainbowRGBValues[rainbowRowIdx][2] );
        WS2812b_SetSinglePixelColor( &ledArray,
                                     row2[thisIndex - 1],
                                     rainbowRGBValues[rainbowRowIdx][0],
                                     rainbowRGBValues[rainbowRowIdx][1],
                                     rainbowRGBValues[rainbowRowIdx][2] );
        WS2812b_SetSinglePixelColor( &ledArray,
                                     row3[thisIndex],
                                     rainbowRGBValues[rainbowRowIdx][0],
                                     rainbowRGBValues[rainbowRowIdx][1],
                                     rainbowRGBValues[rainbowRowIdx][2] );
        WS2812b_SetSinglePixelColor( &ledArray,
                                     row4[thisIndex - 1],
                                     rainbowRGBValues[rainbowRowIdx][0],
                                     rainbowRGBValues[rainbowRowIdx][1],
                                     rainbowRGBValues[rainbowRowIdx][2] );
        WS2812b_SetSinglePixelColor( &ledArray,
                                     row5[thisIndex - 2],
                                     rainbowRGBValues[rainbowRowIdx][0],
                                     rainbowRGBValues[rainbowRowIdx][1],
                                     rainbowRGBValues[rainbowRowIdx][2] );
    }

    /* Limits thisIndex between 0-14. Every reset of thisIndex triggers a rainbowIdx increment. rainbowIdx
     * is limited from 0-6.
     */
    if( MAX_LED_INDEX_RAINBOW == thisIndex )
    {
        thisIndex = 0;
        rainbowRowIdx = ( RAINBOW_INDEX_MAX == rainbowRowIdx ) ? 0 : rainbowRowIdx + 1;
    }
    else
    {
        thisIndex++;
    }
    WS2812B_Render( &ledArray );
    __delay_ms( 75 );
    return;
}

void Clock_Popcorn_Pattern( void )
{
    WS2812b_SetStripConstantColor( &ledArray,
                                   0u,
                                   0u,
                                   0u );
    WS2812b_SetSinglePixelColor( &ledArray,
                                 rand( ) % NUM_CLOCK_PIXELS - 1,
                                 rand( ) % 255,
                                 rand( ) % 255,
                                 rand( ) % 255 );
    WS2812b_SetSinglePixelColor( &ledArray,
                                 rand( ) % NUM_CLOCK_PIXELS - 1,
                                 rand( ) % 255,
                                 rand( ) % 255,
                                 rand( ) % 255 );
    WS2812b_SetSinglePixelColor( &ledArray,
                                 rand( ) % NUM_CLOCK_PIXELS - 1,
                                 rand( ) % 255,
                                 rand( ) % 255,
                                 rand( ) % 255 );
    WS2812b_SetSinglePixelColor( &ledArray,
                                 rand( ) % NUM_CLOCK_PIXELS - 1,
                                 rand( ) % 255,
                                 rand( ) % 255,
                                 rand( ) % 255 );
    WS2812b_SetSinglePixelColor( &ledArray,
                                 rand( ) % NUM_CLOCK_PIXELS - 1,
                                 rand( ) % 255,
                                 rand( ) % 255,
                                 rand( ) % 255 );
    WS2812b_SetSinglePixelColor( &ledArray,
                                 rand( ) % NUM_CLOCK_PIXELS - 1,
                                 rand( ) % 255,
                                 rand( ) % 255,
                                 rand( ) % 255 );
    WS2812b_SetSinglePixelColor( &ledArray,
                                 rand( ) % NUM_CLOCK_PIXELS - 1,
                                 rand( ) % 255,
                                 rand( ) % 255,
                                 rand( ) % 255 );
    WS2812b_SetSinglePixelColor( &ledArray,
                                 rand( ) % NUM_CLOCK_PIXELS - 1,
                                 rand( ) % 255,
                                 rand( ) % 255,
                                 rand( ) % 255 );
    WS2812b_SetSinglePixelColor( &ledArray,
                                 rand( ) % NUM_CLOCK_PIXELS - 1,
                                 rand( ) % 255,
                                 rand( ) % 255,
                                 rand( ) % 255 );
    WS2812b_SetSinglePixelColor( &ledArray,
                                 rand( ) % NUM_CLOCK_PIXELS - 1,
                                 rand( ) % 255,
                                 rand( ) % 255,
                                 rand( ) % 255 );
    WS2812B_Render( &ledArray );
    __delay_ms( 150 );
}

void Clock_Popcorn_Pattern_Hold( void )
{
    // TODO make half hold during duration so there is no instant screen clear 
#define NUM_RENDERS_BEFORE_RESET 8
    static size_t numRenders = 0;

    WS2812b_SetSinglePixelColor( &ledArray,
                                 rand( ) % NUM_CLOCK_PIXELS - 1,
                                 rand( ) % 255,
                                 rand( ) % 255,
                                 rand( ) % 255 );
    WS2812b_SetSinglePixelColor( &ledArray,
                                 rand( ) % NUM_CLOCK_PIXELS - 1,
                                 rand( ) % 255,
                                 rand( ) % 255,
                                 rand( ) % 255 );
    WS2812b_SetSinglePixelColor( &ledArray,
                                 rand( ) % NUM_CLOCK_PIXELS - 1,
                                 rand( ) % 255,
                                 rand( ) % 255,
                                 rand( ) % 255 );
    WS2812b_SetSinglePixelColor( &ledArray,
                                 rand( ) % NUM_CLOCK_PIXELS - 1,
                                 rand( ) % 255,
                                 rand( ) % 255,
                                 rand( ) % 255 );
    WS2812b_SetSinglePixelColor( &ledArray,
                                 rand( ) % NUM_CLOCK_PIXELS - 1,
                                 rand( ) % 255,
                                 rand( ) % 255,
                                 rand( ) % 255 );
    WS2812b_SetSinglePixelColor( &ledArray,
                                 rand( ) % NUM_CLOCK_PIXELS - 1,
                                 rand( ) % 255,
                                 rand( ) % 255,
                                 rand( ) % 255 );
    WS2812b_SetSinglePixelColor( &ledArray,
                                 rand( ) % NUM_CLOCK_PIXELS - 1,
                                 rand( ) % 255,
                                 rand( ) % 255,
                                 rand( ) % 255 );
    WS2812b_SetSinglePixelColor( &ledArray,
                                 rand( ) % NUM_CLOCK_PIXELS - 1,
                                 rand( ) % 255,
                                 rand( ) % 255,
                                 rand( ) % 255 );
    WS2812b_SetSinglePixelColor( &ledArray,
                                 rand( ) % NUM_CLOCK_PIXELS - 1,
                                 rand( ) % 255,
                                 rand( ) % 255,
                                 rand( ) % 255 );
    WS2812B_Render( &ledArray );
    __delay_ms( 150 );
    numRenders++;
    if( NUM_RENDERS_BEFORE_RESET == numRenders )
    {
        WS2812b_SetStripConstantColor( &ledArray,
                                       0u,
                                       0u,
                                       0u );
        numRenders = 0;
    }
    return;
}

void Clock_GradientFlashingColors( void )
{
    /* colorIndex controls the mode. 
     * baseColor controls the color gradient. 
     *
     * When base color reaches its max, isDirectionForward switches to false and 
     *      subsequent entries causes baseColor to decrement. When baseColor reaches
     *      0, isDirectionForward switches to true and baseColor will increment. 
     * 
     */

    static size_t colorIndex = 0;
    static uint8_t baseColor = 0; // Single color used for all iterations 
    static bool isDirectionForward = true;

    switch( colorIndex )
    {
        case ( 0 ):
        {
            /* Magenta*/
            WS2812b_SetStripConstantColor( &ledArray,
                                           baseColor,
                                           0u,
                                           baseColor );
            break;
        }
        case ( 1 ):
        {
            /* Lime Green */
            WS2812b_SetStripConstantColor( &ledArray,
                                           baseColor / 4u,
                                           baseColor,
                                           baseColor / 4u );
            break;
        }
        case ( 2 ):
        {
            /* Orange */
            WS2812b_SetStripConstantColor( &ledArray,
                                           baseColor,
                                           baseColor / 6u,
                                           0u );
            break;
        }
        default:
        {
            break;
        }
    }
    WS2812B_Render( &ledArray );
    __delay_ms( 3 );

    if( ( UINT8_MAX == baseColor ) &&
        ( isDirectionForward ) )
    {
        isDirectionForward = false;
    }
    else if( ( 0u == baseColor ) &&
             ( false == isDirectionForward ) )
    {
        isDirectionForward = true;

        colorIndex = ( 2 == colorIndex ) ? 0 : colorIndex + 1;
    }
    baseColor = ( isDirectionForward ) ? baseColor + 1u : baseColor - 1u;
}

void Clock_SnakeTravelPattern( void )
{

}

void Clock_SinWaveMovementPattern( void )
{

}

/* End ledArray.c source file */
