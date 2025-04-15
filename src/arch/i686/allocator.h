#ifndef HAZEL_ALLOCATOR_H_
#define HAZEL_ALLOCATOR_H_

#include <stdint.h>

/*
 * Initialize the page frame allocator
 */
void alloc_init(void);

uint8_t *alloc_get_byte(uint32_t page_num);

/*
 * Reserves the specified page
 */
void alloc_res_page(uint32_t page_num);

/*
 * Frees the specified page
 */
void alloc_free_page(uint32_t page_num);

/*
 * Reserves the first free page found
 */
void *alloc_first_page(void);

#endif
