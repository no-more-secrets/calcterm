//// Basic implementation of the icalcterm interface

#include "icalc/icalc.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define EXPORT __attribute__ ((visibility ("default"))) 

extern "C" {

void EXPORT CI_init( CI_Config* config )
{
    printf("Starting defcalc.\n");
}

// Note: this function returns a pointer, but for some strange
// reason we must put the asterisk after the EXPORT.
CI_Result EXPORT * CI_submit( char const* input )
{
    CI_Result* res = new CI_Result;
    res->one_line = strdup( input );
    res->y = 3;
    res->grid = new char*[res->y];
    for( int i = 0; i < res->y; ++i )
        res->grid[i] = strdup( input );
    return res;
}

void EXPORT CI_result_release( CI_Result* result )
{
    free( result->one_line );
    for( int i = 0; i < result->y; ++i )
        free( result->grid[i] );
    delete[] result->grid;
    delete result;
}

} // extern "C"
