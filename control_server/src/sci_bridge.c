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

#include "motor_config.h"

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
    
    char argument[100];
    sprintf(argument,"exec(%s);",scriptPath);
    SendScilabJob(argument);

	return 0;
}

void Init_MotorModel()
{
    char JobString[100];
    sprintf(JobString, "[sys_c_p, sys_u_p, sys_u, sys_f]=Motor(%f,%f,%f,%f,%f,%f,%f)" , E,b,J,K,R,L,T_pwm);
    SendScilabJob(JobString);

    sprintf(JobString, "[Fpd, f_u, Ap, Bp]=getFpd(%f,%f)", current_weight, input_weight);
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

int process_ref(double velo, double accel, double V,double Tf)
{
    char JobString[100];
    sprintf(JobString, "[ref, Idot]=calculate_ref(%f, %f, sys_c_p ,%f, %f)" , velo, accel ,V, Tf);
    SendScilabJob(JobString);
}

double * read_matrix(const char variableToBeRetrieved[], int * rowCount, int * colCount)
{
	/******************************** READ ****************************/

    /* Load the previously set variable A */
    {
        void* pvApiCtx = NULL;
        int rowA_ = 0, colA_ = 0, lp = 0;
        int i = 0, j = 0;

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
            *rowCount = rowA_;
            *colCount = colA_;
            return matrixOfDouble;
        }
        else return NULL;
    }
}
