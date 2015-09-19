#include <stdio.h>
#include "strbuf.h"

int main(int argc, char **argv)
{
    strbuf_t *str = strbuf_new_with_size(5);
    strbuf_append_char(str, 'l');
    strbuf_append_char(str, 'w');
    strbuf_append_char(str, 'a');
    strbuf_append_char(str, 'n');
    strbuf_append_str(str, " strbuf", (size_t)10);
    printf("strbuf: %s\n", str->value.buffer);
    strbuf_free(str);
}

