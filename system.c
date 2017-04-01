/******************************************************************************/
/*Files to Include                                                            */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>    /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#endif

#include "system.h"

/* Refer to the device datasheet for information about available
oscillator configurations. */
void ConfigureOscillator(void)
{
    OSCCONbits.IRCF = 0b110;
    OSCTUNEbits.PLLEN = 0b1;
    
}

/**
 * Wait for a given number of milli-seconds using busy waiting scheme.
 * @param time - time in ms to wait.
 */
void wait_ms(uint16_t time)
{
    static long timel = 0;
    timel = time * 1000l;
    for( ; timel; timel--);// no initial condition, while time is >0, decrement time each loop
}
