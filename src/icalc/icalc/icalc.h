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

typedef      void (*CI_init_t)( CI_Config* );
void         CI_init( CI_Config* config );

typedef      CI_Result* (*CI_submit_t)( char const* );
CI_Result*   CI_submit( char const* input );

typedef      void (*CI_result_release_t)( CI_Result* result );
void         CI_result_release( CI_Result* result );

} /* extern "C" */
