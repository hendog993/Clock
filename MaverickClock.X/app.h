/* Filename: app.h
 *
 * Date: 8 September 2022
 *
 * Author: Henry Gilbert
 *
 * Description: Declaration of extern app structure.
 *
 *
 *  * Notes: Timer 1 is the fault out light timer.
 *        Timer 3 is the rotary encoder period register counter for encoder velocity
 *        Timer 5 is the running clock timer that keeps track of elapsed time.
 *
 */

/*********************** Included File(s) ***************/
#include <stdint.h>
#include <stdbool.h>

#ifndef APP_H
#define APP_H

/* State machine structs */
typedef enum
{
    DISPLAY_TIME_MODE,
    CHANGE_COLOR_MODE,
    PATTERN_MODE,
} APP_STATE;

typedef struct
{
    struct
    {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
    } BackgroundColor;

    struct
    {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
    } DigitColor;

} nvmColorSettings;

typedef struct // TODO
{
    bool isRotaryEncoderValid;
    bool isLEDSetupValid;
    bool isLEDWriteValid; // Error flag for LED writes. Determines out of bounds writes.
} ApplicationStatus;

typedef struct
{
    APP_STATE state;
    nvmColorSettings colors;
    ApplicationStatus status;
} APP;

/* Function:
 *      APP_Initialize
 *
 * Description:
 *      Initializes the relevant modules
 */
void APP_Initialize(void);


/* Function:
 *      APP_TASKS
 *
 * Description:
 *      If a button change has been detected, update the app's current state. Otherwise, execute the current state. Used to
 *      prevent unnesessary polling of the encoder's switch state.
 */
void APP_TASKS(void);






#endif
