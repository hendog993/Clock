/* Filename: app.c
 *
 * Date: 8 September 2022
 *
 * Author: Henry Gilbert
 *
 * Description: Project application layer. 
 *
 */


/*********************** Included File(s) ********************/
#include "mcc_generated_files/mcc.h"
#include "app.h"
#include "rotaryEncoder.h"
#include "patternMode.h"
#include "changeColorMode.h"
#include "timeCalculation.h"
#include "clockLEDs.h"
#include "CRC16bit.h"
#include "mcc_generated_files/memory.h"

/********************* Function Prototype(s) *******************/
static void Timer1Interrupt( void );

/**************************** Local Variables ******************/
APP app = {
    .state = DISPLAY_TIME_MODE
};

static RotaryEncoder rot = {
    .rotBtn.config.multiClickTimerThreshold_cts = 0x54D, // 350 ms equivalent in counts
    .rotBtn.config.longPressTimerThreshold_cts = 0x1E48, // 2s equivalent in counts
    .rotBtn.config.multiClickTimeFreshnessLimit_cts = 0xF24, // 1s equivalent in counts 
};

void APP_Initialize( void )
{
    /* Read NVM Memory and verify CRC */
    uint8_t eepromReadbackData[NUM_BYTES_EEPROM_BLOCK];
    bool wasReadbackValid;
    size_t maxRetries = 3;
    while( maxRetries )
    {
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

    if( !wasReadbackValid )
    {
        // TODO error flag 

    }

    /* Initialize clock LED strip instance */
    Clock_InitializeClockLEDs( NUM_CLOCK_PIXELS,
                               eepromReadbackData[0],
                               eepromReadbackData[1],
                               eepromReadbackData[2],
                               eepromReadbackData[3],
                               eepromReadbackData[4],
                               eepromReadbackData[5] );

    /* Prerender all digit values to file static pixel buffers */
    Clock_PrerenderPixelAndBackgroundValues( );

    /* Enable time calculation module */
    Time_InitializeTimeModule( );
    Time_EnableTimeModule( );

    /* Initialize and enable  rotary encoder */
    RotaryEncoder_Init( &rot );
    RotaryEncoder_Enable( );

    /* Setup pattern mode function table */
    State_SetFunctionLookupTable( );

    /* Fault out LED timer */
    TMR1_SetInterruptHandler( Timer1Interrupt );
    TMR1_StartTimer( );

    return;
}

void APP_TASKS( void )
{
    static bool wasSwitchPressedInTimeMode = false; // Denotes time mode minutes to hours change. 

    /* Update state machine if encoder button is pressed */
    if( RotaryEncoder_HasButtonPressOccurred( ) )
    {
        switch( RotaryEncoder_GetSwitchState( ) )
        {
            case SWITCH_DOUBLE_CLICK:
                if( PATTERN_MODE == app.state )
                {
                    Clock_PrerenderPixelAndBackgroundValues( );
                    app.state = DISPLAY_TIME_MODE;
                }
                else if( DISPLAY_TIME_MODE == app.state )
                {
                    app.state = PATTERN_MODE;
                }
                else
                {
                    // Do nothing. Switch is resting or single pressed 
                }
                break;
            case SWITCH_HOLD:
                app.state = CHANGE_COLOR_MODE;
                break;
            case SWITCH_PRESSED:
                if( DISPLAY_TIME_MODE == app.state )
                {
                    wasSwitchPressedInTimeMode ^= true;
                }
                break;
            default:
                app.state = DISPLAY_TIME_MODE;
                break;
        }
    }

    /* State machine execution */
    switch( app.state )
    {
        case DISPLAY_TIME_MODE:
            Time_RenderIfMinutesHaveChanged( wasSwitchPressedInTimeMode );
            break;
        case CHANGE_COLOR_MODE:
            STATE_ChangeColorMode( );
            app.state = DISPLAY_TIME_MODE; // Upon exit of this mode function, revert back to display time mode 
            break;
        case PATTERN_MODE:
            STATE_PatternMode( );
            break;
        default:
            break;
    }
    return;
}

/*  Timer 1 interrupt handler. Toggle LED  */
static void Timer1Interrupt( void )
{
    FAULT_OUT_Toggle( );
    return;
}