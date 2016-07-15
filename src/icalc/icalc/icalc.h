/*
 * C Interface between calcterm and a calculator.
 * A shared library must implement this interface
 * to be loadable by calcterm.
 */

extern "C" {

struct CI_Config {
    int flag;
};

struct CI_Result {
    char*  one_line;
    char** grid;
    int    y;
};

void         CI_init( CI_Config* config );
CI_Result*   CI_submit( char const* input );
void         CI_result_release( CI_Result* result );

} /* extern "C" */
