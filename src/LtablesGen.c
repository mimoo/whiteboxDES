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
	        //    char M1[96][64];
            //    char M1_permutation[96][64];
	        //  char M1_expansion[96][96];

	        // M1[ii][jj] = M1_expansion[ii][0 -> 96] *
	        //              M1_permutation[0 -> 96][jj]
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
    for(int ii = 0; ii < 4; ii++)
    {
        res+=Vec[ii];
        if(ii != 3)
            res = res << 1;
    }
    return res;
}

void st2_to_st3(unsigned int LUT2[288][256])
{
    unsigned int M2[96][96];
    unsigned int intMat[4][8];
    unsigned int Vec[8];
    unsigned int Vec2[4];
    unsigned int res;
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
                res=0;
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
                res=vec_to_int(Vec2);
                LUT2[cmp][kk]=res;
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
}

int main(/*int argc, char ** argv*/)
{
    uint64_t key = 8554016168460312; // A RECUPERER DANS LES PARAMETRES NORMALEMENT
    unsigned non_linear_tboxes[16][8][256]; 
    unsigned linear_tboxes[16][4][256];
    unsigned int LUT2[288][256];
    
    // Take KEY as argument

    // Make subkeys (use DES algorithm)

    // Make Lookuptables in Ltables.c
    st1_to_st2(key, non_linear_tboxes, linear_tboxes);
    st2_to_st3(LUT2);
    return EXIT_SUCCESS;

    // Open the output file
    output = fopen("tboxes.c", "w");
    if(output == NULL)
    {
        fprintf(stderr, "Error: don't have permission to write output file");
        exit(EXIT_FAILURE);
    }
    
    // Write the header of the file
    fprintf(output, "//////////////////////////////////////////////////////\n"
                    "//                  LOOK-UP TABLES                 //\n"
                    "////////////////////////////////////////////////////\n\n"
                    "// State 1 -> State 2 tables\n");
    
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
           
    // Close the output file            
    fclose(output);

    return EXIT_SUCCESS;
}
