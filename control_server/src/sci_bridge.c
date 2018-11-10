/*
 * sci_bridge.c
 *
 *  Created on: 2018/11/07
 *      Author: kazuki
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h> /* getenv */
#include "api_scilab.h"  /* Provide functions to access to the memory of Scilab */
#include "call_scilab.h" /* Provide functions to call Scilab engine */

int init_sci(char* scriptPath)
{
#ifdef _MSC_VER
	if ( StartScilab(NULL,NULL,NULL) == FALSE )
#else
	if ( StartScilab(getenv("SCI"),NULL,NULL) == FALSE )
#endif
	{
		fprintf(stderr,"Error while calling StartScilab\n");
		return -1;
	}
	if(access(scriptPath,R_OK))
	{
		return -2;
	}

	return 0;
}

int deinit_sci()
{
	
    if ( TerminateScilab(NULL) == FALSE )
    {
        fprintf(stderr, "Error while calling TerminateScilab\n");
        return -2;
    }
    return 0;

}

int read_example()
{
	/******************************** READ ****************************/

    /* Load the previously set variable A */
    {
        int rowA_ = 0, colA_ = 0, lp = 0;
        int i = 0, j = 0;
        double *matrixOfDouble = NULL;

        char variableToBeRetrieved[] = "A";
        SciErr sciErr;

        /* First, retrieve the size of the matrix */
        sciErr = readNamedMatrixOfDouble(pvApiCtx, variableToBeRetrieved, &rowA_, &colA_, NULL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
        }

        /* Alloc the memory */
        matrixOfDouble = (double*)malloc((rowA_ * colA_) * sizeof(double));

        /* Load the matrix */
        sciErr = readNamedMatrixOfDouble(pvApiCtx, variableToBeRetrieved, &rowA_, &colA_, matrixOfDouble);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
        }

        printf("\n");
        printf("Display from A (size: %d, %d):\n", rowA_, colA_);
        for (i = 0; i < rowA_ * colA_; i++)
        {
            fprintf(stdout, "A[%d] = %5.2f\n", i, matrixOfDouble[i]);
        }

        if (matrixOfDouble)
        {
            free(matrixOfDouble);
            matrixOfDouble = NULL;
        }
    }
}

int getFpd(double Fpd[16])
{

}

int getC1()
{

}