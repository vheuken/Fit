#include <fit/lambda.h>
#include <fit/conditional.h>
#include <memory>
#include "test.h"

static constexpr auto add_one = FIT_STATIC_LAMBDA(int x)
{
    return x + 1;
};

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(3 == add_one(2));
}

struct t1 {};
struct t2 {};
struct t3 {};

#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 7

#else

static constexpr auto f = fit::conditional(
    FIT_STATIC_LAMBDA(t1)
    {
        return 1;
    },
    FIT_STATIC_LAMBDA(t2)
    {
        return 2;
    },
    FIT_STATIC_LAMBDA(t3)
    {
        return 3;
    }
);

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(f(t1()) == 1);
    FIT_TEST_CHECK(f(t2()) == 2);
    FIT_TEST_CHECK(f(t3()) == 3);
}

#endif
