#include "Ltables.c"


void rounds(uint64_t *data, int round)
{
    uint64_t left_block = 0;
    uint64_t right_block = 0;
    uint64_t right_block_temp = 0;

    // 1. Block expansion
    for(int ii = 0; ii < 48; ii++)
	addbit(&right_block, *data, (DesExpansion[ii] + 31), ii);

    // 2. 16bits of right_block_temp
    right_block_temp =
}
