#include "fzip.h"
#include "dir_tree.h"
#include "utility.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_PATH 256

struct DfsData {
    struct DirTree* node; int length;
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

    char buff[MAX_PATH];
    int path_length = strlen(path);
    memcpy(buff, path, path_length);
    if (path_length > 1) {
        buff[path_length++] = '/';
    }
    buff[path_length] = 0;

    printf("[readdir] %s %d\n", buff + 1, strlen(buff + 1));

    struct DirTree* node;
    if (*buff == 0 || (node = find(get_data()->tree, buff + 1)) == NULL) {
        printf("[readdir] Failed\n");
        return -ENOENT;
    }

    filler(buf, ".", NULL, 0, 0);
    filler(buf, "..", NULL, 0, 0);

    struct List* head = NULL;
    push_front(&head, create_data(node->son, path_length));
    while (head != NULL) {
        struct DfsData* state = head->data;
        pop_front(&head);

        node = state->node;
        if (node == NULL) {
            if (path_length < state->length) {
                buff[state->length] = 0;

                struct stat st;
                fzip_getattr(buff, &st, 0);
                filler(buf, buff + path_length, &st, 0, 0);

                printf("Filler call: %s\n", buff + path_length);
            }
        } else {
            if (node->next != NULL) {
                push_front(&head, create_data(node->next, state->length));
            }
            buff[state->length] = node->ch;

            push_front(&head, (node->ch != '/') ? create_data(node->son, state->length + 1)
                                                : create_data(NULL, state->length));
        }

        free(state);
    }

    return 0;
}
