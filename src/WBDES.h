#ifndef WBDES_H
#define WBDES_H

#include <stdint.h>

#include "DES.h"

void addbit_64(uint64_t *block, uint64_t from, 
int position_from, int position_to);

void addbit_32(uint32_t *block, uint64_t from,
int position_from, int position_to);

void before_rounds(uint64_t *data, uint32_t LL, uint64_t XX, uint32_t rr);

void rounds(uint32_t *LL, uint64_t *XX, uint32_t *rr, int round);

#endif
