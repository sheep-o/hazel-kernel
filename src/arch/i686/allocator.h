#ifndef HAZEL_ALLOCATOR_H_
#define HAZEL_ALLOCATOR_H_

#include <stdint.h>
#include <stdbool.h>

/*
 * Initialize the page frame allocator
 */
void alloc_init(void);

/*
 * Returns a pointer to the byte in the bitmap where the bit corresponding to page_num would be stored
 */
uint8_t *alloc_get_byte(uint32_t page_num);

/*
 * Reserves the specified page number
 */
void alloc_res_page(uint32_t page_num);

/*
 * Frees the specified page number
 */
void alloc_free_page(uint32_t page_num);

/*
 * Reserves the first free page found and returns its physical address
 */
void *alloc_first_page(void);

/*
 * Returns true if the given page number is free, otherwise false
 */
bool alloc_is_free(uint32_t page_num);

/*
 * Allocates num_pages consecutive pages and returns a pointer to the first page's physical address
 */
void *alloc_consec(uint32_t num_pages);

/*
 * Allocates enough pages to hold the specified number of bytes and returns a pointer to the first page's physical address
 */
void *alloc_enough(uint32_t size);

#endif
