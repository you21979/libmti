/*
 * Name:
 *	Memory Safe Layer
 * Author:
 *	you21979
 * Comment:
 *	if debugmode compile, allocate and free with initialize cleanup memory
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef DEBUG
#include "msl.h"
#endif

int msl_alloc(void **buf, size_t size)
{
#ifdef DEBUG
	*buf = mt_regist(size, "msl_alloc", __FILE__, __LINE__);
#else
	*buf = malloc(size);
#endif
	if(!buf){
		return -1;
	}
	return 0;
}

int msl_free(void **buf)
{
	if(!buf || !*buf){
		return 0;
	}
#ifdef DEBUG
	mt_unregist(*buf);
#else
	free(*buf);
#endif
	*buf = NULL;
	return 0;
}


