#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "DES.h"

//////////////////////////////////////////////////////
//                 GLOBAL VARIABLES                //
////////////////////////////////////////////////////

static FILE * output = NULL;

//////////////////////////////////////////////////////
//                 FUNCTIONS                       //
////////////////////////////////////////////////////

// Generate Lookuptables between state 1 and state 2
void st1_to_st2(uint64_t key, char non_linear_tboxes[16][8][256], 
           char linear_tboxes[16][4][256])
{
    uint64_t a_key[16];
    a_key[0] = key;
    uint64_t next_key;
    char temp_byte;
    char minibit;
    
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

int main(/*int argc, char ** argv*/)
{
    uint64_t key = 8554016168460312; // A RECUPERER DANS LES PARAMETRES NORMALEMENT
    char non_linear_tboxes[16][8][256]; 
    char linear_tboxes[16][4][256];
    
    // Take KEY as argument

    // Make subkeys (use DES algorithm)

    // Make Lookuptables in Ltables.c
    st1_to_st2(key, non_linear_tboxes, linear_tboxes);
    
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
          fprintf(output, "%d, ", non_linear_tboxes[ii][jj][kk]);
        }
        fprintf(output, "},\n");
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
          fprintf(output, "%d, ", non_linear_tboxes[ii][jj][kk]);
        }
        fprintf(output, "},\n");
      }
      fprintf(output, "    },\n\n");
    }
    fprintf(output, "};\n\n");  
           
    // Close the output file            
    fclose(output);

    return EXIT_SUCCESS;
}
