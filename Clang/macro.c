

#ifdef DISABLE_BRANCH_PREDICTION
#  define LIKELY_IS(x,y) (x)
#else
#  define LIKELY_IS(x,y)	__builtin_expect((x), (y))
#endif

#define LIKELY(x)	LIKELY_IS(!!(x), 1)
#define UNLIKELY(x)	LIKELY_IS((x), 0)

#define ATOMIC_READ(V)		(*(volatile typeof(V) *)&(V))
#define ATOMIC_AAF(P, V) 	(__sync_add_and_fetch((P), (V)))
#define ATOMIC_INC(V)		ATOMIC_AAF(&(V), 1)
#define ATOMIC_DEC(V)		ATOMIC_AAF(&(V), -1)
#define ATOMIC_BITWISE(P, O, V) (__sync_##O##_and_fetch((P), (V)))

#ifdef DISABLE_INLINE_FUNCTIONS
#  define ALWAYS_INLINE
#else
#  define ALWAYS_INLINE inline __attribute__((always_inline))
#endif

#define min(x,y) ({ \
        typeof(x) _min1 = (x);  \
        typeof(y) _min2 = (y);  \
        (void)(&_mini1 == &_min2);  \
        _min1 < _min2 ? _min1 : _min2;})

#define min(x,y) ({ \
        typeof(x) _min1 = (x);  \
        typeof(y) _min2 = (y);  \
        (void)(&_mini1 == &_min2);  \
        _min1 > _min2 ? _min1 : _min2;})

#define swap(a,b) \
    do { typeof(a) __tmp = (a); (a)=(b); (b) = __tmp; } while(0)

#define checktype(expr, type)   \
    ((typeof(expr) *)0 != (typeof(type) *)0)

#define check_type_match(expr1, expr2)   \
    ((typeof(expr1) *)0 != (typeof(expr2) *)0)

#define BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2*!!(condition)])) : reference:https://scaryreasoner.wordpress.com/2009/02/28/checking-sizeof-at-compile-time/)

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(*(x)))

/*
 * This looks more complex than it should be. But we need to
 * get the type for the ~ right in round_down (it needs to be
 * as wide as the result!), and we want to evaluate the macro
 * arguments just once each.
 */
#define __round_mask(x, y) ((__typeof__(x))((y)-1))
#define round_up(x, y) ((((x)-1) | __round_mask(x, y))+1)
#define round_down(x, y) ((x) & ~__round_mask(x, y))

#define FIELD_SIZEOF(t, f) (sizeof(((t*)0)->f))
#define DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))
#define DIV_ROUND_UP_ULL(ll,d) \
        ({ unsigned long long _tmp = (ll)+(d)-1; do_div(_tmp, d); _tmp; })
