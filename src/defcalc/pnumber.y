%{

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <cmath>

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

%token L_PAREN R_PAREN
%left  PLUS NEGATIVE
%left  TIMES DIV
%left  NEG
%token DOT
%left  E
%token IMG
%token <integer> INT

%%

document  : expr
expr      : L_PAREN expr R_PAREN
          | expr PLUS expr          { auto d1 = st.top()._double; st.pop();
                                      auto d2 = st.top()._double; st.pop();
                                      st.push( toNE( d1 + d2 ) ); }
          | expr NEGATIVE expr      { auto d1 = st.top()._double; st.pop();
                                      auto d2 = st.top()._double; st.pop();
                                      st.push( toNE( d2 - d1 ) ); }
          | expr TIMES expr         { auto d1 = st.top()._double; st.pop();
                                      auto d2 = st.top()._double; st.pop();
                                      st.push( toNE( d2 * d1 ) ); }
          | expr DIV expr           { auto d1 = st.top()._double; st.pop();
                                      auto d2 = st.top()._double; st.pop();
                                      st.push( toNE( d2 / d1 ) ); }
          | NEGATIVE expr %prec NEG { auto d = st.top()._double; st.pop();
                                      st.push( toNE( -d ) ); }
          | number
number    : real IMG           { /* need to fix this */ }
          | real
real      : float
          | float E integer    { auto l = st.top()._long;   st.pop();
                                 auto d = st.top()._double; st.pop();
                                 st.push( toNE( d * pow( 10.0, (double)l ) ) ); }
float     : NEGATIVE pos_float { auto ne = st.top();
                                 st.pop();
                                 ne._double = -ne._double;
                                 st.push( ne ); }
          | pos_float
pos_float : INT DOT INT        { auto l1 = parse_long( $1 );
                                 auto decimal = make_decimal( $3 );
                                 st.push( toNE( (double)l1 + decimal ) ); }
          | INT DOT            { auto l = parse_long( $1 );   st.push( toNE( (double)l ) ); }
          | INT                { auto l = parse_long( $1 );   st.push( toNE( (double)l ) ); }
          | DOT INT            { auto d = make_decimal( $2 ); st.push( toNE( d ) );         }
integer   : NEGATIVE INT       { auto l = parse_long( $2 );   st.push( toNE( -l ) );        }
          | INT                { auto l = parse_long( $1 );   st.push( toNE( l ) );         }

%%

bool parse_number( char const* input, double& output ) {
    auto st = std::stack<NumElement>();
    YY_BUFFER_STATE bs = ct_num__scan_string( input );
    // SCOPE_EXIT( ct_num__delete_buffer( bs ) )
    auto success = false;
    try {
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
