/* This is the flex file for scanning real or imaginary numbers
 * in "scientific notation." */
%{

#include <string>
#include <stdexcept>
#include <stack>
#include "phelper.hpp"
#include "pnumber.y.hpp"

#define YY_NO_UNPUT

%}

/* This is so that we generate a default version of yywrap
 * function and therefore don't need to link with -lfl */
%option noyywrap
/* Don't generate this because it doesn't appear to be used
 * and so then triggers a warning. */
%option nounput

/* Use the "calcterm number" namespace */
%option prefix="ct_num_"

%%

"("     { return L_P;   }
")"     { return R_P;   }
"+"     { return PLUS;  }
"*"     { return TIMES; }
"/"     { return DIV;   }
"-"     { return NEG;   }
"."     { return DOT;   }
"e"     { return E;     }
"i"     { return IMG;   }
"pow"   { return POW;   }
"log"   { return LOG;   }
","     { return COM;   }
[0-9]+  { ct_num_lval.integer = strdup( yytext );
          return INT;
        }
[ ]+
.|\n    { throw std::logic_error( "cannot tokenize: " + std::string( yytext ) ); }

%%
