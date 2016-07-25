#include <string>
#include <stack>

struct NumElement {
    enum class ElemType { LONG, DOUBLE };
    ElemType type;
    union {
        long   _long;
        double _double;
    };
};

auto parse_long( char* l ) -> long;

auto make_decimal( char* l ) -> double;

template<typename T>
auto toNE( T d ) -> NumElement;
