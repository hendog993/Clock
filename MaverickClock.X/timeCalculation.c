/* Filename: timeCalculation.c
 *  
 * Date: 12 Septempber 2022
 *
 * Author: Henry Gilbert
 *
 * Description: Link between timer module, time calculation, rotary encoder,
 *      and pixel buffer writing. Has local static variables that hold the current
 *      time in counts. Function has a polled function that allows the mainloop
 *      to detect a time change (in minutes) and render accordingly. Only
 *      responsible for calculating time, and should only call a single function
 *      from the pixel buffer writer library.
 *
 */

/*********************** Included File(s) ********************************/
#include "timeCalculation.h"
#include "mcc_generated_files/tmr5.h"
#include "rotaryEncoder.h"
#include "clockLEDs.h"
#include <stdlib.h>
#include <xc.h>

/*********************** Local Variable(s) *******************************/
static volatile uint32_t currentTimeInSeconds = 0u; // Shared data between polled routines and ISR incrementer. 



/************************ Function Macro Definition(s) *******************/
// limit: IF value is less than low, make value high. Else if value is greater than high, make low. Else, nothing.
#define limit(val, low, high) ( (val < low) ? high : (val > high) ? low : val)


/*********************** Macro Definition (s) ****************************/
#define MAX_NUM_SECONDS 43199u
#define SECONDS_OFFSET 3600u
#define NUM_SECONDS_IN_MINUTE 60u
#define NUM_MINUTES_IN_HOURS 60u
#define NUM_COUNTS_PER_SECOND 60 // corresponds to seconds, just added another macro here for readability


/*********************** Function Prototype(s) ***************************/
static void Time_IncrementSecondsISR( void );

/************************** Functions ************************************/


void Time_InitializeTimeModule( void )
{
    TMR5_SetInterruptHandler( Time_IncrementSecondsISR );
    return;
}

void Time_EnableTimeModule( void )
{
    TMR5_StartTimer( );
    return;
}

void Time_DisableTimeModule( void )
{
    TMR5_StopTimer( );
    return;
}

void Time_RenderIfMinutesHaveChanged( bool isSwitchPressHrMnMode )
{
    /* Save the minutes at last change value. This is only updated when a time change in minutes has been detected */
    static uint16_t minutesAtLastChange = 0xFFFF; // Make this 0xFF at first so that at startup, it's forced to change. 

    /* Get rotary encoder counts and change time accordingly */
    int32_t minutesDelta = RotaryEncoder_GetShaftCounts( );

    if( isSwitchPressHrMnMode )
    {
        minutesDelta *= 60u;
    }

    int32_t thisTimeInSeconds = (int32_t) currentTimeInSeconds + ( minutesDelta * NUM_COUNTS_PER_SECOND );
    thisTimeInSeconds = limit( thisTimeInSeconds, 0, MAX_NUM_SECONDS );
    currentTimeInSeconds = ( volatile uint32_t ) thisTimeInSeconds; // Reset the volatile timer variable back
    uint16_t currentTimeInMinutes = (uint16_t) ( currentTimeInSeconds / NUM_SECONDS_IN_MINUTE );

    /* If a minutes change has occurred, update the minutes at last change and render the current minutes*/
    if( currentTimeInMinutes != minutesAtLastChange )
    {
        minutesAtLastChange = currentTimeInMinutes;
        uint8_t hours = (uint8_t) ( currentTimeInMinutes / NUM_MINUTES_IN_HOURS ) + 1u;
        uint8_t minutes = (uint8_t) ( currentTimeInMinutes % NUM_MINUTES_IN_HOURS );

        TimeInDigits t = {
            .digit1 = ( hours % 100 ) / 10,
            .digit2 = ( hours % 10 ),
            .digit3 = ( minutes % 100 ) / 10,
            .digit4 = ( minutes % 10 )
        };

        Clock_WriteTimeDigitValuesAndRenderScreen( &t );
    }
    return;
}

uint32_t Time_GetCurrentTimeInSeconds( void)
{
    return currentTimeInSeconds;
}

void Time_RenderInputTime( uint32_t timeInSeconds,
                           bool setCurrentTime )
{
    uint16_t currentTimeInMinutes = (uint16_t) ( timeInSeconds / NUM_SECONDS_IN_MINUTE );

    /* If a minutes change has occurred, update the minutes at last change and render the current minutes*/

    uint8_t hours = (uint8_t) ( currentTimeInMinutes / NUM_MINUTES_IN_HOURS ) + 1u;
    uint8_t minutes = (uint8_t) ( currentTimeInMinutes % NUM_MINUTES_IN_HOURS );

    TimeInDigits t = {
        .digit1 = ( hours % 100 ) / 10,
        .digit2 = ( hours % 10 ),
        .digit3 = ( minutes % 100 ) / 10,
        .digit4 = ( minutes % 10 )
    };

    if( setCurrentTime )
    {
        currentTimeInSeconds = timeInSeconds;
    }

    Clock_WriteTimeDigitValuesAndRenderScreen( &t );
    return;
}

/* Function:
 *      void Time_IncrementSecondsISR(void)
 *
 * Description:
 *      ISR to increment the seconds value. Handles rollover. Called once every second. 
 */
static void Time_IncrementSecondsISR( void )
{
    currentTimeInSeconds = ( currentTimeInSeconds + 1 > MAX_NUM_SECONDS ) ? 0 : currentTimeInSeconds + 1;
    return;
}


/* End timeCalculation.c source file */
