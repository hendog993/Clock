/* Filename: patternMode.c
 *
 * Author: Henry Gilbert
 *
 * Date: 7 October 2022
 *
 * Description: Application layer for pattern mode
 *
 */


/*********************** Included File(s) ********************************/
#include "patternMode.h"
#include "clockLEDs.h"
#include "rotaryEncoder.h"


/*********************** Macro Definition (s) ****************************/
#define NUM_PATTERNS 6u
#define limit(val, low, high) ( (val < low) ? high : (val > high) ? low : val)


/*********************** Local Variables (s) ****************************/
static void (*patterns[NUM_PATTERNS] )(void);

/************************** Functions ************************************/
void STATE_PatternMode( void )
{
    /* Gets rotary encoder counts and adjusts pattern accordingly */
    static int32_t patternIndex = 0;
    int32_t rotCounts = RotaryEncoder_GetShaftCounts( );

    if( rotCounts )
    {
        patternIndex += rotCounts;
        patternIndex = limit( patternIndex, 0, NUM_PATTERNS - 1 );

        /* Reset the screen buffer to all blank after every pattern change */
        Clock_SetScreenBlankAndRender( );
    }

    ( *patterns[patternIndex] )( );
    return;
}

void State_SetFunctionLookupTable( void )
{
    patterns[0] = Clock_IterateSinglePixelByIndex;
    patterns[1] = Clock_CrossingRainbowPattern;
    patterns[2] = Clock_CrossingRainbowPatternwithDelays;
    patterns[3] = Clock_Popcorn_Pattern;
    patterns[4] = Clock_Popcorn_Pattern_Hold;
    patterns[5] = Clock_GradientFlashingColors;
}
