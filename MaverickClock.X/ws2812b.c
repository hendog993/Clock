/* Filename: ws2812b.c
 *
 * Author: Henry Gilbert
 *
 * Date: 8 September 2022
 *
 * Description: Implementation of ws2812b led protocol. 
 *
 */


/****************** Included File(s) **********************/
#include "ws2812b.h"
#include <xc.h>
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/interrupt_manager.h"
#include <string.h>



/****************** Macro Definition(s) *******************/
#define WRITE_ONE()      DATA_PIN_SetHigh(); NOP(); NOP(); NOP(); NOP(); DATA_PIN_SetLow(); NOP(); NOP();
#define WRITE_ZERO()     DATA_PIN_SetHigh(); NOP(); DATA_PIN_SetLow(); NOP(); NOP(); NOP(); NOP(); NOP();

#define NUM_BITS_IN_RGB_PIXEL 24u
#define GREEN_LEFT_SHIFT_OFFSET 16u
#define RED_LEFT_SHIFT_OFFSET 8u
#define NUM_BITS_IN_BYTE 8u



/****************** Static Function Prototype(s) **********/
static void WS2812b_RenderSinglePixel( const ws2812bPixel * const pixel );
static inline void WS2812B_Reset( );

/*********************** Function(s) **********************/

ws2812bArray WS2812b_Initialize( ws2812bPixel * pxBuff,
                                 const size_t numElements,
                                 bool * const wasSetupSuccessful )
{
    ws2812bArray array;
    array.pixelBuffer = pxBuff;
    array.numPixels = numElements;
    *wasSetupSuccessful = ( ( NULL == pxBuff ) || ( 0 == numElements ) ) ? false : true;
    return array;
}

/****************************************************************************************************/
/*********************                       Hardware API Section  **********************************/

/****************************************************************************************************/



void WS2812B_Render( const ws2812bArray * const strip )
{
    if( ( NULL == strip ) ||
        ( NULL == strip->pixelBuffer ) )
    {
        return;
    }

    /* Disable interrupts to avoid incomplete renders */
    INTERRUPT_GlobalInterruptDisable( );
    INTERRUPT_PeripheralInterruptDisable( );

    size_t i;
    ws2812bPixel * thisPixel = strip->pixelBuffer;
    for( i = 0; i < strip->numPixels; i++ )
    {
        size_t j;
        for( j = 0; j < NUM_BITS_IN_BYTE; j++ )
        {
            if( ( thisPixel->green << j ) & 0x80u )
            {
                WRITE_ONE( );
            }
            else
            {
                WRITE_ZERO( );
            }
        }
        for( j = 0; j < NUM_BITS_IN_BYTE; j++ )
        {
            if( ( thisPixel->red << j ) & 0x80u )
            {
                WRITE_ONE( );
            }
            else
            {
                WRITE_ZERO( );
            }
        }
        for( j = 0; j < NUM_BITS_IN_BYTE; j++ )
        {
            if( ( thisPixel->blue << j ) & 0x80u )
            {
                WRITE_ONE( );
            }
            else
            {
                WRITE_ZERO( );
            }
        }

        thisPixel++;

    }

    /* Reset back to 0th index */
    WRITE_ONE( );
    WRITE_ZERO( );
    __delay_us( 50 );
    WRITE_ONE( );

    INTERRUPT_GlobalInterruptEnable( );
    INTERRUPT_PeripheralInterruptEnable( );
    return;
}


/****************************************************************************************************/
/************                       RGB Pixel Modification Section  *********************************/

/****************************************************************************************************/


void WS2812b_SetSinglePixelColor( ws2812bArray * const strip,
                                  const size_t pixelIndex,
                                  const uint8_t red,
                                  const uint8_t green,
                                  const uint8_t blue )
{
    if( ( NULL == strip ) ||
        ( NULL == strip->pixelBuffer ) ||
        ( pixelIndex > strip->numPixels ) )
    {
        return;
    }

    ws2812bPixel * thisPixel = &( strip->pixelBuffer[pixelIndex] );

    if( NULL == thisPixel )
    {
        return;
    }

    thisPixel->red = red;
    thisPixel->green = green;
    thisPixel->blue = blue;
    return;
}

void WS2812b_SetPixelBlockFromRGBArray( const ws2812bArray * const strip,
                                        const size_t pixelStartOffset,
                                        const size_t numPixelsToSet,
                                        const uint8_t * const rgbArray )
{
    if( ( NULL == strip ) ||
        ( NULL == strip->pixelBuffer ) ||
        ( NULL == rgbArray ) )
    {
        return;
    }

    size_t finalPixelIndex = pixelStartOffset + numPixelsToSet;

    if( finalPixelIndex > strip->numPixels ) // this sould be >=, since writing a block at the last pixel wouldn't make sense..
    {
        return;
    }

    uint8_t * startPixelAddress = &( strip->pixelBuffer[pixelStartOffset].red );
    memcpy( startPixelAddress, rgbArray, numPixelsToSet );
    return;
}

void WS2812b_SetPixelBlockConstantColor( const ws2812bArray * const strip,
                                         const size_t pixelStartOffset,
                                         const size_t numPixelsToSet,
                                         const uint8_t red,
                                         const uint8_t green,
                                         const uint8_t blue )
{
    if( ( NULL == strip ) ||
        ( NULL == strip->pixelBuffer ) )
    {
        return;
    }

    size_t finalPixelIndex = pixelStartOffset + numPixelsToSet;

    if( finalPixelIndex > strip->numPixels )
    {
        return;
    }

    size_t i;
    uint8_t rgbArray[NUM_BYTES_IN_PIXEL] = {red, green, blue};
    uint8_t * startPixelAddress = &( strip->pixelBuffer[pixelStartOffset].red );
    for( i = 0; i < numPixelsToSet; i++ )
    {
        memcpy( ( startPixelAddress + NUM_BYTES_IN_PIXEL * i ), rgbArray, NUM_BYTES_IN_PIXEL );
    }
    return;
}

void WS2812b_SetStripConstantColor( ws2812bArray * const strip,
                                    const uint8_t red,
                                    const uint8_t green,
                                    const uint8_t blue )
{
    if( ( NULL == strip ) ||
        ( NULL == strip->pixelBuffer ) )
    {
        return;
    }
    WS2812b_SetPixelBlockConstantColor( strip,
                                        0,
                                        strip->numPixels,
                                        red,
                                        green,
                                        blue );
    return;
}

void WS2812b_CopyPixelBufferArrayFromSource( const ws2812bArray * const strip,
                                             ws2812bPixel * const pixelSource,
                                             const size_t startAddress,
                                             const size_t numPixelsToCopy )
{
    if( ( NULL == strip ) ||
        ( NULL == pixelSource ) ||
        ( startAddress + numPixelsToCopy > strip->numPixels ) )
    {
        return;
    }

    ws2812bPixel * startPixel = &( strip->pixelBuffer[startAddress] );

    /* Starting element of the ws2812bPixel structure is the red element */
    uint8_t * dest = &( startPixel->red );
    uint8_t * src = &( pixelSource->red );

    memcpy( dest, src, numPixelsToCopy * NUM_BYTES_IN_PIXEL );
    return;
}


// end
