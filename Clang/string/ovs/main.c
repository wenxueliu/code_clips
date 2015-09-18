
#include "dynamic-string.h"

int main(int argc, char **argv)
{
    struct ds s;

    long long int ago = 5;
    long long int interval = 5;

    int n_flow_mods = 5;
    ds_init(&s);
    ds_put_format(&s, "%d flow_mods ", n_flow_mods);
    if (interval == ago) {
        ds_put_format(&s, "in the last %lld s", ago);
    } else if (interval) {
        ds_put_format(&s, "in the %lld s starting %lld s ago",
                        interval, ago);
    } else {
        ds_put_format(&s, "%lld s ago", ago);
    }

    ds_put_cstr(&s, " (");
    s.length -= 2;
    ds_put_char(&s, ')');
    printf("string:%s\n", ds_cstr(&s));
    ds_destroy(&s);
}
