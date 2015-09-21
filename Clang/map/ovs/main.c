#include <stdio.h>
#include <string.h>
#include "hmap.h"

struct hmap student = HMAP_INITIALIZER(&student);

struct nd {
    struct hmap_node node;
    const char *key;
};

static struct nd nodes[] = {
    {HMAP_NODE_NULL_INITIALIZER, "1" },
    {HMAP_NODE_NULL_INITIALIZER, "b" },
};

int main(int argc, char **argv)
{
    struct hmap *map = malloc(sizeof(*map));
    hmap_init(map);
    struct hmap_node node2 = {1, NULL};

    hmap_insert_fast(map, &nodes[0].node, strlen(nodes[0].key));
    hmap_insert_fast(map, &nodes[1].node, strlen(nodes[1].key));

    struct nd *q;
    //HMAP_FOR_EACH(q, node, map) {
    //    printf("key= %s",q->key);
    //}
    printf("map size %lu, capacity %lu\n", hmap_count(map), hmap_capacity(map));
    hmap_destroy(map);
}
