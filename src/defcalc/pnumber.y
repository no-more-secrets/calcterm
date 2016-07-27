%{

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

#include "phelper.hpp"

typedef struct yy_buffer_state* YY_BUFFER_STATE;

YY_BUFFER_STATE ct_num__scan_string( char const* str );
void            ct_num__delete_buffer( YY_BUFFER_STATE buffer );
void            yyerror( std::stack<NumElement>& st, char const* s );
int             ct_num_lex();

%}

%parse-param { std::stack<NumElement>& st }

%name-prefix="ct_num_"

%union { char* integer; }

%token POW COM
%token L_P R_P
%left  PLUS NEG
%left  TIMES DIV
%left  NG
%token DOT
%left  E
%token IMG
%token <integer> INT

%%

/*=================================================================*/
/*Grammar
/*=================================================================*/
document : expr
/*=================================================================*/
expr     : POW L_P expr COM expr R_P { pow_       ( st         ); }
         | L_P expr R_P              { noop       ( st         ); }
         | expr PLUS     expr        { plus       ( st         ); }
         | expr NEG expr             { minus      ( st         ); }
         | expr TIMES    expr        { times      ( st         ); }
         | expr DIV      expr        { div        ( st         ); }
         | NEG expr %prec NG         { neg        ( st         ); }
         | number                    { noop       ( st         ); }
/*=================================================================*/
number   : real IMG                  { noop       ( st         ); }
         | real                      { noop       ( st         ); }
real     : posfloat                  { noop       ( st         ); }
         | posfloat E integer        { reale      ( st         ); }
posfloat : INT DOT INT               { pfintdotint( st, $1, $3 ); }
         | INT DOT                   { pfintdot   ( st, $1     ); }
         | INT                       { pfint      ( st, $1     ); }
         | DOT INT                   { pfdotint   ( st, $2     ); }
integer  : NEG INT                   { inegint    ( st, $2     ); }
         | PLUS INT                  { iplusint   ( st, $2     ); }
         | INT                       { iint       ( st, $1     ); }
/*=================================================================*/

%%

bool parse_number( char const* input, double& output ) {
    YY_BUFFER_STATE bs = ct_num__scan_string( input );
    // SCOPE_EXIT( ct_num__delete_buffer( bs ) )
    auto success = false;
    try {
        auto st = std::stack<NumElement>();
        ct_num_parse( st );
        if( st.size() != 1 )
            throw std::range_error( "st.size() != 1" );
        if( st.top().type != NumElement::ElemType::DOUBLE )
            throw std::range_error( "st.top().type != DOUBLE" );
        output = st.top()._double;
        success = true;
    }
    catch( std::logic_error& e ) {
        (void)e;
    }
    ct_num__delete_buffer( bs );
    return success;
}

void yyerror( std::stack<NumElement>& st, char const* s ) {
    /* who calls this function, flex or bison? */
	throw std::logic_error("bison parse error: " + std::string( s ));
}
