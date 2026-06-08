#pragma once
// Numbers.h — Type-safe numeric wrapper with common_type arithmetic
// Binary operators return Number<common_type_t<N1,N2>>, so mixing types never
// silently narrows either operand.  Storing a wider result into a narrower
// Number triggers convert_to and throws Bad_value if the value doesn't fit.
// Requires C++23.

#include <type_traits>
#include <concepts>
#include <compare>
#include <limits>
#include <iosfwd>
#include <numbers>

// ══════════════════════════════════════════════════════════════════════════════
// Concepts
// ══════════════════════════════════════════════════════════════════════════════

/// Matches any built-in integral or floating-point type.
template<typename T>
concept Num = std::integral<T> || std::floating_point<T>;

/// True when a value of type U may not be representable as type T.
template<typename T, typename U>
concept can_narrow =
    ( sizeof(T) < sizeof(U))
    || ( std::integral<T>   &&  std::floating_point<U>)
    || ((sizeof(T) == sizeof(U))
        && (   ( std::floating_point<T> &&  std::integral<U>)
            || (std::signed_integral<T>  != std::signed_integral<U>)));

// ══════════════════════════════════════════════════════════════════════════════
// Common-type alias
// ══════════════════════════════════════════════════════════════════════════════

/// The type that both N1 and N2 promote to under the usual arithmetic rules.
template<Num N1, Num N2>
using Common = std::common_type_t<N1, N2>;

// ══════════════════════════════════════════════════════════════════════════════
// Narrowing detection & safe conversion
// ══════════════════════════════════════════════════════════════════════════════

template<Num T, Num U>
constexpr bool will_narrow(U u) noexcept
{
    if constexpr (!can_narrow<T, U>) { return false; }

    // Unsigned T cannot hold negative values
    if constexpr (std::unsigned_integral<T> && std::signed_integral<U>)
        if (u < 0) return true;

    // Signed T cannot hold unsigned values above its max
    if constexpr (std::signed_integral<T> && std::unsigned_integral<U>)
        if (u > static_cast<U>(std::numeric_limits<T>::max())) return true;

    // General roundtrip: covers size differences and float↔int mismatches
    T t = static_cast<T>(u);
    return static_cast<U>(t) != u;
}

struct Bad_value {};

template<Num T, Num U>
constexpr T convert_to(U u)
{
    if (will_narrow<T>(u)) throw Bad_value{};
    return static_cast<T>(u);
}

// ══════════════════════════════════════════════════════════════════════════════
// Number<T>
// ══════════════════════════════════════════════════════════════════════════════
// The class itself holds only: construction/assignment, unary operators,
// increment/decrement, and value access.  All binary operators are defined
// as free functions below using Common<N1,N2> as the result type.

template<Num T>
class Number
{
    T val{};

    /// Bypass checked construction — used internally by unary ops whose
    /// result is already known to be of type T.
    static constexpr Number raw(T v) noexcept { Number n{}; n.val = v; return n; }

public:
    // ── Construction ──────────────────────────────────────────────────────────

    constexpr Number() noexcept = default;
    constexpr Number(const Number&) noexcept = default;
    constexpr Number(Number&&)      noexcept = default;

    /// From any raw Num type — throws Bad_value if u would narrow to T.
    template<Num U>
    constexpr Number(U u) : val(convert_to<T>(u)) {}

    /// From another Number<U> — throws Bad_value if the value wouldn't fit.
    template<Num U>
    constexpr Number(const Number<U>& o) : val(convert_to<T>(o.value())) {}

    // ── Assignment ────────────────────────────────────────────────────────────

    constexpr Number& operator=(const Number&) noexcept = default;
    constexpr Number& operator=(Number&&)      noexcept = default;

    template<Num U>
    constexpr Number& operator=(U u) { val = convert_to<T>(u); return *this; }

    template<Num U>
    constexpr Number& operator=(const Number<U>& o)
    { val = convert_to<T>(o.value()); return *this; }

    // ── Value access ──────────────────────────────────────────────────────────

    /// Implicit conversion — allows Number<T> to be used wherever T is expected.
    constexpr operator T() const noexcept { return val; }

    /// Explicit named accessor used by the free-function operators below.
    constexpr T value() const noexcept { return val; }

    // ── Unary operators ───────────────────────────────────────────────────────
    // These stay as members: they are inherently single-type operations.

    [[nodiscard]] constexpr Number operator+() const noexcept { return *this; }

    // Negation: wraps for unsigned, identical to built-in.
    [[nodiscard]] constexpr Number operator-() const noexcept
    { return raw(static_cast<T>(-val)); }

    [[nodiscard]] constexpr bool operator!() const noexcept { return !val; }

    // Bitwise NOT — integers only; bool excluded (deprecated since C++14).
    [[nodiscard]] constexpr Number operator~() const noexcept
        requires std::integral<T> && (!std::same_as<T, bool>)
    { return raw(static_cast<T>(~val)); }

    // ── Increment / Decrement ─────────────────────────────────────────────────

    constexpr Number& operator++() noexcept    { ++val; return *this; }
    constexpr Number  operator++(int) noexcept { Number t = *this; ++val; return t; }
    constexpr Number& operator--() noexcept    { --val; return *this; }
    constexpr Number  operator--(int) noexcept { Number t = *this; --val; return t; }

    // ── Stream output ─────────────────────────────────────────────────────────

    template<typename Ch, typename Tr>
    friend std::basic_ostream<Ch, Tr>&
    operator<<(std::basic_ostream<Ch, Tr>& os, const Number& n)
    { return os << n.val; }
};

// ══════════════════════════════════════════════════════════════════════════════
// Class Template Argument Deduction (CTAD) guides
//
// Without these, Number<T> cannot be deduced because T (the class parameter)
// and U (the constructor parameter) are independent — the compiler has no way
// to know T should equal U.  These guides make all of the following work:
//
//   Number x = 42;               →  Number<int>
//   Number y = 2U;               →  Number<unsigned int>
//   Number z = 3.14f;            →  Number<float>
//   Number a = x + y;            →  Number<common_type_t<int,unsigned>>
//   Number b = std::sqrt(x);     →  Number<double>
//   Number pi = std::numbers::pi;→  Number<double>  ← the tricky case
//
// WHY forwarding reference (U&&) instead of by-value (U):
//   std::numbers::pi is 'inline constexpr double', so it is a const-qualified
//   lvalue.  A by-value guide deduces U = double on standards-compliant
//   compilers (const is stripped), but MSVC can preserve the const and deduce
//   U = const double.  Num<const double> then fails at instantiation because
//   std::floating_point<const double> → std::is_floating_point_v<const double>
//   = true, but the specialisation Number<const double> is ill-formed in
//   practice.  Using U&& + remove_cvref_t<U> is unambiguous on all compilers:
//   const double& → stripped → double → Number<double>.
// ══════════════════════════════════════════════════════════════════════════════

/// Deduce Number<T> from any raw Num value — lvalue or rvalue, const or not.
/// Handles plain literals (42, 3.14f), named variables, and constexpr
/// constants like std::numbers::pi.
/// The Num constraint uses remove_cvref_t so it is never tripped by const/ref.
template<typename U> requires Num<std::remove_cvref_t<U>>
Number(U&&) -> Number<std::remove_cvref_t<U>>;

/// Deduce Number<T> when copy-constructing from another Number<U>.
template<Num U>
Number(const Number<U>&) -> Number<U>;

/// Deduce Number<T> when move-constructing from an rvalue Number<U>.
template<Num U>
Number(Number<U>&&) -> Number<U>;

// ══════════════════════════════════════════════════════════════════════════════
// Free-function binary operators
//
// Each operator has three overload groups:
//   (a) Number<N1>  op  Number<N2>  →  Number<Common<N1,N2>>   [the core form]
//   (b) Number<N1>  op  N2          →  delegates to (a)
//   (c) N1          op  Number<N2>  →  delegates to (a)
//
// The (b)/(c) scalar forms construct a Number<N> from the raw scalar first,
// which performs the same narrowing check as any other construction — so
// e.g. Number<int8_t>{} + (-300) throws before the addition even happens.
//
// Compound-assignment operators delegate to the binary operator and then
// assign the result back to x — so if the computed Common-type result would
// narrow into x's type, that assignment also throws Bad_value.
//
// Comparisons use Common to avoid the classic signed/unsigned pitfalls and
// give the same result as the equivalent built-in expression.
// ══════════════════════════════════════════════════════════════════════════════

// ── Arithmetic ───────────────────────────────────────────────────────────────

template<Num N1, Num N2>
[[nodiscard]] constexpr auto operator+(Number<N1> x, Number<N2> y)
{
    using C = Common<N1, N2>;
    return Number<C>(static_cast<C>(x.value()) + static_cast<C>(y.value()));
}
template<Num N1, Num N2>
[[nodiscard]] constexpr auto operator-(Number<N1> x, Number<N2> y)
{
    using C = Common<N1, N2>;
    return Number<C>(static_cast<C>(x.value()) - static_cast<C>(y.value()));
}
template<Num N1, Num N2>
[[nodiscard]] constexpr auto operator*(Number<N1> x, Number<N2> y)
{
    using C = Common<N1, N2>;
    return Number<C>(static_cast<C>(x.value()) * static_cast<C>(y.value()));
}
template<Num N1, Num N2>
[[nodiscard]] constexpr auto operator/(Number<N1> x, Number<N2> y)
{
    using C = Common<N1, N2>;
    return Number<C>(static_cast<C>(x.value()) / static_cast<C>(y.value()));
}
template<Num N1, Num N2>
[[nodiscard]] constexpr auto operator%(Number<N1> x, Number<N2> y)
    requires std::integral<N1> && std::integral<N2>
{
    using C = Common<N1, N2>;
    return Number<C>(static_cast<C>(x.value()) % static_cast<C>(y.value()));
}

// Scalar RHS: Number op Num
template<Num N1, Num N2> [[nodiscard]] constexpr auto operator+(Number<N1> x, N2 y) { return x + Number<N2>(y); }
template<Num N1, Num N2> [[nodiscard]] constexpr auto operator-(Number<N1> x, N2 y) { return x - Number<N2>(y); }
template<Num N1, Num N2> [[nodiscard]] constexpr auto operator*(Number<N1> x, N2 y) { return x * Number<N2>(y); }
template<Num N1, Num N2> [[nodiscard]] constexpr auto operator/(Number<N1> x, N2 y) { return x / Number<N2>(y); }
template<Num N1, Num N2> [[nodiscard]] constexpr auto operator%(Number<N1> x, N2 y)
    requires std::integral<N1> && std::integral<N2>
{ return x % Number<N2>(y); }

// Scalar LHS: Num op Number
template<Num N1, Num N2> [[nodiscard]] constexpr auto operator+(N1 x, Number<N2> y) { return Number<N1>(x) + y; }
template<Num N1, Num N2> [[nodiscard]] constexpr auto operator-(N1 x, Number<N2> y) { return Number<N1>(x) - y; }
template<Num N1, Num N2> [[nodiscard]] constexpr auto operator*(N1 x, Number<N2> y) { return Number<N1>(x) * y; }
template<Num N1, Num N2> [[nodiscard]] constexpr auto operator/(N1 x, Number<N2> y) { return Number<N1>(x) / y; }
template<Num N1, Num N2> [[nodiscard]] constexpr auto operator%(N1 x, Number<N2> y)
    requires std::integral<N1> && std::integral<N2>
{ return Number<N1>(x) % y; }

// ── Bitwise (integers only) ───────────────────────────────────────────────────

template<Num N1, Num N2>
[[nodiscard]] constexpr auto operator&(Number<N1> x, Number<N2> y)
    requires std::integral<N1> && std::integral<N2>
{
    using C = Common<N1, N2>;
    return Number<C>(static_cast<C>(x.value()) & static_cast<C>(y.value()));
}
template<Num N1, Num N2>
[[nodiscard]] constexpr auto operator|(Number<N1> x, Number<N2> y)
    requires std::integral<N1> && std::integral<N2>
{
    using C = Common<N1, N2>;
    return Number<C>(static_cast<C>(x.value()) | static_cast<C>(y.value()));
}
template<Num N1, Num N2>
[[nodiscard]] constexpr auto operator^(Number<N1> x, Number<N2> y)
    requires std::integral<N1> && std::integral<N2>
{
    using C = Common<N1, N2>;
    return Number<C>(static_cast<C>(x.value()) ^ static_cast<C>(y.value()));
}
template<Num N1, Num N2>
[[nodiscard]] constexpr auto operator<<(Number<N1> x, Number<N2> y)
    requires std::integral<N1> && std::integral<N2>
{
    using C = Common<N1, N2>;
    return Number<C>(static_cast<C>(x.value()) << static_cast<C>(y.value()));
}
template<Num N1, Num N2>
[[nodiscard]] constexpr auto operator>>(Number<N1> x, Number<N2> y)
    requires std::integral<N1> && std::integral<N2>
{
    using C = Common<N1, N2>;
    return Number<C>(static_cast<C>(x.value()) >> static_cast<C>(y.value()));
}

// Scalar RHS: Number op Num
template<Num N1, Num N2> [[nodiscard]] constexpr auto operator& (Number<N1> x, N2 y) requires std::integral<N1> && std::integral<N2> { return x &  Number<N2>(y); }
template<Num N1, Num N2> [[nodiscard]] constexpr auto operator| (Number<N1> x, N2 y) requires std::integral<N1> && std::integral<N2> { return x |  Number<N2>(y); }
template<Num N1, Num N2> [[nodiscard]] constexpr auto operator^ (Number<N1> x, N2 y) requires std::integral<N1> && std::integral<N2> { return x ^  Number<N2>(y); }
template<Num N1, Num N2> [[nodiscard]] constexpr auto operator<<(Number<N1> x, N2 y) requires std::integral<N1> && std::integral<N2> { return x << Number<N2>(y); }
template<Num N1, Num N2> [[nodiscard]] constexpr auto operator>>(Number<N1> x, N2 y) requires std::integral<N1> && std::integral<N2> { return x >> Number<N2>(y); }

// Scalar LHS: Num op Number
template<Num N1, Num N2> [[nodiscard]] constexpr auto operator& (N1 x, Number<N2> y) requires std::integral<N1> && std::integral<N2> { return Number<N1>(x) &  y; }
template<Num N1, Num N2> [[nodiscard]] constexpr auto operator| (N1 x, Number<N2> y) requires std::integral<N1> && std::integral<N2> { return Number<N1>(x) |  y; }
template<Num N1, Num N2> [[nodiscard]] constexpr auto operator^ (N1 x, Number<N2> y) requires std::integral<N1> && std::integral<N2> { return Number<N1>(x) ^  y; }
template<Num N1, Num N2> [[nodiscard]] constexpr auto operator<<(N1 x, Number<N2> y) requires std::integral<N1> && std::integral<N2> { return Number<N1>(x) << y; }
template<Num N1, Num N2> [[nodiscard]] constexpr auto operator>>(N1 x, Number<N2> y) requires std::integral<N1> && std::integral<N2> { return Number<N1>(x) >> y; }

// ── Compound assignment ───────────────────────────────────────────────────────
// Delegating to `x = x OP y` means the computed Common-type result is
// assigned back through convert_to<N1>, which throws if it no longer fits.

template<Num N1, Num N2> constexpr Number<N1>& operator+=(Number<N1>& x, Number<N2> y) { x = x + y; return x; }
template<Num N1, Num N2> constexpr Number<N1>& operator-=(Number<N1>& x, Number<N2> y) { x = x - y; return x; }
template<Num N1, Num N2> constexpr Number<N1>& operator*=(Number<N1>& x, Number<N2> y) { x = x * y; return x; }
template<Num N1, Num N2> constexpr Number<N1>& operator/=(Number<N1>& x, Number<N2> y) { x = x / y; return x; }
template<Num N1, Num N2> constexpr Number<N1>& operator%=(Number<N1>& x, Number<N2> y)
    requires std::integral<N1> && std::integral<N2>                        { x = x % y; return x; }
template<Num N1, Num N2> constexpr Number<N1>& operator&= (Number<N1>& x, Number<N2> y)
    requires std::integral<N1> && std::integral<N2>                        { x = x &  y; return x; }
template<Num N1, Num N2> constexpr Number<N1>& operator|= (Number<N1>& x, Number<N2> y)
    requires std::integral<N1> && std::integral<N2>                        { x = x |  y; return x; }
template<Num N1, Num N2> constexpr Number<N1>& operator^= (Number<N1>& x, Number<N2> y)
    requires std::integral<N1> && std::integral<N2>                        { x = x ^  y; return x; }
template<Num N1, Num N2> constexpr Number<N1>& operator<<=(Number<N1>& x, Number<N2> y)
    requires std::integral<N1> && std::integral<N2>                        { x = x << y; return x; }
template<Num N1, Num N2> constexpr Number<N1>& operator>>=(Number<N1>& x, Number<N2> y)
    requires std::integral<N1> && std::integral<N2>                        { x = x >> y; return x; }

// Scalar RHS compound assignment — delegates to the Number×Number form above
template<Num N1, Num N2> constexpr Number<N1>& operator+=(Number<N1>& x, N2 y) { return x += Number<N2>(y); }
template<Num N1, Num N2> constexpr Number<N1>& operator-=(Number<N1>& x, N2 y) { return x -= Number<N2>(y); }
template<Num N1, Num N2> constexpr Number<N1>& operator*=(Number<N1>& x, N2 y) { return x *= Number<N2>(y); }
template<Num N1, Num N2> constexpr Number<N1>& operator/=(Number<N1>& x, N2 y) { return x /= Number<N2>(y); }
template<Num N1, Num N2> constexpr Number<N1>& operator%=(Number<N1>& x, N2 y)
    requires std::integral<N1> && std::integral<N2> { return x %= Number<N2>(y); }
template<Num N1, Num N2> constexpr Number<N1>& operator&= (Number<N1>& x, N2 y)
    requires std::integral<N1> && std::integral<N2> { return x &=  Number<N2>(y); }
template<Num N1, Num N2> constexpr Number<N1>& operator|= (Number<N1>& x, N2 y)
    requires std::integral<N1> && std::integral<N2> { return x |=  Number<N2>(y); }
template<Num N1, Num N2> constexpr Number<N1>& operator^= (Number<N1>& x, N2 y)
    requires std::integral<N1> && std::integral<N2> { return x ^=  Number<N2>(y); }
template<Num N1, Num N2> constexpr Number<N1>& operator<<=(Number<N1>& x, N2 y)
    requires std::integral<N1> && std::integral<N2> { return x <<= Number<N2>(y); }
template<Num N1, Num N2> constexpr Number<N1>& operator>>=(Number<N1>& x, N2 y)
    requires std::integral<N1> && std::integral<N2> { return x >>= Number<N2>(y); }

// ── Comparison ────────────────────────────────────────────────────────────────
// Computed in Common<N1,N2> to match built-in arithmetic conversion semantics.
// Defining operator<=> and operator== is sufficient: C++20 synthesises
// !=, <, >, <=, >= from them, and also handles the reversed (N op Number) forms.

template<Num N1, Num N2>
[[nodiscard]] constexpr auto operator<=>(Number<N1> x, Number<N2> y) noexcept
{
    using C = Common<N1, N2>;
    return static_cast<C>(x.value()) <=> static_cast<C>(y.value());
}
template<Num N1, Num N2>
[[nodiscard]] constexpr bool operator==(Number<N1> x, Number<N2> y) noexcept
{
    using C = Common<N1, N2>;
    return static_cast<C>(x.value()) == static_cast<C>(y.value());
}

// Scalar comparisons — Number vs Num
// The reversed forms (Num vs Number) are handled automatically by C++20
// rewriting of operator<=> and operator==.
template<Num N1, Num N2>
[[nodiscard]] constexpr auto operator<=>(Number<N1> x, N2 y) noexcept
{ return x <=> Number<N2>(y); }

template<Num N1, Num N2>
[[nodiscard]] constexpr bool operator==(Number<N1> x, N2 y) noexcept
{ return x == Number<N2>(y); }

// ══════════════════════════════════════════════════════════════════════════════
// Standard library specialisations
// ══════════════════════════════════════════════════════════════════════════════

#include <format>
#include <functional>

template<Num T, typename CharT>
struct std::formatter<Number<T>, CharT> : std::formatter<T, CharT>
{
    template<typename FormatContext>
    auto format(const Number<T>& n, FormatContext& ctx) const
    { return std::formatter<T, CharT>::format(n.value(), ctx); }
};

template<Num T>
struct std::hash<Number<T>>
{
    constexpr std::size_t operator()(const Number<T>& n) const noexcept
    { return std::hash<T>{}(n.value()); }
};

template<Num T>
struct std::numeric_limits<Number<T>> : std::numeric_limits<T> {};

// ══════════════════════════════════════════════════════════════════════════════
// <cmath> integration
//
// Every standard math function is overloaded for Number<T> in two places:
//
//   Global namespace — found by ADL for unqualified calls:
//       sin(Number<double>{1.0})
//
//   namespace std    — found for qualified calls:
//       std::sin(Number<double>{1.0})
//
// Return-type rule (mirrors standard cmath):
//   floating-point T  →  Number<T>          (float→float, double→double, …)
//   integral T        →  Number<double>      (same promotion cmath does)
//
// Functions that inherently return a non-Number type keep that type:
//   isnan/isinf/isfinite/isnormal/signbit  →  bool
//   fpclassify/ilogb                        →  int
//   lround/lrint                            →  long
//   llround/llrint                          →  long long
// ══════════════════════════════════════════════════════════════════════════════

#include <cmath>

/// Result type for a cmath function applied to Number<T>:
/// floating-point types pass through; integrals widen to double.
template<Num T>
using CmathResult = std::conditional_t<std::floating_point<T>, T, double>;

// ── Macros (global namespace) ─────────────────────────────────────────────────
// NUM_UN  — single argument,   returns Number<CmathResult<T>>
// NUM_ABS — single argument,   returns Number<T>  (abs preserves int types)
// NUM_BIN — two arguments,     returns Number<CmathResult<Common<N1,N2>>>
//           generates: Number×Number, Number×Num, Num×Number overloads
// NUM_INT1— single argument,   returns IntRet (bool/int/long/long long)
// NUM_INT2— two int-type args, returns Number<CmathResult<T>>  (ldexp family)

#define NUM_UN(fn) \
    template<Num T> \
    [[nodiscard]] inline auto fn(Number<T> x) noexcept \
    { return Number<CmathResult<T>>(std::fn(static_cast<T>(x))); }

#define NUM_ABS \
    template<Num T> \
    [[nodiscard]] inline auto abs(Number<T> x) noexcept \
    { return Number<T>(std::abs(static_cast<T>(x))); }

#define NUM_BIN(fn) \
    template<Num N1, Num N2> \
    [[nodiscard]] inline auto fn(Number<N1> a, Number<N2> b) noexcept { \
        using C = Common<N1,N2>; using R = CmathResult<C>; \
        return Number<R>(std::fn(static_cast<R>(a.value()), \
                                 static_cast<R>(b.value()))); } \
    template<Num N1, Num N2> \
    [[nodiscard]] inline auto fn(Number<N1> a, N2 b) noexcept \
    { return fn(a, Number<N2>(b)); } \
    template<Num N1, Num N2> \
    [[nodiscard]] inline auto fn(N1 a, Number<N2> b) noexcept \
    { return fn(Number<N1>(a), b); }

#define NUM_BOOL(fn) \
    template<Num T> \
    [[nodiscard]] inline bool fn(Number<T> x) noexcept \
    { return std::fn(static_cast<T>(x)); }

#define NUM_INT(IntRet, fn) \
    template<Num T> \
    [[nodiscard]] inline IntRet fn(Number<T> x) noexcept \
    { return std::fn(static_cast<T>(x)); }

// ── Trigonometric ─────────────────────────────────────────────────────────────
NUM_UN(sin)   NUM_UN(cos)   NUM_UN(tan)
NUM_UN(asin)  NUM_UN(acos)  NUM_UN(atan)  NUM_BIN(atan2)

// ── Hyperbolic ────────────────────────────────────────────────────────────────
NUM_UN(sinh)  NUM_UN(cosh)  NUM_UN(tanh)
NUM_UN(asinh) NUM_UN(acosh) NUM_UN(atanh)

// ── Exponential / logarithmic ─────────────────────────────────────────────────
NUM_UN(exp)   NUM_UN(exp2)  NUM_UN(expm1)
NUM_UN(log)   NUM_UN(log2)  NUM_UN(log10) NUM_UN(log1p) NUM_UN(logb)

// ── Power / root ──────────────────────────────────────────────────────────────
NUM_BIN(pow)  NUM_UN(sqrt)  NUM_UN(cbrt)  NUM_BIN(hypot)

// ── Rounding ──────────────────────────────────────────────────────────────────
NUM_UN(ceil)  NUM_UN(floor) NUM_UN(trunc) NUM_UN(round)
NUM_UN(nearbyint) NUM_UN(rint)

// ── Error / gamma ─────────────────────────────────────────────────────────────
NUM_UN(erf)   NUM_UN(erfc)  NUM_UN(tgamma) NUM_UN(lgamma)

// ── Floating-point utilities ──────────────────────────────────────────────────
NUM_UN(fabs)  NUM_ABS
NUM_BIN(fmod) NUM_BIN(remainder) NUM_BIN(fdim) NUM_BIN(fmax) NUM_BIN(fmin)
NUM_BIN(nextafter) NUM_BIN(copysign)

// ── ldexp / scalbn / scalbln  (integer exponent, not a Num) ──────────────────
template<Num T> [[nodiscard]] inline auto ldexp  (Number<T> x, int  e) noexcept
{ return Number<CmathResult<T>>(std::ldexp  (static_cast<T>(x), e)); }
template<Num T> [[nodiscard]] inline auto scalbn (Number<T> x, int  e) noexcept
{ return Number<CmathResult<T>>(std::scalbn (static_cast<T>(x), e)); }
template<Num T> [[nodiscard]] inline auto scalbln(Number<T> x, long e) noexcept
{ return Number<CmathResult<T>>(std::scalbln(static_cast<T>(x), e)); }

// ── nexttoward  (second arg is always long double) ────────────────────────────
template<Num T> [[nodiscard]] inline auto nexttoward(Number<T> x, long double y) noexcept
{ return Number<CmathResult<T>>(std::nexttoward(static_cast<T>(x), y)); }

// ── fma  (three arguments) ────────────────────────────────────────────────────
template<Num N1, Num N2, Num N3>
[[nodiscard]] inline auto fma(Number<N1> x, Number<N2> y, Number<N3> z) noexcept
{
    using C = Common<Common<N1,N2>, N3>;
    using R = CmathResult<C>;
    return Number<R>(std::fma(static_cast<R>(x.value()),
                              static_cast<R>(y.value()),
                              static_cast<R>(z.value())));
}

// ── hypot  (three-argument form, C++17) ───────────────────────────────────────
template<Num N1, Num N2, Num N3>
[[nodiscard]] inline auto hypot(Number<N1> x, Number<N2> y, Number<N3> z) noexcept
{
    using C = Common<Common<N1,N2>, N3>;
    using R = CmathResult<C>;
    return Number<R>(std::hypot(static_cast<R>(x.value()),
                                static_cast<R>(y.value()),
                                static_cast<R>(z.value())));
}

// ── Classification — return bool ──────────────────────────────────────────────
NUM_BOOL(isnan) NUM_BOOL(isinf) NUM_BOOL(isfinite)
NUM_BOOL(isnormal) NUM_BOOL(signbit)

// ── Classification — return int ───────────────────────────────────────────────
NUM_INT(int, fpclassify) NUM_INT(int, ilogb)

// ── Rounding — return long / long long ────────────────────────────────────────
NUM_INT(long,      lround) NUM_INT(long long, llround)
NUM_INT(long,      lrint)  NUM_INT(long long, llrint)

#undef NUM_UN
#undef NUM_ABS
#undef NUM_BIN
#undef NUM_BOOL
#undef NUM_INT

// ══════════════════════════════════════════════════════════════════════════════
// namespace std — qualified  std::sin(Number<T>{})  forms
// Each overload simply forwards to its global-namespace twin above.
// (Defining overloads for program-defined types in namespace std is
//  permitted by [namespace.std] when the declaration depends on a
//  program-defined type.)
// ══════════════════════════════════════════════════════════════════════════════

namespace std {

// ── Macros (std namespace, forwarding to global) ──────────────────────────────
#define SNUN(fn) \
    template<::Num T> \
    [[nodiscard]] inline auto fn(::Number<T> x) noexcept { return ::fn(x); }

#define SNBIN(fn) \
    template<::Num N1,::Num N2> \
    [[nodiscard]] inline auto fn(::Number<N1> a,::Number<N2> b) noexcept \
    { return ::fn(a,b); } \
    template<::Num N1,::Num N2> \
    [[nodiscard]] inline auto fn(::Number<N1> a, N2 b) noexcept \
    { return ::fn(a,::Number<N2>(b)); } \
    template<::Num N1,::Num N2> \
    [[nodiscard]] inline auto fn(N1 a,::Number<N2> b) noexcept \
    { return ::fn(::Number<N1>(a),b); }

#define SNBOOL(fn) \
    template<::Num T> \
    [[nodiscard]] inline bool fn(::Number<T> x) noexcept { return ::fn(x); }

#define SNINT(IntRet,fn) \
    template<::Num T> \
    [[nodiscard]] inline IntRet fn(::Number<T> x) noexcept { return ::fn(x); }

// Trigonometric
SNUN(sin)   SNUN(cos)   SNUN(tan)
SNUN(asin)  SNUN(acos)  SNUN(atan)  SNBIN(atan2)
// Hyperbolic
SNUN(sinh)  SNUN(cosh)  SNUN(tanh)
SNUN(asinh) SNUN(acosh) SNUN(atanh)
// Exponential / logarithmic
SNUN(exp)   SNUN(exp2)  SNUN(expm1)
SNUN(log)   SNUN(log2)  SNUN(log10) SNUN(log1p) SNUN(logb)
// Power / root
SNBIN(pow)  SNUN(sqrt)  SNUN(cbrt)  SNBIN(hypot)
// Rounding
SNUN(ceil)  SNUN(floor) SNUN(trunc) SNUN(round)
SNUN(nearbyint) SNUN(rint)
// Error / gamma
SNUN(erf)   SNUN(erfc)  SNUN(tgamma) SNUN(lgamma)
// Floating-point utilities
SNUN(fabs)
SNBIN(fmod) SNBIN(remainder) SNBIN(fdim) SNBIN(fmax) SNBIN(fmin)
SNBIN(nextafter) SNBIN(copysign)
// Classification — bool
SNBOOL(isnan) SNBOOL(isinf) SNBOOL(isfinite) SNBOOL(isnormal) SNBOOL(signbit)
// Classification — int
SNINT(int, fpclassify) SNINT(int, ilogb)
// Rounding — long / long long
SNINT(long,      lround) SNINT(long long, llround)
SNINT(long,      lrint)  SNINT(long long, llrint)

// abs: returns Number<T>, not Number<CmathResult<T>>
template<::Num T>
[[nodiscard]] inline auto abs(::Number<T> x) noexcept { return ::abs(x); }

// ldexp / scalbn / scalbln
template<::Num T> [[nodiscard]] inline auto ldexp  (::Number<T> x,int  e) noexcept { return ::ldexp  (x,e); }
template<::Num T> [[nodiscard]] inline auto scalbn (::Number<T> x,int  e) noexcept { return ::scalbn (x,e); }
template<::Num T> [[nodiscard]] inline auto scalbln(::Number<T> x,long e) noexcept { return ::scalbln(x,e); }

// nexttoward
template<::Num T> [[nodiscard]] inline auto nexttoward(::Number<T> x, long double y) noexcept
{ return ::nexttoward(x,y); }

// fma — three args
template<::Num N1,::Num N2,::Num N3>
[[nodiscard]] inline auto fma(::Number<N1> x,::Number<N2> y,::Number<N3> z) noexcept
{ return ::fma(x,y,z); }

// hypot — three args
template<::Num N1,::Num N2,::Num N3>
[[nodiscard]] inline auto hypot(::Number<N1> x,::Number<N2> y,::Number<N3> z) noexcept
{ return ::hypot(x,y,z); }

#undef SNUN
#undef SNBIN
#undef SNBOOL
#undef SNINT

} // namespace std
