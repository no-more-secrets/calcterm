template<typename T>
struct ScopeExit {
    ScopeExit(T f) : f(f) {}
    ~ScopeExit() { f(); }
    ScopeExit(ScopeExit const&)  = delete;
    ScopeExit(ScopeExit const&&) = delete;
    ScopeExit const& operator= (ScopeExit const&)  = delete;
    ScopeExit const& operator= (ScopeExit const&&) = delete;
    T f;
};

#define STRING_JOIN2(arg1, arg2) arg1 ## arg2
#define STRING_JOIN(arg1, arg2) STRING_JOIN2(arg1, arg2)
#define SCOPE_EXIT(code)                                        \
    auto STRING_JOIN(scope_exit_f_, __LINE__) = [&](){ code; }; \
    ScopeExit<decltype(STRING_JOIN(scope_exit_f_, __LINE__))>   \
        STRING_JOIN(scope_exit_, __LINE__)(                     \
            STRING_JOIN(scope_exit_f_, __LINE__)                \
        );
