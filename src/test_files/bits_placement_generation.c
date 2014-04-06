#include <stdio.h>
#include <stdlib.h>

int main()
{
    int M[96][96];
    int M2[96][96];
	FILE * file;
    FILE * file2;
	file = fopen("order_bits","w");
	file2 = fopen("disorder_bits","w");
    for(int ii = 0; ii < 96; ii++)
    {
        for(int jj = 0; jj < 96; jj++)
        {
            M[ii][jj]=0;
        }
    }
    for(int ii = 0; ii < 64; ii++)
    {
        if(ii % 8 == 0 || ii % 8 == 1)
        {
            M[ii][2*(ii/8)+ii%8]=1;
        }
        else if(ii % 8 > 1 && ii %8 < 6)
        {
            M[ii][4*(ii/8)+(ii%8-2)+32]=1;
        }
        else
        {
            M[ii][2*(ii/8)+(ii%8-6)+64]=1;
        }
    }
    for(int ii = 64; ii < 96; ii++)
    {
        if(ii % 8 < 4)
        {
            M[ii][4*(ii/8)+ii%8-16]=1;
        }
        else
        {
            M[ii][4*(ii/8)+(ii%8-4)-16+64]=1;
        }
    }
	if(file!=NULL)
    {
	    for(int ii = 0; ii < 96; ii++)
		{
			for(int jj = 0; jj < 96; jj++)
			{
				fprintf(file,"%d",M[ii][jj]);
			}
			fprintf(file,"\n");
		}
    }
    fclose(file);
    for(int ii = 0; ii < 96; ii++)
    {
        for(int jj = 0; jj < 96; jj++)
        {
            M2[ii][jj]=0;
        }
    }
    for(int ii = 0; ii < 64; ii++)
    {
        if(ii % 8 == 0 || ii % 8 == 1)
        {
            M2[2*(ii/8)+ii%8][ii]=1;
        }
        else if(ii % 8 > 1 && ii %8 < 6)
        {
            M2[4*(ii/8)+(ii%8-2)+32][ii]=1;
        }
        else
        {
            M2[2*(ii/8)+(ii%8-6)+64][ii]=1;
        }
    }
    for(int ii = 64; ii < 96; ii++)
    {
        if(ii % 8 < 4)
        {
            M2[4*(ii/8)+ii%8-16][ii]=1;
        }
        else
        {
            M2[4*(ii/8)+(ii%8-4)-16+64][ii]=1;
        }
    }
	if(file2!=NULL)
    {
	    for(int ii = 0; ii < 96; ii++)
		{
			for(int jj = 0; jj < 96; jj++)
			{
				fprintf(file2,"%d",M2[ii][jj]);
			}
			fprintf(file2,"\n");
		}
    }
    fclose(file2);
    return EXIT_SUCCESS;
}
