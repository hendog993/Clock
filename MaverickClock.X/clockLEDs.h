/* Filename: ledArray.h
 *
 * Date: 12 September 2022
 *
 * Author: Henry Gilbert
 *
 * Description: Public interface to clock LED configuration.
 *
 */

#ifndef CLOCK_LEDS_H
#define CLOCK_LEDS_H

/*********************** Included File(s) ********************************/
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*********************** Type Definition(s) ******************************/
typedef struct
{
    uint8_t digit1; // Tens digit of hour
    uint8_t digit2; // Ones digit of hour
    uint8_t digit3; // Tens digit of minutes
    uint8_t digit4; // Ones digit of minutes
} TimeInDigits;


/*********************** Macro Definition (s) ****************************/
#define NUM_CLOCK_PIXELS 64



/*
 * Function:
 *      bool Clock_InitializeClockLEDs
 *
 * Description:
 *      Sets the local background rgb array, digit rgb array, and led strip struct.
 *
 *
 * Return: True if setup was successful
 */
bool Clock_InitializeClockLEDs(const size_t numElements,
        const uint8_t digitRed,
        const uint8_t digitGreen,
        const uint8_t digitBlue,
        const uint8_t backgroundRed,
        const uint8_t backgroundGreen,
        const uint8_t backgroundBlue);

/* Function: Clock_SetBackgroundRGBArray
 *
 * Description:
 *      Sets local background RGB array. Allows the user to reconfigure colors at runtime.
 *
 */
void Clock_SetBackgroundRGBArray(const uint8_t red,
        const uint8_t green,
        const uint8_t blue);

/* Function: Clock_SetDigitRGBArray
 *
 * Description:
 *      Sets local digit RGB array. Allows the user to reconfigure colors at runtime.
 */
void Clock_SetDigitRGBArray(const uint8_t red,
        const uint8_t green,
        const uint8_t blue);



/* Function:
 *      Clock_WriteTimeDigitValuesAndRender
 *
 * Description:
 *      Function called by primary renderer method during time mode. Accepts a digit structure
 *      and writes to the pixel buffer accordingly. Calls the render function to display the
 *      minutes value on the screen.
 *
 * Precondition:
 *      Static pixel buffers must have been prerendered, otherwise the screen will only contain
 *      background colors.
 */
void Clock_WriteTimeDigitValuesAndRenderScreen(const TimeInDigits * const digits);


/*
 * Function:
 *      void Clock_PrerenderPixelAndBackgroundValues()
 *
 * Description:
 *      Start by prerendering the background color RGB values to the render buffer entirely.
 *      Iterates through each digit encoding and prerenders the digit values to the statically
 *      allocated digit buffers. This allows the user to simply point to the desired digit at
 *      runtime and copy the values over instead of doing the entire calculation every single
 *      cycle. This function is called once at startup and again for every time the user
 *      changes the desired display colors.
 *
 */
void Clock_PrerenderPixelAndBackgroundValues(void);

/* Function:
 *      Clock_DoubleFlashWithClearScreen
 * 
 * Description:
 *      Double flashes the screen white and black to clearly show entry into color change mode. 
 * 
 */
void Clock_DoubleFlashWithClearScreen(void );



/* Function:
 *      Clock_ForceRender
 *
 * Description:
 *      Forces a render based on input TimeInDigits struct. Used primarily in color change mode to force colors to display.
 *
 *
 */
void Clock_ForceTimeRender(const TimeInDigits * const t);


/* Function:
 *      Clock_SetScreenBlankAndRender
 * 
 * Description:
 *      Clears screen and renders to reset all LEDs to blank. 
 */
void Clock_SetScreenBlankAndRender(void);


/************************************ Pattern mode functions ***********************/

/* Function:
 *      Clock_IterateSinglePixelByIndex
 *
 * Description:
 *      Pattern to iterate all pixels based on index (0-63);
 *
 */
void Clock_IterateSinglePixelByIndex(void);


/* Clock_CrossingRainbowPattern
 *
 *      Creates a horizontal, left to right crossing pattern. Changes colors after every final crossing.
 *
 */
void Clock_CrossingRainbowPattern(void);

/* Clock_CrossingRainbowPatternwithDelays
 *
 *      Creates rainbow pattern with small time delay
 */
void Clock_CrossingRainbowPatternwithDelays(void);


/* Clock_Popcorn_Pattern
 *
 * Generates 8 random pixel values at 8 random locations.  
 */
void Clock_Popcorn_Pattern(void);


/* Clock_Popcorn_Pattern_Hold
 *
 * Generates 8 random pixel values at 8 random locations. Holds for 8 patterns  
 */
void Clock_Popcorn_Pattern_Hold(void);


/* Clock_GradientFlashingColors
 *
 * Iterates through selected colors and displays gradient patterns 
 */
void Clock_GradientFlashingColors( void);

/* Clock_SnakeTravelPattern
 *
 * Creates a snake traversing through the screen 
 */
void Clock_SnakeTravelPattern(void);


/* Clock_SinWaveMovementPattern
 *
 * Creates sin wave pattern with static colors 
 */
void Clock_SinWaveMovementPattern( void );












#endif

/* End FILENAME header file */
