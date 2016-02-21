#include <stdio.h>
#include "stdgc.h"

#ifdef malloc
#undef malloc
#endif
#ifdef free
#undef free
#endif

#define MANUAL_GC = 1

#ifdef MANUAL_GC
static int is_auto_mode = 1;
#else
static int is_auto_mode = 0;
#endif

typedef struct MemoryNode {
    struct MemoryNode *prev;
    void *ptr;
    struct MemoryNode *next;
} MemoryNode;

static MemoryNode *head = NULL;
static MemoryNode *tail = NULL;

void free_on_exit() {
    MemoryNode *iterator;

    if (head == NULL)
        return;
    else
        iterator = head;

    for (iterator; iterator->next != NULL; iterator = head) {
        free(iterator->ptr);
        head = iterator->next;
        iterator->next->prev = NULL;
        free(iterator);
    }
    if (iterator != NULL) {
        free(iterator->ptr);
        head = NULL;
        tail = NULL;
        free(iterator);
    }
}

void init_mallocgc() {
    if (is_auto_mode == 0)
        atexit(free_on_exit);
}

void *mallocgc(size_t size) {
    MemoryNode *new;
    void *ptr;

    ptr = malloc(size);
    if (ptr == NULL) {
        return NULL;
    }

    new = (MemoryNode *) malloc(sizeof(MemoryNode));
    if (new == NULL) {
        abort();
    }

    new->ptr = ptr;

    if (head == NULL) {
        init_mallocgc();
        head = new;
        tail = new;
        new->next = NULL;
        new->prev = NULL;
    } else {
        head->prev = new;
        new->next = head;
        head = new;
        new->prev = NULL;
    }

    return new->ptr;
}

void freegc(void *ptr) {
    MemoryNode *iterator = NULL;
    int found = 1;

    if (ptr == NULL) {
        return;
    }
    if (head == NULL) {
        abort();
    }

    if (head->ptr == ptr || tail->ptr == ptr) {
        if (head->ptr == ptr)
            iterator = head;
        else
            iterator = tail;
    } else {
        for (iterator = head; iterator->next != NULL; iterator = iterator->next)
            if (iterator->ptr == ptr) {
                found = 0;
                break;
            }
    }

    if (found == 0 && iterator != ptr) {
        free(ptr);
        return;
    }

    if (head == iterator && tail == iterator) {
        free(iterator->ptr);
        free(iterator);
        head = NULL;
        tail = NULL;
    } else if (head == iterator) {
        free(iterator->ptr);
        head = iterator->next;
        iterator->next->prev = NULL;
        free(iterator);
    } else if (tail == iterator) {
        free(iterator->ptr);
        tail = iterator->prev;
        iterator->prev->next = NULL;
        free(iterator);
    } else {
        free(iterator->ptr);
        iterator->prev->next = iterator->next;
        iterator->next->prev = iterator->prev;
        free(iterator);
    }
}