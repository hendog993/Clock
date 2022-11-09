#include "mcc_generated_files/mcc.h"
#include "app.h"

void main( void )
{
    SYSTEM_Initialize( );
    APP_Initialize( );
    INTERRUPT_GlobalInterruptEnable( );
    INTERRUPT_PeripheralInterruptEnable( );
    while( 1 )
    {
        APP_TASKS( );
    }
    return;
}
