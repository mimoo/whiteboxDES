#include "Ltables.c"

const int rr_table[48] = {
    2, 3, 6, 7, 10, 11, 14, 18, 19, 22, 23, 26, 27, 30, 31
}; 

void addbit_64(uint64_t *block, uint64_t from,
int position_from, int position_to)
{
    if(((from << (position_from)) & FIRSTBIT) != 0)
	*block += (FIRSTBIT >> position_to);
}

void addbit_32(uint32_t *block, uint64_t from,
int position_from, int position_to)
{
    if(((from << (position_from)) & FIRSTBIT) != 0)
	*block += (FIRSTBIT >> position_to);
}

void before_rounds(uint64_t *data, uint32_t LL, uint64t XX, uint32t rr)
{
    // L
    LL = 0;
    for(int ii = 0; ii < 32; ii++)
	addbit_32(&LL, *data, ii, ii);
    // X
    XX = 0;
    for(int ii = 0; ii < 48; ii++)
	addbit_64(&XX, *data, (DesExpansion[ii] + 31), ii);
    // r
    rr = 0;
    for(int ii = 0; ii < 16; ii++)
	addbit_32(&rr, *data, (rr_table[ii] + 31), ii);
}

void rounds(uint32_t *LL, uint64_t *XX, uint32_t *rr, int round)
{
    // State 1 -> State 2

    // State 2 -> State 3
}
