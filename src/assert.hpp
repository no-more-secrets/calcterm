#include <stdexcept>

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define ASSERT( a )                                           \
    if( !(a) )                                                \
        throw std::logic_error(                               \
            "assertion:" __FILE__ ":" TOSTRING(__LINE__) "\n" \
            #a                                                \
        );
