#include "dir_tree.h"

#include <stdlib.h>

#include "zip.h"

struct DirTree* create_node(struct DirTree* son, struct DirTree* next,
                            int size, char ch) {
    struct DirTree* node = malloc(sizeof(struct DirTree));
    node->son = son;
    node->next = next;
    node->size = size;
    node->ch = ch;
    return node;
}

void insert(struct DirTree** root, const char* file) {
    if (*root == NULL) {
        *root = create_node(NULL, NULL, 0, -1);
    }

    struct DirTree* node = *root;
    for (int i = 0; file[i]; ++i) {
        ++node->size;
        struct DirTree* son = node->son;
        while (son != NULL && son->ch != file[i]) {
            son = son->next;
        }

        if (son == NULL) {
            son = create_node(NULL, node->son, 0, file[i]);
            node->son = son;
        }
        node = son;
    }
    ++node->size;
}

int create_tree(const char* path, struct DirTree** root_) {
    *root_ = NULL;

    struct zip* zip_file;
    if (!(zip_file = zip_open(path, ZIP_CREATE, 0))) {
        return EXIT_FAILURE;
    }

    zip_int64_t num_entries = zip_get_num_entries(zip_file, 0);
    for (zip_int64_t i = 0; i < num_entries; ++i) {
        insert(root_, zip_get_name(zip_file, i, ZIP_FL_ENC_RAW));
    }

    return EXIT_SUCCESS;
}

int destroy_tree(struct DirTree* root) {
    if (root == NULL) {
        return EXIT_SUCCESS;
    }

    struct List {
        struct DirTree* data;
        struct List* next;
    };

    struct List* head = malloc(sizeof(struct List));
    *head = (struct List){.data = root, .next = NULL};

    while (head != NULL) {
        struct DirTree* node = head->data;
        struct List* old_head = head;
        head = head->next;

        free(old_head);

        struct DirTree* chk[2] = {node->son, node->next};
        for (int t = 0; t < 2; ++t) {
            if (chk[t] != NULL) {
                struct List* new_head = malloc(sizeof(struct List));
                *new_head = (struct List){.data = chk[t], .next = head};
                head = new_head;
            }
        }
        free(node);
    }
    return EXIT_SUCCESS;
}

struct DirTree* find(struct DirTree* root, const char* path) {
    int i = 0;
    while (root && path[i]) {
        struct DirTree* it = root->son;
        while (it != NULL && it->ch != path[i]) {
            it = it->next;
        }
        root = it;
        ++i;
    }
    return root;
}