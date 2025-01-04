#ifndef USER_H
#define USER_H

#include <stdint.h>

extern void jmp_ring3(uint32_t sp, uint32_t ip); // Defined in start.S
void user_init(void);

#endif //USER_H
