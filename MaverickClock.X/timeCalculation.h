/* Filename: timeCalculation.h
 *
 * Author: Henry Gilbert
 * 
 * Date: 12 September 2022 
 * 
 * Description: Prototype for primary polling function in mainloop. 
 */

#ifndef TIME_CALCULATION_H
#define TIME_CALCULATION_H
#include <stdbool.h>
#include <stdint.h>

/************************ Function Prototype(s) ***************/

/* Function:
 *      Time_InitializeTimeModule
 *
 * Description:
 *      Links the timer interrupt to the Timer 5 callback register.
 *
 */
void Time_InitializeTimeModule(void);



/* Function:
 *      Time_EnableTimeModule
 *
 * Description:
 *      Writes the value saved if the timer was disabled (0 if initial). Starts timer.
 */
void Time_EnableTimeModule(void);



/* Function:
 *      Time_DisableTimeModule
 *
 * Description:
 *      Saves the current timer state for recovery. Stops timer.
 */
void Time_DisableTimeModule(void);



/* Function:
 *      Time_RenderIfMinutesHaveChanged()
 *
 * Description:
 *      Function periodically polled in the mainloop to determine if
 *      a time update (in minutes) has occurred.
 */
void Time_RenderIfMinutesHaveChanged(bool isSwitchPressHrMnMode);


/* Function:
 *      Time_GetCurrentTimeInSeconds
 * 
 * Description:
 *      Returns time 
 */
uint32_t Time_GetCurrentTimeInSeconds(void);


/* Function:
 *      Time_RenderInputTime 
 * 
 * Description:
 *      
 */
void Time_RenderInputTime(uint32_t timeInSeconds,
        bool setCurrentTime);

#endif 


/* end timeCalculation.h header file*/