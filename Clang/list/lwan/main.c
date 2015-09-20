#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "list.h"

struct parent {
	const char *name;
	struct list_head children;
	unsigned int num_children;
};


struct child {
	const char *name;
	// Linked list of all us children.
	struct list_node list;
};

static void add_child(struct parent *parent, char *name) {
    struct child *child = malloc(sizeof(*child));
    child->name = name;
    list_add(&parent->children, &child->list);
    parent->num_children++;
}

static void add_child_tail(struct parent *parent, char *name) {
    struct child *child = malloc(sizeof(*child));
    child->name = name;
    list_add_tail(&parent->children, &child->list);
    parent->num_children++;
}

static void dump_parent(struct parent *p)
{
	struct child *c;

	printf("%s (%u children):\n", p->name, p->num_children);
	list_check(&p->children, "bad child list");
	list_for_each(&p->children, c, list)
		printf(" -> %s\n", c->name);
}

static void dump_child(const struct child *c)
{
	list_check_node(&c->list, "bad child list");
	printf("%s\n", c->name);
}

int main(int argc, char **argv)
{
    struct parent *parent = malloc(sizeof(*parent));
    memset(parent, 0, sizeof(*parent));
    parent->name = "aoo";
    list_head_init(&parent->children);
    parent->num_children = 0;

    add_child(parent, "boo1");
    add_child(parent, "boo2");
    add_child(parent, "boo3");
    add_child_tail(parent, "boo1");
    add_child_tail(parent, "boo2");
    add_child_tail(parent, "boo3");
    dump_parent(parent);

}
