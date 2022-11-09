/* Filename: rotaryEncoder.c
 *
 * Author: Henry Gilbert
 * 
 * Date: 11 September 2022
 * 
 * Description: Software driver for CyleWet KY-040 rotary encoder. Rotary encoder inputs 
 *      are filtered through an RC debounce circuit and input to an inverting schmitt 
 *      trigger (SN74HC14N). Using MCC, a generic driver for multiple 
 *      rotary encoders per project won't work. Each instance of a rotary encoder
 *      requires a different file or at least more functions added for each encoder. 
 * 
 *      Rotary encoder module is linked to the TIMER3 module, and the timer3 callback and 
 *      enable/disable are handled in this module. Timer3 is currently configured with a 
 *      129uS per register increment resolution. 
 * 
 *      The CyleWet KY-040 uses 3 pins (CLK, DT, and SW). The inputs to the microcontroller
 *      are all active high. 
 * 
 */


/********************* Included File(s) ************/
#include "rotaryEncoder.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/tmr3.h"

/********************** Local Variable(s) **********/
static volatile RotaryEncoder * rot;
static bool isRotaryEncoderEnabled = false;


/*************** Local Function Prototype(s) *******/
static void RotaryEncoder_ReadShaftISR( void );
static void RotaryEncoder_ReadButtonISR( void );

/******************** Public Functions *************/

bool RotaryEncoder_Init( RotaryEncoder * const rotEncPtr )
{
    if( NULL == rotEncPtr )
    {
        return false;
    }

    rot = rotEncPtr;
    IOCCF3_SetInterruptHandler( RotaryEncoder_ReadShaftISR );
    IOCCF4_SetInterruptHandler( RotaryEncoder_ReadShaftISR );
    IOCCF5_SetInterruptHandler( RotaryEncoder_ReadButtonISR );
    return true;
}

void RotaryEncoder_Enable( void )
{
    rot->shaft.counts = 0u;
    rot->shaft.vector = 0u;
    rot->shaft.currentTimestamp_tr = TMR3_ReadTimer( );
    rot->shaft.velocity_tps = 0u;

    rot->rotBtn.currentState = SWITCH_RESTING;
    rot->rotBtn.lastFallingEdgeTimestamp_cts = 0u;
    rot->rotBtn.lastRisingEdgeTimestamp_cts = 0u;
    rot->rotBtn.currentNumMulticlicks = 0;
    rot->rotBtn.hasSwitchBeenPressed = false;

    TMR3_StartTimer( );
    isRotaryEncoderEnabled = true;
}

void RotaryEncoder_Disable( void )
{
    TMR3_StopTimer( );
    isRotaryEncoderEnabled = false;
}

int32_t RotaryEncoder_GetShaftCounts( void )
{
    volatile int32_t thisCountValue = rot->shaft.counts;
    rot->shaft.counts = 0;
    return thisCountValue;
}

RotarySwitchState RotaryEncoder_GetSwitchState( void )
{
    RotarySwitchState currentState = rot->rotBtn.currentState;
    rot->rotBtn.currentState = SWITCH_RESTING;

    return currentState;
}

bool RotaryEncoder_HasButtonPressOccurred( void )
{
    if( rot->rotBtn.hasSwitchBeenPressed )
    {
        rot->rotBtn.hasSwitchBeenPressed = false;
        return true;
    }
    else
    {
        return false;
    }
}


/*********************** Private Function(s) *******************/

/* Function: 
 *      RotaryEncoder_Read
 * 
 * Description: 
 *      Reads the states of the rotary encoder pins and changes the counts based on the velocity and direction. 
 * 
 * Rotary vector format: 0b0000 (Last CK) (Last DT) (Current CK) (Current DT)
 * Based on above vector format and read pins on change interrupt, every vector will go in this order:
 *      CW Order: 11->6->13->10->4->9->2->5
 *     CCW Order: 7-15->14->12->8->0->1->3
 * Select the count incrementer value based on the desired pulses per detent.  
 * 
 */
static void RotaryEncoder_ReadShaftISR( void )
{
    if( isRotaryEncoderEnabled )
    {
        /* Perserve old rotary encoder values */
        rot->shaft.vector = ( rot->shaft.vector << 2u ) & 0b00001100;

        /* Read the current state of both pins  */
        rot->shaft.vector |= ( ROT_DT_GetValue( ) ) ? 0x2u : 0x0u; 
        rot->shaft.vector |= ( ROT_CLK_GetValue( ) ) ? 0x1u : 0x0u;

        /* Save current direction */
        ROTARY_ENCODER_DIRECTION currentDirection = rot->shaft.currentDirection;

        /* Convert last and current pin state vector to counts */
        switch( rot->shaft.vector )
        {
            case 0b1000: // Clockwise case
                rot->shaft.counts++;
                rot->shaft.currentDirection = ROT_ENC_CW_DIR;
                break;
            case 0b0100: // Counter clockwise case 
                rot->shaft.counts--;
                rot->shaft.currentDirection = ROT_ENC_CCW_DIR;
                break;
            default:
                break;
        }
    }
    return;
}

/*
 * Function:         
 *      RotaryEncoder_ReadButtonISR
 *
 * Description: 
 *      ISR calls after ever edge change (both rising and falling). Reads the state of the  switch pin to determine if a switch was 
 *      pressed or released. Saves the current state of the button and 
 * 
 */
static void RotaryEncoder_ReadButtonISR( void )
{
    if( true == isRotaryEncoderEnabled )
    {
        /* Get the current timestamp */
        uint16_t currentTimestamp = TMR3_ReadTimer( );
        uint16_t positiveWidth_cts;

        /* Get the pin pressed state */
        switch( ROT_SW_GetValue( ) )
        {
            case 0x01: // Rising edge - switch is pressed. 
                rot->rotBtn.lastRisingEdgeTimestamp_cts = currentTimestamp;
                break;
            case 0x00: // Switch is released, falling edge. 
                positiveWidth_cts = currentTimestamp - rot->rotBtn.lastRisingEdgeTimestamp_cts;

                /* Check for a long press condition. Long press takes priority over multi click */
                if( positiveWidth_cts > rot->rotBtn.config.longPressTimerThreshold_cts )
                {
                    rot->rotBtn.currentState = SWITCH_HOLD;
                }

                    /* Check for double click condition. Last two falling edges were ~350ms apart. */
                else if( currentTimestamp - rot->rotBtn.lastFallingEdgeTimestamp_cts < rot->rotBtn.config.multiClickTimerThreshold_cts )
                {
                    rot->rotBtn.currentNumMulticlicks++;
                    rot->rotBtn.currentState = SWITCH_DOUBLE_CLICK;
                }

                else
                {
                    rot->rotBtn.currentState = SWITCH_PRESSED;

                }

                rot->rotBtn.hasSwitchBeenPressed = true;
                rot->rotBtn.lastFallingEdgeTimestamp_cts = currentTimestamp;
                break;
            default:
                // This should never happen. 
                break;
        }
    }
    return;
}