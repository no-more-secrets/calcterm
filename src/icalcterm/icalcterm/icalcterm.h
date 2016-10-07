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
 * Holds a single component of a result.  These
 * should only exist inside the CI_Result structure.
 */
typedef struct _CI_ResultComponent {

    char*  one_line;
    char** grid;
    int    grid_rows;

}   CI_ResultComponent;

/*
 * Holds all of the info that you get back from the
 * calculator.  These must be freed by calling the
 * CI_result_free function.
 */
typedef struct _CI_Result {

    CI_ResultComponent  input;
    CI_ResultComponent* outputs;

    int num_outputs;

}   CI_Result;

/*
 * Initialize the calculator
 */
typedef void       ( *CI_init_t )( CI_Config* );

/*
 * Set the configuration of the calculator
 */
typedef void       ( *CI_config_t )( CI_Config* );

/*
 * Submit a string for processing
 */
typedef CI_Result* ( *CI_submit_t )( char const* );

/*
 * This must be used to free a CI_Result object
 */
typedef void       ( *CI_result_free_t )( CI_Result* result );

} /* extern "C" */
