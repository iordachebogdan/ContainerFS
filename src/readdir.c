#include "fzip.h"
#include "dir_tree.h"
#include "utility.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_PATH 256

struct DfsData {
    struct DirTree* node;
    int length;
};

struct DfsData* create_data(struct DirTree* node, int length) {
    struct DfsData* data = malloc(sizeof(struct DfsData));
    *data = (struct DfsData){.node = node, .length = length};
    return data;
}

int fzip_readdir(const char* path, void* buf, fuse_fill_dir_t filler,
                 off_t offset, struct fuse_file_info* fi,
                 enum fuse_readdir_flags flags) {
    (void)offset; (void)fi; (void)flags;

    struct DirTree* node;
    if (*path == 0 || (node = find(get_data()->tree, path)) == NULL) {
        return -ENOENT;
    }

    filler(buf, ".", NULL, 0, 0);
    filler(buf, "..", NULL, 0, 0);

    char buff[MAX_PATH];
    int path_len = strlen(path);
    memcpy(buff, path, path_len);

    struct List* head = NULL;
    push_front(&head, create_data(node->son, path_len));
    while (head != NULL) {
        struct DfsData* state = head->data;
        pop_front(&head);

        if (state->node == NULL) {
            buff[state->length] = 0;
            filler(buf, buff, NULL, 0, 0);
        } else {
            buff[state->length] = node->ch;
            push_front(&head, create_data(node->son, state->length + 1));
            if (node->next != NULL) {
                push_front(&head, create_data(node->next, state->length));
            }
        }

        free(state);
    }

    return EXIT_SUCCESS;
}
