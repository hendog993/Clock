/* Filename: ws2812b.h 
 *
 * Date: 9 September 2022
 * 
 * Author: Henry Gilbert 
 * 
 * Description: Public interface to ws2812b hardware abstraction layer. Includes
 *      primary pixel type definition, along with driver specific routines. 
 * 
 * "Bit bang" driver for ws2812b led strip. Manually controls a data pin
 *      and adds NOP instructions in for the required timing delays (listed below).
 *      Module is divided into two sections. 1. Hardware section, and 2. Pixel RGB modifiation.
 *
 * Timing requirements: 1 code = HIGH 800 ns , LOW 450 ns (All toleranced at += 150 ns)
 *                      0 code = HIGH 400 ns , LOW 850 ns (All toleranced at += 150 ns)
 * Hardware section
 *      - WS2812B_Render
 *      - WS2812B_Reset
 *      - WS2812b_RenderSinglePixel
 *
 * Pixel Modification
 *      - WS2812b_SetPixelBlock
 *      - WS2812b_SetSinglePixelColor
 *      - WS2812b_ClearPixels
 *
 * API Requirements:
 *      1. Instruction frequency must operate at no less than 8 MHz (32 MHz clock speed). This
 *         driver is written using 8MHz (125 ns) instruction speed. The macros for WRITE_ONE and
 *         WRITE_ZERO must be adjusted for anything outside of the specified tolerance.
 *      2. To avoid modifying this file, the data control pin should be controlled with macros
 *         named DATA_PIN_SetHigh() and DATA_PIN_SetLow(). These can be automatically generated
 *         using the MCC code generator, or they can be manually programmed. Either or.
 *      3. Pixel indices must start from 0!! Importrant note. 

 * Future design modifications: Can control the data pin using a timer interrupt at exactly the
 *      specified period desired. This bitbang method relies on the very liberal tolerance of
 *      400ns += 150 ns of the instruction frequency. Faster clock speeds would alter the driver,
 *      but a more generic driver could be implemented using timers or serial transfer peripherals.
 * 
 *      Another future addition could be to pass in the data control pin functions as generic, such
 *      that modification of the file is not necessary, and generic instances of ledArrays can be 
 *      created based on an GPIO input data pin parameter.
 * 
 *      TODO update return parameters to bool so write faults can be diagnosed. 
 *      
 *      TODO can use via output compare? 
 * 
 * 
 * 
 */

#ifndef WS2812B_H
#define WS2812B_H


/********************* Included File(s) ********************/
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>


/******************* Macro Definition(s) *******************/
#define WS2812B_RED_INDEX 0u
#define WS2812B_GREEN_INDEX 1u
#define WS2812B_BLUE_INDEX 2u
#define NUM_BYTES_IN_PIXEL 3u

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 32000000u
#endif 


/****************** Type Definition(s) *********************/

/* This order red->green->blue MUST be maintained for proper functionality. 
 * Also note that this struct doesn't have to be __packed__, as the xc8 
 * compiler is byte addressable in memory. However, for compiler cross compatability, 
 * this issue must be addressed. 
 */
typedef struct
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} ws2812bPixel;

typedef struct
{
    ws2812bPixel * pixelBuffer;
    size_t numPixels;
} ws2812bArray;

/********************* Function Prototype(s) ****************/


/*
 * Function: 
 *      WS2812b_Initialize
 *
 * Description: 
 *      Links a pixel buffer pointer and number of elements into a single ws2812bArray structure. In the future,
 *      this allows multiple LED strips to be defined in a single program. This can be implemented when a generic
 *      function parameter function pass to control the data pin is written (GPIO harmony module). 
 *
 * Return: 
 *      ws2812bArray structure with a pointer to the input pixel buffer and the number of elements.
 *
 */
ws2812bArray WS2812b_Initialize(ws2812bPixel * pixelBuffer,
        const size_t numElements,
        bool * const wasSetupSuccessful);


/* Function: 
 *      WS2812B_Render
 *
 * Description: 
 *      Iteratively renders the contents of the strip's pixel buffer to the LEDs.
 *      This function takes 26 ms to render 64 pixels. This can be tremendously improved.. 
 */
void WS2812B_Render(const ws2812bArray * const strip);


/* Function: 
 *      WS2812b_SetSinglePixelColor
 *
 * Description: 
 *      Sets a single specified pixel's RGB value to the input red, green, and blue values.
 */
void WS2812b_SetSinglePixelColor(ws2812bArray * const strip,
        const size_t pixelIndex,
        const uint8_t red,
        const uint8_t green,
        const uint8_t blue);


/* Function: 
 *      WS2812b_SetPixelBlockFromRGBArray
 *
 * Description: 
 *      Sets a block of pixels RGB values based on the input RGB byte array.
 *
 * Parameter checks:
 *      1. NULL on all pointers.
 *      2. If the pixel offset + the number of pixels to set is greater than total number of pixels, errors could occur.
 *      3. The pixel start offset can't be larger than the number of pixels.
 *
 */
void WS2812b_SetPixelBlockFromRGBArray(const ws2812bArray * const strip,
        const size_t pixelStartOffset,
        const size_t numPixelsToSet,
        const uint8_t * const rgbArray);


/*
 * Function: 
 *      void WS2812b_SetPixelBlockConstantColor()
 *
 * Description: 
 *      Sets a pixel block to a constant color input via RGB bytes.
 */
void WS2812b_SetPixelBlockConstantColor(const ws2812bArray * const strip,
        const size_t pixelStartOffset,
        const size_t numPixelsToSet,
        const uint8_t red,
        const uint8_t green,
        const uint8_t blue);


/*
 * Function:
 *      WS2812b_SetStripConstantColor()
 *
 * Description: 
 *      Sets all pixels to RGB value of input byte values. Can call this function to clear LEDs or write all high.
 *
 */
void WS2812b_SetStripConstantColor(ws2812bArray * const strip,
        const uint8_t red,
        const uint8_t green,
        const uint8_t blue);

/*
 * Function: 
 *      void WS2812b_CopyPixelBufferArrayFromSource
 *
 * Description:
 *      Copies the contents of a source pixel buffer into the source pixel buffer of 
 *      an LED strip. Starts using the byte address, not the ws2812bPixel address, as 
 *      memcpy works in byte address modes. 
 * 
 * Return: 
 */
void WS2812b_CopyPixelBufferArrayFromSource(const ws2812bArray * const strip,
        ws2812bPixel * const pixelSource,
        const size_t startAddress,
        const size_t numPixelsToCopy);


#endif