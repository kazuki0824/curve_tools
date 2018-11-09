/*
 * sci_bridge.c
 *
 *  Created on: 2018/11/07
 *      Author: kazuki
 */


#include <stdio.h>
#include <stdlib.h> /* getenv */
#include "api_scilab.h"  /* Provide functions to access to the memory of Scilab */
#include "call_scilab.h" /* Provide functions to call Scilab engine */

int init_sci()
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
	return 0;
}

