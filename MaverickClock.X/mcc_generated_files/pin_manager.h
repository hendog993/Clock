/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC16F18326
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.36 and above
        MPLAB 	          :  MPLAB X 6.00	
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

/**
  Section: Included Files
*/

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set FAULT_OUT aliases
#define FAULT_OUT_TRIS                 TRISCbits.TRISC0
#define FAULT_OUT_LAT                  LATCbits.LATC0
#define FAULT_OUT_PORT                 PORTCbits.RC0
#define FAULT_OUT_WPU                  WPUCbits.WPUC0
#define FAULT_OUT_OD                   ODCONCbits.ODCC0
#define FAULT_OUT_ANS                  ANSELCbits.ANSC0
#define FAULT_OUT_SetHigh()            do { LATCbits.LATC0 = 1; } while(0)
#define FAULT_OUT_SetLow()             do { LATCbits.LATC0 = 0; } while(0)
#define FAULT_OUT_Toggle()             do { LATCbits.LATC0 = ~LATCbits.LATC0; } while(0)
#define FAULT_OUT_GetValue()           PORTCbits.RC0
#define FAULT_OUT_SetDigitalInput()    do { TRISCbits.TRISC0 = 1; } while(0)
#define FAULT_OUT_SetDigitalOutput()   do { TRISCbits.TRISC0 = 0; } while(0)
#define FAULT_OUT_SetPullup()          do { WPUCbits.WPUC0 = 1; } while(0)
#define FAULT_OUT_ResetPullup()        do { WPUCbits.WPUC0 = 0; } while(0)
#define FAULT_OUT_SetPushPull()        do { ODCONCbits.ODCC0 = 0; } while(0)
#define FAULT_OUT_SetOpenDrain()       do { ODCONCbits.ODCC0 = 1; } while(0)
#define FAULT_OUT_SetAnalogMode()      do { ANSELCbits.ANSC0 = 1; } while(0)
#define FAULT_OUT_SetDigitalMode()     do { ANSELCbits.ANSC0 = 0; } while(0)

// get/set DATA_PIN aliases
#define DATA_PIN_TRIS                 TRISCbits.TRISC2
#define DATA_PIN_LAT                  LATCbits.LATC2
#define DATA_PIN_PORT                 PORTCbits.RC2
#define DATA_PIN_WPU                  WPUCbits.WPUC2
#define DATA_PIN_OD                   ODCONCbits.ODCC2
#define DATA_PIN_ANS                  ANSELCbits.ANSC2
#define DATA_PIN_SetHigh()            do { LATCbits.LATC2 = 1; } while(0)
#define DATA_PIN_SetLow()             do { LATCbits.LATC2 = 0; } while(0)
#define DATA_PIN_Toggle()             do { LATCbits.LATC2 = ~LATCbits.LATC2; } while(0)
#define DATA_PIN_GetValue()           PORTCbits.RC2
#define DATA_PIN_SetDigitalInput()    do { TRISCbits.TRISC2 = 1; } while(0)
#define DATA_PIN_SetDigitalOutput()   do { TRISCbits.TRISC2 = 0; } while(0)
#define DATA_PIN_SetPullup()          do { WPUCbits.WPUC2 = 1; } while(0)
#define DATA_PIN_ResetPullup()        do { WPUCbits.WPUC2 = 0; } while(0)
#define DATA_PIN_SetPushPull()        do { ODCONCbits.ODCC2 = 0; } while(0)
#define DATA_PIN_SetOpenDrain()       do { ODCONCbits.ODCC2 = 1; } while(0)
#define DATA_PIN_SetAnalogMode()      do { ANSELCbits.ANSC2 = 1; } while(0)
#define DATA_PIN_SetDigitalMode()     do { ANSELCbits.ANSC2 = 0; } while(0)

// get/set ROT_CLK aliases
#define ROT_CLK_TRIS                 TRISCbits.TRISC3
#define ROT_CLK_LAT                  LATCbits.LATC3
#define ROT_CLK_PORT                 PORTCbits.RC3
#define ROT_CLK_WPU                  WPUCbits.WPUC3
#define ROT_CLK_OD                   ODCONCbits.ODCC3
#define ROT_CLK_ANS                  ANSELCbits.ANSC3
#define ROT_CLK_SetHigh()            do { LATCbits.LATC3 = 1; } while(0)
#define ROT_CLK_SetLow()             do { LATCbits.LATC3 = 0; } while(0)
#define ROT_CLK_Toggle()             do { LATCbits.LATC3 = ~LATCbits.LATC3; } while(0)
#define ROT_CLK_GetValue()           PORTCbits.RC3
#define ROT_CLK_SetDigitalInput()    do { TRISCbits.TRISC3 = 1; } while(0)
#define ROT_CLK_SetDigitalOutput()   do { TRISCbits.TRISC3 = 0; } while(0)
#define ROT_CLK_SetPullup()          do { WPUCbits.WPUC3 = 1; } while(0)
#define ROT_CLK_ResetPullup()        do { WPUCbits.WPUC3 = 0; } while(0)
#define ROT_CLK_SetPushPull()        do { ODCONCbits.ODCC3 = 0; } while(0)
#define ROT_CLK_SetOpenDrain()       do { ODCONCbits.ODCC3 = 1; } while(0)
#define ROT_CLK_SetAnalogMode()      do { ANSELCbits.ANSC3 = 1; } while(0)
#define ROT_CLK_SetDigitalMode()     do { ANSELCbits.ANSC3 = 0; } while(0)

// get/set ROT_DT aliases
#define ROT_DT_TRIS                 TRISCbits.TRISC4
#define ROT_DT_LAT                  LATCbits.LATC4
#define ROT_DT_PORT                 PORTCbits.RC4
#define ROT_DT_WPU                  WPUCbits.WPUC4
#define ROT_DT_OD                   ODCONCbits.ODCC4
#define ROT_DT_ANS                  ANSELCbits.ANSC4
#define ROT_DT_SetHigh()            do { LATCbits.LATC4 = 1; } while(0)
#define ROT_DT_SetLow()             do { LATCbits.LATC4 = 0; } while(0)
#define ROT_DT_Toggle()             do { LATCbits.LATC4 = ~LATCbits.LATC4; } while(0)
#define ROT_DT_GetValue()           PORTCbits.RC4
#define ROT_DT_SetDigitalInput()    do { TRISCbits.TRISC4 = 1; } while(0)
#define ROT_DT_SetDigitalOutput()   do { TRISCbits.TRISC4 = 0; } while(0)
#define ROT_DT_SetPullup()          do { WPUCbits.WPUC4 = 1; } while(0)
#define ROT_DT_ResetPullup()        do { WPUCbits.WPUC4 = 0; } while(0)
#define ROT_DT_SetPushPull()        do { ODCONCbits.ODCC4 = 0; } while(0)
#define ROT_DT_SetOpenDrain()       do { ODCONCbits.ODCC4 = 1; } while(0)
#define ROT_DT_SetAnalogMode()      do { ANSELCbits.ANSC4 = 1; } while(0)
#define ROT_DT_SetDigitalMode()     do { ANSELCbits.ANSC4 = 0; } while(0)

// get/set ROT_SW aliases
#define ROT_SW_TRIS                 TRISCbits.TRISC5
#define ROT_SW_LAT                  LATCbits.LATC5
#define ROT_SW_PORT                 PORTCbits.RC5
#define ROT_SW_WPU                  WPUCbits.WPUC5
#define ROT_SW_OD                   ODCONCbits.ODCC5
#define ROT_SW_ANS                  ANSELCbits.ANSC5
#define ROT_SW_SetHigh()            do { LATCbits.LATC5 = 1; } while(0)
#define ROT_SW_SetLow()             do { LATCbits.LATC5 = 0; } while(0)
#define ROT_SW_Toggle()             do { LATCbits.LATC5 = ~LATCbits.LATC5; } while(0)
#define ROT_SW_GetValue()           PORTCbits.RC5
#define ROT_SW_SetDigitalInput()    do { TRISCbits.TRISC5 = 1; } while(0)
#define ROT_SW_SetDigitalOutput()   do { TRISCbits.TRISC5 = 0; } while(0)
#define ROT_SW_SetPullup()          do { WPUCbits.WPUC5 = 1; } while(0)
#define ROT_SW_ResetPullup()        do { WPUCbits.WPUC5 = 0; } while(0)
#define ROT_SW_SetPushPull()        do { ODCONCbits.ODCC5 = 0; } while(0)
#define ROT_SW_SetOpenDrain()       do { ODCONCbits.ODCC5 = 1; } while(0)
#define ROT_SW_SetAnalogMode()      do { ANSELCbits.ANSC5 = 1; } while(0)
#define ROT_SW_SetDigitalMode()     do { ANSELCbits.ANSC5 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);


/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handler for the IOCCF3 pin functionality
 * @Example
    IOCCF3_ISR();
 */
void IOCCF3_ISR(void);

/**
  @Summary
    Interrupt Handler Setter for IOCCF3 pin interrupt-on-change functionality

  @Description
    Allows selecting an interrupt handler for IOCCF3 at application runtime
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    InterruptHandler function pointer.

  @Example
    PIN_MANAGER_Initialize();
    IOCCF3_SetInterruptHandler(MyInterruptHandler);

*/
void IOCCF3_SetInterruptHandler(void (* InterruptHandler)(void));

/**
  @Summary
    Dynamic Interrupt Handler for IOCCF3 pin

  @Description
    This is a dynamic interrupt handler to be used together with the IOCCF3_SetInterruptHandler() method.
    This handler is called every time the IOCCF3 ISR is executed and allows any function to be registered at runtime.
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCCF3_SetInterruptHandler(IOCCF3_InterruptHandler);

*/
extern void (*IOCCF3_InterruptHandler)(void);

/**
  @Summary
    Default Interrupt Handler for IOCCF3 pin

  @Description
    This is a predefined interrupt handler to be used together with the IOCCF3_SetInterruptHandler() method.
    This handler is called every time the IOCCF3 ISR is executed. 
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCCF3_SetInterruptHandler(IOCCF3_DefaultInterruptHandler);

*/
void IOCCF3_DefaultInterruptHandler(void);


/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handler for the IOCCF4 pin functionality
 * @Example
    IOCCF4_ISR();
 */
void IOCCF4_ISR(void);

/**
  @Summary
    Interrupt Handler Setter for IOCCF4 pin interrupt-on-change functionality

  @Description
    Allows selecting an interrupt handler for IOCCF4 at application runtime
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    InterruptHandler function pointer.

  @Example
    PIN_MANAGER_Initialize();
    IOCCF4_SetInterruptHandler(MyInterruptHandler);

*/
void IOCCF4_SetInterruptHandler(void (* InterruptHandler)(void));

/**
  @Summary
    Dynamic Interrupt Handler for IOCCF4 pin

  @Description
    This is a dynamic interrupt handler to be used together with the IOCCF4_SetInterruptHandler() method.
    This handler is called every time the IOCCF4 ISR is executed and allows any function to be registered at runtime.
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCCF4_SetInterruptHandler(IOCCF4_InterruptHandler);

*/
extern void (*IOCCF4_InterruptHandler)(void);

/**
  @Summary
    Default Interrupt Handler for IOCCF4 pin

  @Description
    This is a predefined interrupt handler to be used together with the IOCCF4_SetInterruptHandler() method.
    This handler is called every time the IOCCF4 ISR is executed. 
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCCF4_SetInterruptHandler(IOCCF4_DefaultInterruptHandler);

*/
void IOCCF4_DefaultInterruptHandler(void);


/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handler for the IOCCF5 pin functionality
 * @Example
    IOCCF5_ISR();
 */
void IOCCF5_ISR(void);

/**
  @Summary
    Interrupt Handler Setter for IOCCF5 pin interrupt-on-change functionality

  @Description
    Allows selecting an interrupt handler for IOCCF5 at application runtime
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    InterruptHandler function pointer.

  @Example
    PIN_MANAGER_Initialize();
    IOCCF5_SetInterruptHandler(MyInterruptHandler);

*/
void IOCCF5_SetInterruptHandler(void (* InterruptHandler)(void));

/**
  @Summary
    Dynamic Interrupt Handler for IOCCF5 pin

  @Description
    This is a dynamic interrupt handler to be used together with the IOCCF5_SetInterruptHandler() method.
    This handler is called every time the IOCCF5 ISR is executed and allows any function to be registered at runtime.
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCCF5_SetInterruptHandler(IOCCF5_InterruptHandler);

*/
extern void (*IOCCF5_InterruptHandler)(void);

/**
  @Summary
    Default Interrupt Handler for IOCCF5 pin

  @Description
    This is a predefined interrupt handler to be used together with the IOCCF5_SetInterruptHandler() method.
    This handler is called every time the IOCCF5 ISR is executed. 
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCCF5_SetInterruptHandler(IOCCF5_DefaultInterruptHandler);

*/
void IOCCF5_DefaultInterruptHandler(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/