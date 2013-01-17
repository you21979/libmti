#ifndef __MSL_H__
#define __MSL_H__

#include "mti.h"

#ifdef __cplusplus
extern "C"
{
#endif

int msl_alloc(void **buf, size_t size);
int msl_free(void **buf);

#ifdef __cplusplus
}
#endif

#endif

