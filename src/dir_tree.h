#ifndef _DIR_TREE_H_
#define _DIR_TREE_H_

struct DirTree {
    struct DirTree* son, *next;
    int size;
    char ch;
};

int create_tree(const char*, struct DirTree**);
int destroy_tree(struct DirTree**);

#endif