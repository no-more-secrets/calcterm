%{

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

// Stuff from flex that bison needs to know about:
typedef struct yy_buffer_state* YY_BUFFER_STATE;
// Functions
YY_BUFFER_STATE ct_num__scan_string( char const* str );
void            ct_num__delete_buffer( YY_BUFFER_STATE buffer );
void            yyerror( char const* s );
int             ct_num_lex();

%}

// calcterm-number namespace
%name-prefix "ct_num_"

%union {
	char* integer;
}

%token NEGATIVE
%token DOT
%token E
%token IMG
%token <integer> INT

%%

document:  numbers             //{ std::cout << "finished." << std::endl; }
/*numbers: real IMG            //{ std::cout << "bison found real IMG" << std::endl; }
    |      real                //{ std::cout << "bison found real" << std::endl; }*/
numbers:   real                //{ std::cout << "bison found real IMG" << std::endl; }
real:      float               //{ std::cout << "bison found float" << std::endl; }
    |      float E integer     //{ std::cout << "bison found float E integer" << std::endl; }
float:     NEGATIVE pos_float  //{ std::cout << "bison found NEGATIVE pos_float" << std::endl; }
    |      pos_float           //{ std::cout << "bison found pos_float" << std::endl; }
pos_float: INT DOT INT         //{ std::cout << "bison found INT DOT INT" << std::endl; }
    |      INT DOT             //{ std::cout << "bison found INT DOT" << std::endl; }
    |      INT                 //{ std::cout << "bison found INT" << std::endl; }
    |      DOT INT             //{ std::cout << "bison found DOT INT" << std::endl; }
integer:   NEGATIVE INT        //{ std::cout << "bison found NEGATIVE INT" << std::endl; }
    |      INT                 //{ std::cout << "bison found INT" << std::endl; }
	;

%%

bool parse_number( char const* input, double& output ) {
    YY_BUFFER_STATE bs = ct_num__scan_string( input );
    // SCOPE_EXIT( ct_num__delete_buffer( bs ) )
    bool success = false;
    try {
        ct_num_parse();
        output = 1.0;
        success = true;
    }
    catch( std::logic_error& e ) {
        (void)e;
    }
    ct_num__delete_buffer( bs );
    return success;
}

void yyerror(char const* s) {
	throw std::logic_error("bison parse error: " + std::string( s ));
}
