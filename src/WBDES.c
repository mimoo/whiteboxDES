#include "WBDES.h"
#include "tboxes.c"

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

// THIS FUNCTION SHOULD BE REPLACED BY A MATRIX M_1
// SEE CHOW ET AL'S PAPER
void before_rounds(uint64_t *data, uint32_t LL, uint64_t XX, uint32_t rr)
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
    //
    // State 1 -> State 2 (Cr)
    //

    char minibit;

    // YY
    // non-linear T-boxes
    uint64_t YY = 0;

    for(int ii = 0; ii < 8; ii ++)
    {
	minibit = 0;
	minibit += ((LL << ii * 2) & FIRSTBIT) + 
	    (((LL << ii * 2 + 1) & FIRSTBIT) >> 1) + 
	    (((XX << ii * 6) & FIRSTBIT) >> 2) +
	    (((XX << ii * 6 + 1) & FIRSTBIT) >> 3) +
	    (((XX << ii * 6 + 2) & FIRSTBIT) >> 4) +
	    (((XX << ii * 6 + 3) & FIRSTBIT) >> 5) +
	    (((XX << ii * 6 + 4) & FIRSTBIT) >> 6) +
	    (((XX << ii * 6 + 5) & FIRSTBIT) >> 7) ;

	YY = lookuptable_state1_YY[round][minibit];
    }

    // rr
    // linear T-boxes
    uint32_t rr_1 = 0;

    for(int ii = 0; ii < 4; ii++)
    {
	minibit = 0;
	minibit += (((LL << ii * 4 + 8) & FIRSTBIT)) +
	    (((LL << ii * 4 + 9) & FIRSTBIT) >> 1) +
	    (((LL << ii * 4 + 10) & FIRSTBIT) >> 2) +
	    (((LL << ii * 4 + 11) & FIRSTBIT) >> 3) +
	    (((rr << ii * 4) & FIRSTBIT) >> 4) +
	    (((rr << ii * 4 + 1) & FIRSTBIT) >> 5) +
	    (((rr << ii * 4 + 2) & FIRSTBIT) >> 6) +
	    (((rr << ii * 4 + 3) & FIRSTBIT) >> 7);

	rr_1 = lookuptable_state1_rr[round][minibit];
    }

    //
    // State 2 -> State 3 (Dr)
    //

    // shuffling

    // look up tables

    // unshuffling

    
}

void end_rounds(uint64_t *data, uint32_t LL, uint64t XX, uint32t rr)
{
    
}
