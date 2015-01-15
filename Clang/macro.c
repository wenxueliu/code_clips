

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
