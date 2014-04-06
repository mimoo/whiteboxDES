#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <math.h>
#include <string.h>

#include "DES.h"

//////////////////////////////////////////////////////
//                 GLOBAL VARIABLES                //
////////////////////////////////////////////////////

static FILE * output = NULL;

//////////////////////////////////////////////////////
//                 FUNCTIONS                       //
////////////////////////////////////////////////////

// Generate Lookuptables between state 1 and state 2
void st1_to_st2(uint64_t key, unsigned  non_linear_tboxes[16][8][256], 
		unsigned linear_tboxes[16][4][256])
{
    uint64_t a_key[16];
    a_key[0] = key;
    uint64_t next_key;
    unsigned char temp_byte;
    unsigned char minibit;
    
    // Get the 16 subkeys
    for(int ii = 0; ii < 16; ii++)
    {
        key_schedule(&a_key[ii], &next_key, ii);
        if(ii != 15)
            a_key[ii + 1] = next_key;
    }
    
    // Non-linear tboxes creation
    for(int ii = 0; ii < 16; ii++)
    {
        for(int jj = 0; jj < 8; jj++)
        {
            for(int kk = 0; kk < 256; kk++)
            {
                temp_byte = kk;
                    
                /* Puting the bits number 0, 1, 2 and 7 respectively at 
                 * the positions 0, 1, 2 and 3
                 */   
                minibit = (temp_byte & 128) + (temp_byte & 64) +
                    (temp_byte & 32) + ((temp_byte << 4) & 18);
                    
                // Xor with the subkey
                temp_byte = temp_byte & 63;
                temp_byte = temp_byte ^ (a_key[ii] >> (6 * (8-(jj+1))));
                
                // Apply the substitution
                int coordx, coordy;

                coordx = ((temp_byte >> 4) & 2) + (temp_byte & 1);
                coordy = (temp_byte >> 1) & 15;
                
                minibit += DesSbox[ii][coordx][coordy];
                
                non_linear_tboxes[ii][jj][kk] = minibit;
            }
        }
    }
        
    // Linear tboxes creation
    for(int ii = 0; ii < 16; ii++)
    {
        for(int jj = 0; jj < 4; jj++)
        {
            for(int kk = 0; kk < 256; kk++)
            {
                temp_byte = kk;
                linear_tboxes[ii][jj][kk] = temp_byte;
            }
        }
    }
}

void Matrix1(unsigned int M1[96][64])
{
    for(int i=0;i<96;i++)
    {
		for(int j=0;j<64;j++)
		{
			M1[i][j]=0;
		}
	}

    for(int i=0;i<64;i++)
    {
	M1[i][PermutationInitial[i]-1]=1;
    }
    int tmp [32][64];
    for(int i=32;i<64;i++)
    {
	for(int j=0;j<64;j++)
	{
	    tmp[i-32][j]=M1[i][j];
	}
    }	
    for(int i=32;i<80;i++)
    {
	for(int j=0;j<64;j++)
	{
	    M1[i][j]=tmp[DesExpansion[i-32]-1][j];
	}
    }
    for(int i=80;i<96;i++)
    {
	for(int j=0;j<64;j++)
	{
	    M1[i][j]=tmp[NotDuplicated[i-80]-1][j];
	}
    }	
}

void Matrix2(unsigned int M2[96][96])
{
    // vars
    int M2_permutation[96][96];
    int M2_expansion[96][96];

    // init zeros
    memset(M2_permutation, 0, sizeof(M2_permutation[96][96]) * 96 * 96);
    memset(M2_expansion, 0, sizeof(M2_expansion[96][96]) * 96 * 96);
    memset(M2, 0, sizeof(M2[96][96]) * 96 * 96);

    // permutation
    for(int ii = 0; ii < 32; ii++)
    {
	M2_permutation[ii + 32][Pbox[ii] + 32 - 1] = 1;
    }

    // xor
    for(int ii = 0; ii < 32; ii++)
    {
	M2_permutation[ii + 32][ii] = 1;
    }

    // R becomes L
    for(int ii = 0; ii < 32; ii++)
    {
	M2_permutation[ii][ii + 64] = 1;
    }

    // expansion identities
    for(int ii = 0; ii < 32; ii++)
    {
	M2_expansion[ii][ii] = 1;
    }

    // expansion
    for(int ii = 32; ii < 80; ii++)
    {
	M2_expansion[ii][DesExpansion[ii - 32] + 32 - 1] = 1;
    }

    // Not Duplicated bits
    for(int ii = 0; ii < 16; ii++)
    {
	M2_expansion[ii + 80][NotDuplicated[ii] + 32 - 1] = 1;
    }
    
    // M2 = M2_expansion*M2_permutation
    int temp = 0;

    for(int ii = 0; ii < 96; ii++)
    {
	for(int jj = 0; jj < 96; jj++)
	{
	    for(int kk = 0; kk < 96; kk++)
	    {
		if(M2_expansion[ii][kk] == 1 && 
		   M2_permutation[kk][jj] == 1)
		    temp ++;
	    }

	    if(temp % 2 == 1){
		M2[ii][jj] = 1;
		temp = 0;
	    }
	}
    }
}

void Matrix3(unsigned int M3[64][96])
{
    for(int ii=0;ii<64;ii++)
    {
        for(int jj=0;jj<96;jj++)
        {
            M3[ii][jj]=0;
        }
    }

    for(int ii=0;ii<32;ii++)
    {
        for(int jj=0;jj<32;jj++)
        {
            if(jj==ii)
                M3[ii][jj]=1;
        }
    }   
    for(int jj=32;jj<80;jj++)
    {
        int cmp=0;
	for(int kk=32;kk<80;kk++)
        {
            if( M3[DesExpansion[jj-32]-1+32][kk] != 0 )
                cmp ++;
        }
        if(cmp == 0)
        {
            M3[DesExpansion[jj-32]-1+32][jj]=1;
        }
    }
    int tmp[64][96];
    for(int ii=0;ii<64;ii++)
    {
        for(int jj=0;jj<96;jj++)
        {
            tmp[ii][jj]=M3[ii][jj];
        }
    }	
    for(int ii=0;ii<64;ii++)
    {
        for(int jj=0;jj<96;jj++)
        {
            M3[ii][jj]=tmp[PermutationFinal[ii]-1][jj];
        }
    }
    for(int ii = 0; ii < 32; ii++)
    {
        for(int jj=0;jj<96;jj++)
        {
            M3[ii][jj]=tmp[ii+32][jj];
            M3[ii+32][jj]=tmp[ii][jj];
        }
    } 
}

void addVec(unsigned int Vec[8])
{
    int i=0;
    while(Vec[i]==1)
    {
        Vec[i]=0;
        i++;
    }
    Vec[i]=1;
}

unsigned int vec_to_int(unsigned int Vec[4])
{
    unsigned int res=0;
    for(int ii = 4; ii >= 0; ii--)
    {
        res+=Vec[ii];
        if(ii != 0)
            res = res << 1;
    }
    return res;
}

/*void st2_to_st3(unsigned int LUT2[288][256])
  {
  unsigned int M2[96][96];
  unsigned int intMat[4][8];
  unsigned int Vec[8];
  unsigned int Vec2[4];
  int cmp=0;
  Matrix2(M2);
  for(int ii = 0; ii < 96; ii+=4)
  {
  for(int jj = 0; jj < 96; jj+=8)
  {
  for(int kk = ii; kk < ii+4; kk++)
  {
  for(int ll = jj; ll < jj+8; ll++)
  {
  intMat[kk-ii][ll-jj]=M2[kk][ll];
  }
  }
  //intMat initialized
  for(int kk = 0; kk < 8; kk++)
  {
  Vec[kk]=0;
  }
  for(int kk = 0; kk < 256; kk++)
  {
  for(int ll = 0; ll < 4; ll++)
  {
  Vec2[ll]=0;
  }
  for(int ll = 0; ll < 4; ll++)
  {
  for(int mm = 0; mm < 8; mm++)
  {
  Vec2[ll]^=intMat[ll][mm]*Vec[mm];
  }
  }
  addVec(Vec);
  LUT2[cmp][kk]=vec_to_int(Vec2);
  }
  cmp++;
  }
  }    
  for(int ii = 0; ii < 288; ii++)
  {
  fprintf(stdout,"{\n");
  for(int jj = 0; jj < 256; jj++)
  {
  fprintf(stdout,"%u ",LUT2[ii][jj]);
  if(jj != 255)
  fprintf(stdout,",");
  }
  fprintf(stdout,"\n}\n");
  }
  }*/

void xor_table(unsigned int xor_Table[256])
{
    unsigned int Vec[8];
    unsigned int Vec2[4];
    for(int ii = 0; ii < 8; ii++)
    {
        Vec[ii]=0;
    }
    for(int ii = 0; ii < 256; ii++)
    {
        for(int jj = 0; jj < 4; jj++)
        {
            Vec2[jj]=Vec[jj]^Vec[jj+4];
        }
        xor_Table[ii]=vec_to_int(Vec2);
        addVec(Vec);
    }
}
void xor_table_david(unsigned int xorTable[256])
{
    for(int byte = 0; byte < 256; byte++)
    {
        xorTable[byte] = (byte >> 4) ^ (byte & 15);
    }
}

int main(int argc, char ** argv)
{
    if(argc != 2)
    {
        fprintf(stdout,"Usage : ./LtablesGen Key\n");
        return EXIT_FAILURE;
    }
    uint64_t key = atoi(argv[1]);
    unsigned non_linear_tboxes[16][8][256]; 
    unsigned linear_tboxes[16][4][256];
    //unsigned int LUT2[288][256];
    unsigned int xor_Table[256];
    
    // Take KEY as argument

    // Make subkeys (use DES algorithm)

    // Make Lookuptables in Ltables.c
    st1_to_st2(key, non_linear_tboxes, linear_tboxes);
    //st2_to_st3(LUT2);
    xor_table(xor_Table);

    // Open the output file
    output = fopen("tboxes.c", "w");
    if(output == NULL)
    {
        fprintf(stderr, "Error: don't have permission to write output file");
        exit(EXIT_FAILURE);
    }
    
    // Write the non-linear tables
    fprintf(output, "const int NonLinearTBoxes[16][8][256] = {\n");          
    for(int ii = 0; ii < 16; ii++)
    {
	fprintf(output, "    {\n");
	for(int jj = 0; jj < 8; jj++)
	{
	    fprintf(output, "    {");
	    for(int kk = 0; kk < 256; kk++)
	    {
		if(non_linear_tboxes[ii][jj][kk] < 10)
		    fprintf(output, "  ");
		else if(non_linear_tboxes[ii][jj][kk] < 100)
		    fprintf(output, " ");
                
		if(kk != 255)
		    fprintf(output, "%d, ", non_linear_tboxes[ii][jj][kk]);
		else
		    fprintf(output, "%d ", non_linear_tboxes[ii][jj][kk]);
          
		if((kk + 1) % 16 == 0)
		    fprintf(output, "\n     ");
	    }
	    fprintf(output, "},\n\n");
	}
	fprintf(output, "    },\n\n");
    }
    fprintf(output, "};\n\n");               

    // Write the linear tables
    fprintf(output, "const int LinearTBoxes[16][4][256] = {\n");
    for(int ii = 0; ii < 16; ii++)
    {
	fprintf(output, "    {\n");
	for(int jj = 0; jj < 4; jj++)
	{
	    fprintf(output, "    {");
	    for(int kk = 0; kk < 256; kk++)
	    {
		if(linear_tboxes[ii][jj][kk] < 10)
		    fprintf(output, "  ");
		else if(linear_tboxes[ii][jj][kk] < 100)
		    fprintf(output, " ");
            
		if(kk != 255)
		    fprintf(output, "%d, ", linear_tboxes[ii][jj][kk]);
		else
		    fprintf(output, "%d ", linear_tboxes[ii][jj][kk]);
          
		if((kk + 1) % 16 == 0)
		    fprintf(output, "\n     ");
	    }
	    fprintf(output, "},\n\n");
	}
	fprintf(output, "    },\n\n");
    }
    fprintf(output, "};\n\n");  
        
    fprintf(output,"const int Xor_Table[256] = {\n     ");
    for(int ii = 0; ii < 256; ii++)
    {
        if(ii != 255)
            fprintf(output, "%u ,", xor_Table[ii]);
        else
            fprintf(output, "%u\n};", xor_Table[ii]);
        if((ii + 1) % 16 == 0)
            fprintf(output, "\n     ");
    }

    //Write M1_LUT
    bool bit_test = 0;
    unsigned int subvector = 0; //4bits!
    unsigned int M1[96][64];
    Matrix1(M1);

    fprintf(output, "\nconst int M1_Ltables[24][8][256] = {\n");

    // lines of submatrices
    for(int ii = 0; ii < 24; ii++)
    {
	fprintf(output, "{\n");
	// cols of submatrices
	for(int jj = 0; jj < 8; jj++)
	{
	    fprintf(output, "{\n");
	    // possible value for the byte
	    for(unsigned int byte = 0; byte < 256; byte++) //8bits!
	    {
		// now let's do the submatrix * subvector
		subvector = 0;
		// lines of submatrix
		for(int byte_i = 0; byte_i < 4; byte_i++)
		{
		    // cols of submatrix
		    for(int byte_j = 0; byte_j < 8; byte_j++)
		    {
			if(M1[ii * 4 + byte_i][jj * 8 + byte_j] == 1 &&
			   (((byte >> (7 - byte_j)) & 1) == 1))
			    bit_test = bit_test == 1 ? 0 : 1;
		    }
		    if(bit_test == 1)
		    {
			subvector += (1 << (3 - byte_i));
			bit_test = 0;
		    }
		}
		fprintf(output, "%i, ", subvector);
	    }
	    fprintf(output, "},\n");
	}
	fprintf(output, "},\n");
    }
    fprintf(output, "};\n");

    //Write M2_LUT
    bit_test = 0;
    subvector = 0; //4bits!
    unsigned int M2[96][96];
    Matrix2(M2);

    fprintf(output, "\nconst int M2_Ltables[24][12][256] = {\n");

    // lines of submatrices
    for(int ii = 0; ii < 24; ii++)
    {
	fprintf(output, "{\n");
	// cols of submatrices
	for(int jj = 0; jj < 12; jj++)
	{
	    fprintf(output, "{\n");
	    // possible value for the byte
	    for(unsigned int byte = 0; byte < 256; byte++) //8bits!
	    {
		// now let's do the submatrix * subvector
		subvector = 0;
		// lines of submatrix
		for(int byte_i = 0; byte_i < 4; byte_i++)
		{
		    // cols of submatrix
		    for(int byte_j = 0; byte_j < 8; byte_j++)
		    {
			if(M2[ii * 4 + byte_i][jj * 8 + byte_j] == 1 &&
			   (((byte >> (7 - byte_j)) & 1) == 1))
			    bit_test = bit_test == 1 ? 0 : 1;
		    }
		    if(bit_test == 1)
		    {
			subvector += (1 << (3 - byte_i));
			bit_test = 0;
		    }
		}
		fprintf(output, "%i, ", subvector);
	    }
	    fprintf(output, "},\n");
	}
	fprintf(output, "},\n");
    }
    fprintf(output, "};\n");

    //Write M3_LUT
    bit_test = 0;
    subvector = 0; //4bits!
    unsigned int M3[64][96];
    Matrix3(M3);

    fprintf(output, "\nconst int M3_Ltables[16][12][256] = {\n");

    // lines of submatrices
    for(int ii = 0; ii < 16; ii++)
    {
	fprintf(output, "{\n");
	// cols of submatrices
	for(int jj = 0; jj < 12; jj++)
	{
	    fprintf(output, "{\n");
	    // possible value for the byte
	    for(unsigned int byte = 0; byte < 256; byte++) //8bits!
	    {
		// now let's do the submatrix * subvector
		subvector = 0;
		// lines of submatrix
		for(int byte_i = 0; byte_i < 4; byte_i++)
		{
		    // cols of submatrix
		    for(int byte_j = 0; byte_j < 8; byte_j++)
		    {
			if(M3[ii * 4 + byte_i][jj * 8 + byte_j] == 1 &&
			   (((byte >> (7 - byte_j)) & 1) == 1))
			    bit_test = bit_test == 1 ? 0 : 1;
		    }
		    if(bit_test == 1)
		    {
			subvector += (1 << (3 - byte_i));
			bit_test = 0;
		    }
		}
		fprintf(output, "%i, ", subvector);
	    }
	    fprintf(output, "},\n");
	}
	fprintf(output, "},\n");
    }
    fprintf(output, "};\n");

    // Close the output file            
    fclose(output); 

    return EXIT_SUCCESS;
}
