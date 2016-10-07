//// Basic implementation of the icalcterm interface

#include "icalcterm/icalcterm.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sstream>

#define EXPORT __attribute__ ((visibility ("default"))) 

bool parse_number( char const* input, double& result );

extern "C" {

void EXPORT CI_init( CI_Config* config )
{
    // Not implemented
}

void EXPORT CI_config( CI_Config* )
{
    // Not implemented
}

static CI_ResultComponent component( char const* s )
{
    CI_ResultComponent res;
    res.one_line  = strdup( s );
    res.grid_rows = 1;
    res.grid      = new char*[1];
    res.grid[0]   = strdup( s );
    return res;
}

// Note: this function returns a pointer, but for some strange
// reason we must put the asterisk after the EXPORT.
CI_Result EXPORT * CI_submit( char const* input )
{
    double number;
    if( !parse_number( input, number ) )
        return NULL;

    std::ostringstream ss;
    ss << number;
    std::string result = ss.str();

    CI_Result* res = new CI_Result;

    res->input       = component( input );
    res->num_outputs = 1;
    res->outputs     = new CI_ResultComponent[1];
    res->outputs[0]  = component( result.c_str() );

    return res;
}

static void component_free( CI_ResultComponent& comp )
{
    free( comp.one_line );

    for( int i = 0; i < comp.grid_rows; ++i )
        free( comp.grid[i] );

    delete[] comp.grid;
}

void EXPORT CI_result_free( CI_Result* result )
{
    if( !result )
        return;

    component_free( result->input );

    for( int i = 0; i < result->num_outputs; ++i )
        component_free( result->outputs[i] );

    delete[] result->outputs;

    delete result;
}

} // extern "C"
