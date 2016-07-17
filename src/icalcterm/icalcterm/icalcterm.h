/*
 * C Interface between calcterm and a calculator.
 * A shared library must implement this interface
 * to be loadable by calcterm.
 */

extern "C" {

typedef struct _CI_Config {

    int flag;

}   CI_Config;

/*
 * Holds all of the info that you get back from the
 * calculator.  These must be freed by calling the
 * CI_result_free function.
 */
typedef struct _CI_Result {

    char*  input_one_line;
    char*  output_one_line;

    char** input_grid;
    char** output_grid;

    int    input_grid_rows;
    int    output_grid_rows;

}   CI_Result;

/*
 * Initialize the calculator
 */
typedef void       (*CI_init)( CI_Config* );

/*
 * Set the configuration of the calculator
 */
typedef void       (*CI_config)( CI_Config* );

/*
 * Submit a string for processing
 */
typedef CI_Result* (*CI_submit)( char const* );

/*
 * This must be used to free a CI_Result object
 */
typedef void       (*CI_result_free)( CI_Result* result );

} /* extern "C" */
