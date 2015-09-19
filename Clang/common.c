#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define     MAXARGC     50
#define     WHITE       " \t\n"
int buf_args(char *buf, int(*optfunc)(int, char**))
{
    char    *ptr,   *argv[MAXARGC];
    int     argc;

    if (strtok(buf, WHITE) == NULL)
    {
        return -1;
    }
    argv[argc = 0] = buf;
    while((ptr = strtok(NULL, WHITE)) != NULL)
    {
        if (++argc >= MAXARGC - 1)
        {
            return -1;
        }
        argv[argc] = ptr;
    }
    argv[++argc] = NULL;
    return ((*optfunc)(argc, argv));
}




void *xmalloc(size_t size)
{
    void *p = malloc(size ? size : 1);
    //COVERAGE_INC(util_xalloc);
    if (p == NULL) {
        //out_of_memory();
        abort();
    }
    return p;
}

char *xvasprintf(const char *format, va_list args)
{
    va_list args2;
    size_t needed;
    char *s;

    va_copy(args2, args);
    needed = vsnprintf(NULL, 0, format, args);

    s = xmalloc(needed + 1);

    vsnprintf(s, needed + 1, format, args2);
    va_end(args2);

    return s;
}

char *xasprintf(const char *format, ...)
{
    va_list args;
    char *s;

    va_start(args, format);
    s = xvasprintf(format, args);
    va_end(args);

    return s;
}
