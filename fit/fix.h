/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    fix.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_FIX_H
#define FIT_GUARD_FUNCTION_FIX_H

/// fix
/// ===
/// 
/// Description
/// -----------
/// 
/// The `fix` function adaptor implements a fixed-point combinator. This can be
/// used to write recursive functions.
/// 
/// Synopsis
/// --------
/// 
///     template<class F>
///     fix_adaptor<F> fix(F f);
/// 
/// Example
/// -------
/// 
///     int r = fit::fix([](auto s, auto x) -> decltype(x) { return x == 0 ? 1 : x * s(x-1); })(5);
///     assert(r, 5*4*3*2*1);
/// 

#include <fit/always.h>
#include <fit/returns.h>
#include <fit/enable_if_convertible.h>

#ifndef FIT_FIX_HAS_CONSTEXPR
#define FIT_FIX_HAS_CONSTEXPR 0
#endif

#if FIT_FIX_HAS_CONSTEXPR
#define FIT_FIX_CONSTEXPR constexpr
#else
#define FIT_FIX_CONSTEXPR
#endif


namespace fit {

namespace detail{

template<class Derived, class F>
struct fix_adaptor_base : F
{
    FIT_FIX_CONSTEXPR fix_adaptor_base()
    {}

    template<class X, FIT_ENABLE_IF_CONVERTIBLE(X, F)>
    FIT_FIX_CONSTEXPR fix_adaptor_base(X x) : F(x)
    {}

    template<class... Ts>
    FIT_FIX_CONSTEXPR const F& base_function(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    template<class... Ts>
    FIT_FIX_CONSTEXPR const Derived& derived_function(Ts&&... xs) const
    {
        return static_cast<const Derived&>(always_ref(*this)(xs...));
    }

    template<class... Ts>
    FIT_FIX_CONSTEXPR auto operator()(Ts&&... xs) const FIT_RETURNS
    (
        this->base_function(xs...)(this->derived_function(xs...), std::forward<Ts>(xs)...)
    );
};
}

template<class F>
struct fix_adaptor : detail::fix_adaptor_base<fix_adaptor<F>, F>
{
    typedef detail::fix_adaptor_base<fix_adaptor<F>, F> base;
    FIT_FIX_CONSTEXPR fix_adaptor()
    {}

    template<class X, FIT_ENABLE_IF_CONVERTIBLE(X, base)>
    FIT_FIX_CONSTEXPR fix_adaptor(X x) : base(x)
    {}
};

template<class F>
FIT_FIX_CONSTEXPR fix_adaptor<F> fix(F f)
{
    return fix_adaptor<F>(f);
}
}

#endif