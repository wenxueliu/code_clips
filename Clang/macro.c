// 更多参考 include/linux/kernel.h

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

/*
 * 1. (e): Compute expression e.
 * 2. !!(e): Logically negate twice: 0 if e == 0; otherwise 1.
 * 3. -!!(e): Numerically negate the expression from step 2: 0 if it was 0; otherwise -1.
 * 4. struct{int: -!!(0);} --> struct{int: 0;}: If it was zero, then we declare a struct
 *    with an anonymous integer bitfield that has width zero. Everything is fine and we
 *    proceed as normal.
 * 5. struct{int: -!!(1);} --> struct{int: -1;}: On the other hand, if it isn't zero,
 *    then it will be some negative number. Declaring any bitfield with negative width
 *    is a compilation error.
 */
#define BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2*!!(condition)])) : reference:https://scaryreasoner.wordpress.com/2009/02/28/checking-sizeof-at-compile-time/)

http://stackoverflow.com/questions/9229601/what-is-in-c-code?rq=1
#define BUILD_BUG_ON(condition) ((void)BUILD_BUG_ON_ZERO(condition))
#define MAYBE_BUILD_BUG_ON(cond) ((void)sizeof(char[1 - 2 * !!(cond)]))
#define BUILD_BUG_ON_ZERO(e) (sizeof(struct { int:-!!(e); }))
#define BUILD_BUG_ON_NULL(e) ((void *)sizeof(struct { int:-!!(e); }))


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

#define rounddown(x, y) (				\
{							\
	typeof(x) __x = (x);				\
	__x - (__x % (y));				\
}							\
)

#define FIELD_SIZEOF(t, f) (sizeof(((t*)0)->f))
#define DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))
#define DIV_ROUND_UP_ULL(ll,d) \
        ({ unsigned long long _tmp = (ll)+(d)-1; do_div(_tmp, d); _tmp; })


/* Casts 'pointer' to 'type' and issues a compiler warning if the cast changes
 * anything other than an outermost "const" or "volatile" qualifier.
 *
 * The cast to int is present only to suppress an "expression using sizeof
 * bool" warning from "sparse" (see
 * http://permalink.gmane.org/gmane.comp.parsers.sparse/2967). */
#define CONST_CAST(TYPE, POINTER)                               \
    ((void) sizeof ((int) ((POINTER) == (TYPE) (POINTER))),     \
     (TYPE) (POINTER))

struct recirc_id_node {
    int field1;
};

void recirc_id_node_try_ref_rcu(const struct recirc_id_node *n_) {
    struct recirc_id_node *node = CONST_CAST(struct recirc_id_node *, n_);
}
