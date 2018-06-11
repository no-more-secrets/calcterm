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

using st_t = std::stack<NumElement>;

auto parse_long( char* l ) -> long;

auto make_decimal( char* l ) -> double;

template<typename T>
auto toNE( T d ) -> NumElement;

auto  noop        (  st_t&  st  )  ->  void;
auto  plus        (  st_t&  st  )  ->  void;
auto  minus       (  st_t&  st  )  ->  void;
auto  times       (  st_t&  st  )  ->  void;
auto  div         (  st_t&  st  )  ->  void;
auto  neg         (  st_t&  st  )  ->  void;
auto  pow_        (  st_t&  st  )  ->  void;
auto  log_        (  st_t&  st  )  ->  void;
auto  int_        (  st_t&  st  )  ->  void;
auto  reale       (  st_t&  st  )  ->  void;

auto  pfintdot    (  st_t&  st,  char*  _1  )  ->  void;
auto  pfint       (  st_t&  st,  char*  _1  )  ->  void;
auto  pfdotint    (  st_t&  st,  char*  _2  )  ->  void;
auto  inegint     (  st_t&  st,  char*  _2  )  ->  void;
auto  iplusint    (  st_t&  st,  char*  _2  )  ->  void;
auto  iint        (  st_t&  st,  char*  _1  )  ->  void;

auto  pfintdotint (  st_t&  st,  char*  _1, char* _3 )  ->  void;
