#ifndef HAZEL_ALLOCATOR_H_
#define HAZEL_ALLOCATOR_H_

#include <stdint.h>
#include <stdbool.h>

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

bool alloc_is_free(uint32_t page_num);

void *alloc_consec(uint32_t num_pages);

/*
 *
 */
void *alloc_enough(uint32_t size);

#endif
