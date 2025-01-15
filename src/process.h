#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>

struct process {
    uint32_t ip;
    uint32_t sp;
    uint32_t page_directory;
};

#endif //PROCESS_H
