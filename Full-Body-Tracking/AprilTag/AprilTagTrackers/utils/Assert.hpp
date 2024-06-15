#pragma once

#include "Cross.hpp"
#include "Log.hpp" // IWYU pragma: keep

#ifdef ATT_TESTING
#    include "Test.hpp"
#endif

// the strings are not comptime combined to prevent duplicates in binary
/// log an assertion failure, the separate LogValues for __VA_ARGS__ allows for empty message
#define ATT_DETAIL_ASSERT_LOG(p_expr, ...)                                                   \
    (::utils::LogPrelude(::utils::LogTag::Assert, __FILE__, __LINE__),                       \
     ::utils::LogValues(__VA_ARGS__),                                                        \
     ::utils::LogValues("\n", ATT_PRETTY_FUNCTION, ": Failing assertion ( ", #p_expr, " )"), \
     ::utils::LogEnd())

#ifdef ATT_TESTING
#    define ATT_DETAIL_ASSERT_ABORT() [] {                     \
        DOCTEST_ADD_FAIL_AT(__FILE__, __LINE__, "ATT_ASSERT"); \
    }()
#else
#    define ATT_DETAIL_ASSERT_ABORT() ATT_ABORT()
#endif

/// assert an expression is true.
/// useful for cheaper checks or non-performance critical contexts
/// not a replacement for error handling, only for checking programmer/logic errors
#define ATT_REQUIRE(p_expr, ...)                             \
    do                                                       \
    {                                                        \
        const bool exprResult = static_cast<bool>((p_expr)); \
        if (!exprResult)                                     \
        {                                                    \
            ATT_DETAIL_ASSERT_LOG(p_expr, __VA_ARGS__);      \
            ATT_DETAIL_ASSERT_ABORT();                       \
        }                                                    \
    } while (false)

#ifdef ATT_DEBUG
/// assert an expression is true. If ATT_DEBUG isn't defined, becomes noop
/// use anywhere that the assertion would affect performance in release builds
#    define ATT_ASSERT(p_expr, ...) ATT_REQUIRE(p_expr, __VA_ARGS__)
#else
#    define ATT_ASSERT(p_expr, ...) ATT_NOOP()
#endif

namespace utils
{

/// allow `static_assert(false)` in `if constexpr`
/// do not specialize
template <typename...>
inline constexpr bool alwaysFalse = false;

/// force `if else constexpr` chain exhaustive by causing a compile error in the else branch
template <typename... Ts>
consteval void Exhaustive()
{
    static_assert(alwaysFalse<Ts...>, "if else constexpr chain was not exhaustive for type");
}

} // namespace utils
