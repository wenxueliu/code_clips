#include <stdio.h>
#include "hash.h"

int main(int argc, char **argv)
{
    struct hash *h = hash_str_new(NULL, NULL);
    hash_add(h, "1", "a");
    hash_add(h, "2", "b");
    hash_add(h, "3", "c");
    hash_add(h, "4", "d");
    hash_add(h, "5", "e");

    printf("hash add total %d\n", hash_get_count(h));
}
