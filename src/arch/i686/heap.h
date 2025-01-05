#ifndef HAZEL_HEAP_H_
#define HAZEL_HEAP_H_

#include <stdint.h>
#include <stdbool.h>

struct res_hdr {
   uint32_t magic;
   uint32_t size;
};

struct node {
    uint32_t size;
    struct node *next;
};

void heap_init(void);
void *heap_alloc(uint32_t len, bool page_aligned);
void heap_free(void *ptr);

#endif
