# 1 "patternMode.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 288 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:/Users/henry/.mchp_packs/Microchip/PIC16F1xxxx_DFP/1.6.143/xc8\\pic\\include\\language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "patternMode.c" 2
# 13 "patternMode.c"
# 1 "./patternMode.h" 1
# 24 "./patternMode.h"
void STATE_PatternMode(void);
# 35 "./patternMode.h"
void State_SetFunctionLookupTable(void);
# 13 "patternMode.c" 2

# 1 "./clockLEDs.h" 1
# 15 "./clockLEDs.h"
# 1 "C:\\Program Files\\Microchip\\xc8\\v2.36\\pic\\include\\c99\\stdint.h" 1 3



# 1 "C:\\Program Files\\Microchip\\xc8\\v2.36\\pic\\include\\c99\\musl_xc8.h" 1 3
# 4 "C:\\Program Files\\Microchip\\xc8\\v2.36\\pic\\include\\c99\\stdint.h" 2 3
# 22 "C:\\Program Files\\Microchip\\xc8\\v2.36\\pic\\include\\c99\\stdint.h" 3
# 1 "C:\\Program Files\\Microchip\\xc8\\v2.36\\pic\\include\\c99\\bits/alltypes.h" 1 3
# 127 "C:\\Program Files\\Microchip\\xc8\\v2.36\\pic\\include\\c99\\bits/alltypes.h" 3
typedef unsigned long uintptr_t;
# 142 "C:\\Program Files\\Microchip\\xc8\\v2.36\\pic\\include\\c99\\bits/alltypes.h" 3
typedef long intptr_t;
# 158 "C:\\Program Files\\Microchip\\xc8\\v2.36\\pic\\include\\c99\\bits/alltypes.h" 3
typedef signed char int8_t;




typedef short int16_t;




typedef __int24 int24_t;




typedef long int32_t;





typedef long long int64_t;
# 188 "C:\\Program Files\\Microchip\\xc8\\v2.36\\pic\\include\\c99\\bits/alltypes.h" 3
typedef long long intmax_t;





typedef unsigned char uint8_t;




typedef unsigned short uint16_t;




typedef __uint24 uint24_t;




typedef unsigned long uint32_t;





typedef unsigned long long uint64_t;
# 229 "C:\\Program Files\\Microchip\\xc8\\v2.36\\pic\\include\\c99\\bits/alltypes.h" 3
typedef unsigned long long uintmax_t;
# 22 "C:\\Program Files\\Microchip\\xc8\\v2.36\\pic\\include\\c99\\stdint.h" 2 3


typedef int8_t int_fast8_t;

typedef int64_t int_fast64_t;


typedef int8_t int_least8_t;
typedef int16_t int_least16_t;

typedef int24_t int_least24_t;
typedef int24_t int_fast24_t;

typedef int32_t int_least32_t;

typedef int64_t int_least64_t;


typedef uint8_t uint_fast8_t;

typedef uint64_t uint_fast64_t;


typedef uint8_t uint_least8_t;
typedef uint16_t uint_least16_t;

typedef uint24_t uint_least24_t;
typedef uint24_t uint_fast24_t;

typedef uint32_t uint_least32_t;

typedef uint64_t uint_least64_t;
# 144 "C:\\Program Files\\Microchip\\xc8\\v2.36\\pic\\include\\c99\\stdint.h" 3
# 1 "C:\\Program Files\\Microchip\\xc8\\v2.36\\pic\\include\\c99\\bits/stdint.h" 1 3
typedef int16_t int_fast16_t;
typedef int32_t int_fast32_t;
typedef uint16_t uint_fast16_t;
typedef uint32_t uint_fast32_t;
# 144 "C:\\Program Files\\Microchip\\xc8\\v2.36\\pic\\include\\c99\\stdint.h" 2 3
# 15 "./clockLEDs.h" 2

# 1 "C:\\Program Files\\Microchip\\xc8\\v2.36\\pic\\include\\c99\\stdlib.h" 1 3
# 10 "C:\\Program Files\\Microchip\\xc8\\v2.36\\pic\\include\\c99\\stdlib.h" 3
# 1 "C:\\Program Files\\Microchip\\xc8\\v2.36\\pic\\include\\c99\\features.h" 1 3
# 10 "C:\\Program Files\\Microchip\\xc8\\v2.36\\pic\\include\\c99\\stdlib.h" 2 3
# 21 "C:\\Program Files\\Microchip\\xc8\\v2.36\\pic\\include\\c99\\stdlib.h" 3
# 1 "C:\\Program Files\\Microchip\\xc8\\v2.36\\pic\\include\\c99\\bits/alltypes.h" 1 3
# 18 "C:\\Program Files\\Microchip\\xc8\\v2.36\\pic\\include\\c99\\bits/alltypes.h" 3
typedef long int wchar_t;
# 122 "C:\\Program Files\\Microchip\\xc8\\v2.36\\pic\\include\\c99\\bits/alltypes.h" 3
typedef unsigned size_t;
# 21 "C:\\Program Files\\Microchip\\xc8\\v2.36\\pic\\include\\c99\\stdlib.h" 2 3


int atoi (const char *);
long atol (const char *);
long long atoll (const char *);
double atof (const char *);

float strtof (const char *restrict, char **restrict);
double strtod (const char *restrict, char **restrict);
long double strtold (const char *restrict, char **restrict);



long strtol (const char *restrict, char **restrict, int);
unsigned long strtoul (const char *restrict, char **restrict, int);
long long strtoll (const char *restrict, char **restrict, int);
unsigned long long strtoull (const char *restrict, char **restrict, int);

int rand (void);
void srand (unsigned);

void *malloc (size_t);
void *calloc (size_t, size_t);
void *realloc (void *, size_t);
void free (void *);

          void abort (void);
int atexit (void (*) (void));
          void exit (int);
          void _Exit (int);

void *bsearch (const void *, const void *, size_t, size_t, int (*)(const void *, const void *));







__attribute__((nonreentrant)) void qsort (void *, size_t, size_t, int (*)(const void *, const void *));

int abs (int);
long labs (long);
long long llabs (long long);

typedef struct { int quot, rem; } div_t;
typedef struct { long quot, rem; } ldiv_t;
typedef struct { long long quot, rem; } lldiv_t;

div_t div (int, int);
ldiv_t ldiv (long, long);
lldiv_t lldiv (long long, long long);

typedef struct { unsigned int quot, rem; } udiv_t;
typedef struct { unsigned long quot, rem; } uldiv_t;
udiv_t udiv (unsigned int, unsigned int);
uldiv_t uldiv (unsigned long, unsigned long);
# 16 "./clockLEDs.h" 2

# 1 "C:\\Program Files\\Microchip\\xc8\\v2.36\\pic\\include\\c99\\stdbool.h" 1 3
# 17 "./clockLEDs.h" 2



typedef struct
{
    uint8_t digit1;
    uint8_t digit2;
    uint8_t digit3;
    uint8_t digit4;
} TimeInDigits;
# 44 "./clockLEDs.h"
_Bool Clock_InitializeClockLEDs(const size_t numElements,
        const uint8_t digitRed,
        const uint8_t digitGreen,
        const uint8_t digitBlue,
        const uint8_t backgroundRed,
        const uint8_t backgroundGreen,
        const uint8_t backgroundBlue);







void Clock_SetBackgroundRGBArray(const uint8_t red,
        const uint8_t green,
        const uint8_t blue);






void Clock_SetDigitRGBArray(const uint8_t red,
        const uint8_t green,
        const uint8_t blue);
# 85 "./clockLEDs.h"
void Clock_WriteTimeDigitValuesAndRenderScreen(const TimeInDigits * const digits);
# 101 "./clockLEDs.h"
void Clock_PrerenderPixelAndBackgroundValues(void);
# 113 "./clockLEDs.h"
void Clock_ForceRender(const TimeInDigits * const t);
# 127 "./clockLEDs.h"
void Clock_IterateSinglePixelByIndex(void);







void Clock_CrossingRainbowPattern(void);





void Clock_CrossingRainbowPatternwithDelays(void);






void Clock_Popcorn_Pattern(void);






void Clock_Popcorn_Pattern_Hold(void);
# 14 "patternMode.c" 2

# 1 "./rotaryEncoder.h" 1
# 22 "./rotaryEncoder.h"
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






typedef struct
{
    volatile RotarySwitchState currentState;
    volatile uint16_t lastRisingEdgeTimestamp_cts;
    volatile uint16_t lastFallingEdgeTimestamp_cts;
    volatile _Bool hasSwitchBeenPressed;
    volatile size_t currentNumMulticlicks;

    struct
    {
        const uint16_t multiClickTimerThreshold_cts;
        const uint16_t longPressTimerThreshold_cts;
        const uint16_t multiClickTimeFreshnessLimit_cts;
    } config;
} RotaryPushButton;
# 65 "./rotaryEncoder.h"
typedef struct
{
    volatile uint8_t vector;
    volatile int32_t counts;
    ROTARY_ENCODER_DIRECTION currentDirection;

    uint16_t currentTimestamp_tr;
    uint16_t velocity_tps;

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
# 97 "./rotaryEncoder.h"
_Bool RotaryEncoder_Init(RotaryEncoder * const rotEncPtr);



void RotaryEncoder_Enable(void);


void RotaryEncoder_Disable(void);
# 116 "./rotaryEncoder.h"
int32_t RotaryEncoder_GetShaftCounts(void);
# 125 "./rotaryEncoder.h"
RotarySwitchState RotaryEncoder_GetSwitchState(void);
# 135 "./rotaryEncoder.h"
_Bool RotaryEncoder_HasButtonPressOccurred(void);
# 15 "patternMode.c" 2
# 24 "patternMode.c"
static void (*patterns[5u])(void);




void STATE_PatternMode(void)
{




    static int32_t patternIndex = 0;
    int32_t rotCounts = RotaryEncoder_GetShaftCounts();

    if (rotCounts)
    {
        patternIndex += rotCounts;
        patternIndex = ( (patternIndex < 0) ? 5u -1 : (patternIndex > 5u -1) ? 0 : patternIndex);
    }

    (*patterns[patternIndex])();
    return;
}

void State_SetFunctionLookupTable(void)
{
    patterns[0] = Clock_IterateSinglePixelByIndex;
    patterns[1] = Clock_CrossingRainbowPattern;
    patterns[2] = Clock_CrossingRainbowPatternwithDelays;
    patterns[3] = Clock_Popcorn_Pattern;
    patterns[4] = Clock_Popcorn_Pattern_Hold;
}
