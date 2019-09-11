/**
 *  \file expression.h
 *  Includes the Expression class
 *
 **/

#ifndef SYMENGINE_EXPRESSION_H
#define SYMENGINE_EXPRESSION_H

#include <symengine/add.h>
#include <symengine/pow.h>
#include <symengine/symbol.h>
#include <symengine/complex_double.h>
#include <symengine/eval_double.h>
#include <symengine/printers.h>

namespace SymEngine
{

// Forward declare these here to avoid including derivative.h and break the
// cycle
RCP<const Basic> diff(const RCP<const Basic> &arg, const RCP<const Symbol> &x,
                      bool cache);

RCP<const Basic> sdiff(const RCP<const Basic> &arg, const RCP<const Basic> &x,
                       bool cache);

class Expression
{
private:
    RCP<const Basic> m_basic;

public:
    //! Plain constructor of Expression
    Expression() : m_basic(integer(0))
    {
    }
    //! Construct Expression from integral types
    template <class T>
    Expression(
        T n,
        typename std::enable_if<std::is_integral<T>::value>::type * = nullptr)
        : m_basic(integer(n))
    {
    }
    //! Construct Expression from floating point types
    template <class T>
    Expression(T n,
               typename std::enable_if<std::is_floating_point<T>::value>::type
                   * = nullptr)
        : m_basic(real_double(n))
    {
    }
    //! Construct Expression from std::complex<> types
    template <class T>
    Expression(std::complex<T> n,
               typename std::enable_if<std::is_floating_point<T>::value>::type
                   * = nullptr)
        : m_basic(complex_double(n))
    {
    }

    Expression(const integer_class &n) : m_basic(integer(n))
    {
    }

    Expression(const rational_class &n) : m_basic(Rational::from_mpq(n))
    {
    }

    Expression(RCP<const Basic> &&o) : m_basic(o)
    {
    }

    Expression(const RCP<const Basic> &o) : m_basic(o)
    {
    }

    Expression(const std::string &s);

    //! Construct Expression from Expression
    Expression(const Expression &) = default;
    //! Construct Expression from reference to Expression
    Expression(Expression &&other) SYMENGINE_NOEXCEPT
        : m_basic(std::move(other.m_basic))
    {
    }
    //! Overload assignment operator
    Expression &operator=(const Expression &) = default;
    //! Overload assignment operator for reference
    Expression &operator=(Expression &&other) SYMENGINE_NOEXCEPT
    {
        if (this != &other) {
            this->m_basic = std::move(other.m_basic);
        }
        return *this;
    }
    //! Destructor of Expression
    virtual ~Expression() SYMENGINE_NOEXCEPT
    {
    }
    //! Overload stream operator
    friend std::ostream &operator<<(std::ostream &os, const Expression &expr)
    {
        os << expr.m_basic->__str__();
        return os;
    }
    //! Overload addition
    friend Expression operator+(const Expression &a, const Expression &b)
    {
        return Expression(add(a.m_basic, b.m_basic));
    }
    friend Expression operator+(const RCP<const Basic> &a, const Expression &b)
    {
        return Expression(add(a, b.m_basic));
    }
    friend Expression operator+(const Expression &a, const RCP<const Basic> &b)
    {
        return Expression(add(a.m_basic, b));
    }
    //! Overload addition and assignment(+=)
    Expression &operator+=(const Expression &other)
    {
        m_basic = add(m_basic, other.m_basic);
        return *this;
    }
    Expression &operator+=(const RCP<const Basic> &other)
    {
        m_basic = add(m_basic, other);
        return *this;
    }
    //! Overload subtraction
    friend Expression operator-(const Expression &a, const Expression &b)
    {
        return Expression(sub(a.m_basic, b.m_basic));
    }
    friend Expression operator-(const RCP<const Basic> &a, const Expression &b)
    {
        return Expression(sub(a, b.m_basic));
    }
    friend Expression operator-(const Expression &a, const RCP<const Basic> &b)
    {
        return Expression(sub(a.m_basic, b));
    }
    operator const RCP<const Basic> &() const
    {
        return m_basic;
    }
    //! Differentiation
    Expression diff(const RCP<const Symbol> &x, bool cache = true) const
    {
        return Expression(SymEngine::diff(m_basic, x, cache));
    }
    //! Differentiation
    Expression diff(const RCP<const Basic> &x, bool cache = true) const
    {
        return Expression(sdiff(m_basic, x, cache));
    }
    //! Substitution
    Expression subs(const map_basic_basic &subs_map) const
    {
        return Expression(m_basic->subs(subs_map));
    }
    //! Evaluation to a floating point type
    template <typename T,
              typename
              = typename std::enable_if<std::is_arithmetic<T>::value>::type>
    explicit operator T() const
    {
        return T(eval_double(*get_basic()));
    }
    //! Evaluation to a complex floating point type
    template <typename T,
              typename
              = typename std::enable_if<std::is_arithmetic<T>::value>::type>
    explicit operator std::complex<T>() const
    {
        return std::complex<T>(eval_complex_double(*get_basic()));
    }
    operator const Basic &() const
    {
        return *m_basic;
    }
    //! Overload unary negative
    Expression operator-() const
    {
        Expression retval(*this);
        retval *= -1;
        return retval;
    }
    //! Overload subtraction and assignment(-=)
    Expression &operator-=(const Expression &other)
    {
        m_basic = sub(m_basic, other.m_basic);
        return *this;
    }
    Expression &operator-=(const RCP<const Basic> &other)
    {
        m_basic = sub(m_basic, other);
        return *this;
    }
    //! Overload multiplication
    friend Expression operator*(const Expression &a, const Expression &b)
    {
        return Expression(mul(a.m_basic, b.m_basic));
    }
    friend Expression operator*(const RCP<const Basic> &a, const Expression &b)
    {
        return Expression(mul(a, b.m_basic));
    }
    friend Expression operator*(const Expression &a, const RCP<const Basic> &b)
    {
        return Expression(mul(a.m_basic, b));
    }
    //! Overload multiplication and assignment (*=)
    Expression &operator*=(const Expression &other)
    {
        m_basic = mul(m_basic, other.m_basic);
        return *this;
    }
    Expression &operator*=(const RCP<const Basic> &other)
    {
        m_basic = mul(m_basic, other);
        return *this;
    }
    //! Overload Division
    friend Expression operator/(const Expression &a, const Expression &b)
    {
        return Expression(div(a.m_basic, b.m_basic));
    }
    friend Expression operator/(const RCP<const Basic> &a, const Expression &b)
    {
        return Expression(div(a, b.m_basic));
    }
    friend Expression operator/(const Expression &a, const RCP<const Basic> &b)
    {
        return Expression(div(a.m_basic, b));
    }
    //! Overload Division and assignment (/=)
    Expression &operator/=(const Expression &other)
    {
        m_basic = div(m_basic, other.m_basic);
        return *this;
    }
    Expression &operator/=(const RCP<const Basic> &other)
    {
        m_basic = div(m_basic, other);
        return *this;
    }
    //! Overload check equality (==)
    bool operator==(const Expression &other) const
    {
        return eq(*m_basic, *other.m_basic);
    }
    bool operator==(const RCP<const Basic> &other) const
    {
        return eq(*m_basic, *other);
    }

    //! Overload check not equal (!=)
    bool operator!=(const Expression &other) const
    {
        return not(*this == other);
    }
    bool operator!=(const RCP<const Basic> &other) const
    {
        return not(*this == other);
    }

    //! Method to get Basic from Expression
    const RCP<const Basic> &get_basic() const
    {
        return m_basic;
    }
  /* TGN adds the following */

    //! Overload Sin
    friend Expression sin(const Expression &a)
    {
      return Expression(sin(a.m_basic));
    }
    //! Overload Cos
    friend Expression cos(const Expression &a)
    {
      return Expression(cos(a.m_basic));
    }
    //! Overload Tan
    friend Expression tan(const Expression &a)
    {
      return Expression(tan(a.m_basic));
    }
    //! Overload Asin
    friend Expression asin(const Expression &a)
    {
      return Expression(asin(a.m_basic));
    }
    //! Overload Acos
    friend Expression acos(const Expression &a)
    {
      return Expression(acos(a.m_basic));
    }
    //! Overload Atan
    friend Expression atan(const Expression &a)
    {
      return Expression(atan(a.m_basic));
    }
    //! Overload Atan2
    friend Expression atan2(const Expression &a, const Expression &b)
    {
      return Expression(atan2(a.m_basic, b.m_basic));
    }
    //! Overload Abs
    friend Expression abs(const Expression &a)
    {
      return Expression(abs(a.m_basic));
    }
    //! Overload Sqrt
    friend Expression sqrt(const Expression &a)
    {
      return Expression(sqrt(a.m_basic));
    }
};

//! Our hash:
struct ExpressionHash {
    //! Returns the hashed value.
    size_t operator()(const Expression &k_ex) const
    {
      const RCP<const Basic> k = k_ex.get_basic();
      return k->hash();
    }
};  

//! Our comparison `(==)`
struct ExpressionKeyEq {
    //! Comparison Operator `==`
    bool operator()(const Expression &x_ex, const Expression &y_ex) const
    {
      const RCP<const Basic> x = x_ex.get_basic(), y = y_ex.get_basic();
      return eq(*x, *y);
    }
};

//! Our less operator `(<)`:
struct ExpressionKeyLess {
    //! true if `x < y`, false otherwise
    bool operator()(const Expression &x_ex, const Expression &y_ex) const
    {
      const RCP<const Basic> x = x_ex.get_basic(), y = y_ex.get_basic();      
      hash_t xh = x->hash(), yh = y->hash();
        if (xh != yh)
            return xh < yh;
        if (eq(*x, *y))
            return false;
        return x->__cmp__(*y) == -1;
    }
};  

inline Expression pow(const Expression &base, const Expression &exp)
{
    return pow(base.get_basic(), exp.get_basic());
}

inline Expression expand(const Expression &arg)
{
    return expand(arg.get_basic());
}

inline bool unified_eq(const Expression &a, const Expression &b)
{
    return a == b;
}

inline int unified_compare(const Expression &a, const Expression &b)
{
    return unified_compare(a.get_basic(), b.get_basic());
}

// Utility functions for piranha

namespace detail
{
// This function must have external linkage
std::string poly_print(const Expression &x);
}

} // namespace SymEngine

#ifdef HAVE_SYMENGINE_PIRANHA

#include <piranha/math.hpp>
#include <piranha/pow.hpp>
#include <piranha/print_coefficient.hpp>
namespace piranha
{
namespace math
{

template <typename T>
struct partial_impl<T, typename std::
                           enable_if<std::is_same<T, SymEngine::Expression>::
                                         value>::type> {
    /// Call operator.
    /**
     * @return an instance of Expression constructed from zero.
     */
    SymEngine::Expression operator()(const SymEngine::Expression &,
                                     const std::string &) const
    {
        return SymEngine::Expression(0);
    }
};

template <typename T, typename U>
struct pow_impl<T, U,
                typename std::
                    enable_if<std::is_same<T, SymEngine::Expression>::value
                              && std::is_integral<U>::value>::type> {
    SymEngine::Expression operator()(const SymEngine::Expression &x,
                                     const U &y) const
    {
        return SymEngine::pow(SymEngine::Expression(x).get_basic(),
                              SymEngine::integer(y));
    }
};
} // namespace piranha::math

template <typename U>
struct print_coefficient_impl<U, typename std::
                                     enable_if<std::is_same<U, SymEngine::
                                                                   Expression>::
                                                   value>::type> {
    auto operator()(std::ostream &os, const U &cf) const -> decltype(os << cf)
    {
        return os << SymEngine::detail::poly_print(cf);
    }
};
} // namespace piranha
#endif // HAVE_SYMENGINE_PIRANHA

// Utility functions for xeus-cling
namespace SymEngine
{

#ifdef __CLING__
// clang-format off
#if defined(__has_include) && __has_include(<nlohmann/json.hpp>)
// clang-format on
#include <nlohmann/json.hpp>

inline nlohmann::json mime_bundle_repr(const Expression &i)
{
    auto bundle = nlohmann::json::object();
    bundle["text/plain"] = str(i);
    bundle["text/latex"] = "$" + latex(i) + "$";
    return bundle;
}
#endif
#endif

} // namespace SymEngine

#endif // SYMENGINE_EXPRESSION_H
