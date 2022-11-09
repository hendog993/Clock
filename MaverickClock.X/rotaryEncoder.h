/* Filename: rotaryEncoder.h
 *
 * Date: 11 September 2022
 * 
 * Author: Henry Gilbert
 * 
 * Description: Public interface to the rotary encoder driver. 
 *      
 * 
 */

#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H

/******************** Included File(s) *********/
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/******************* Type definition(s) ********/

typedef enum
{
    SWITCH_RESTING,
    SWITCH_PRESSED,
    SWITCH_DOUBLE_CLICK,
    SWITCH_HOLD
} RotarySwitchState;

typedef enum
{
    ROT_ENC_CW_DIR,
    ROT_ENC_CCW_DIR
} ROTARY_ENCODER_DIRECTION;

/* Configuration and data structures for the rotary push button. 
 *
 * 
 * 
 */
typedef struct
{
    volatile RotarySwitchState currentState;
    volatile uint16_t lastRisingEdgeTimestamp_cts;
    volatile uint16_t lastFallingEdgeTimestamp_cts;
    volatile bool hasSwitchBeenPressed;
    volatile size_t currentNumMulticlicks;

    struct
    {
        const uint16_t multiClickTimerThreshold_cts; // Number of timer counts required for a double click to be registered 
        const uint16_t longPressTimerThreshold_cts; // Number of timer counts required for a "press" mode to be detected
        const uint16_t multiClickTimeFreshnessLimit_cts; // Number of time counts until the current multiclick counter is reset. 
    } config;
} RotaryPushButton;

/* Configuration and data structures for rotary shaft encoder. 
 * 
 * uint8_t vector - written during interrupt. Saves the last read and the current read
 * int32_t counts - updated based on rotary turn direction. Polled and reset during mainloop
 * currentDirection - saves the encoders current direction
 * 
 * 
 */
typedef struct
{
    volatile uint8_t vector;
    volatile int32_t counts;
    ROTARY_ENCODER_DIRECTION currentDirection;

    uint16_t currentTimestamp_tr; // tr: Comes from timer register
    uint16_t velocity_tps; // current velocity in turns per second. 

    struct
    {
        const uint16_t velocityTolerance_tps;
    } config;

} ShaftEncoder;

typedef struct
{
    RotaryPushButton rotBtn;
    ShaftEncoder shaft;
} RotaryEncoder;

/* Function: 
 *      RotaryEncoder_Init 
 * 
 * Description: 
 *      Links the global rotary encoder structure to the local pointer. Sets up callback functions based on 
 *      interrupt functions. 
 * 
 * Return: 
 *      True if setup was successful, false if null pointer. 
 */
bool RotaryEncoder_Init(RotaryEncoder * const rotEncPtr);


/* Sets relevant rotary encoder parameters to zero, starts timer 3, and enables external int. */
void RotaryEncoder_Enable(void);

/* Disable external interrupts, sets enabled to false, and stops timer3. */
void RotaryEncoder_Disable(void);


/* Function: 
 *      RotaryEncoder_GetCounts
 * 
 * Description: 
 *      Function to poll when the current shaft counts are wanted.  Returns the current shaft count value and resets the counts value to zero. 
 * 
 * Return: 
 *      Rotary encoder shaft count value
 */
int32_t RotaryEncoder_GetShaftCounts(void);

/* Function: 
 *      RotaryEncoder_GetSwitchState
 * 
 * Description: 
 *      Returns the most recent switch state. Only checks for an update if the  hasBeenPressed flag is set. Otherwise, 
 *      returns the latest valid state. 
 */
RotarySwitchState RotaryEncoder_GetSwitchState(void);


/* Function:
 *      RotaryEncoder_HasButtonPressOccurred
 * 
 * Description: 
 *      Returns a button pressed flag to the external caller. Acts as a "service" flag.  Resets the HasButtonPressOccurred back to false. 
 * 
 */
bool RotaryEncoder_HasButtonPressOccurred(void);



#endif