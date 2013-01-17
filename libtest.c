/*
 * library test routine
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "msl.h"
#include "mtil.h"
#include <math.h>
#include <time.h>

int main()
{
	char d[10];
	int rc;
	int i;
	char *data[100000];

	memset(data, 0x00, sizeof(data));


#define L(p1,p2)\
{\
printf(p1,p2);\
fflush(NULL);\
}
	srand((unsigned)time((time_t*)NULL));
	srand(rand() ^ rand());

#define rnd(max)	floor(max * (rand() / (double)RAND_MAX))

msl_alloc((void**)&(data[0]), 10);

mt_buf_memory_dump(data[0], "memdump.log");
strcpy(data[0], "aiueoaiueo");
mt_buf_memory_dump(data[0], "memdump.log");
msl_free((void**)&(data[0]));

return 0;

	while(1){
L("%s\n", "A");
		for(i=0;i<30;i++){
			rc = rnd(512);
			msl_alloc((void**)&(data[i]), rc);
//L("a: %p\n", data[i]);
		}
L("%s\n", "B");
		for(i=10;i<20;i++){
//L("b: %p\n", data[i]);
			msl_free((void**)&(data[i]));
		}
L("%s\n", "C");
		for(i=20;i<30;i++){
//L("c: %p\n", data[i]);
			msl_free((void**)&(data[i]));
		}
L("%s\n", "D");
		for(i=20;i<30;i++){
			rc = rnd(512);
			msl_alloc((void**)&(data[i]), rc);
		}
L("%s\n", "E");
		for(i=0;i<10;i++){
//L("e: %p\n", data[i]);
			msl_free((void**)&(data[i]));
		}
L("%s\n", "F");
		for(i=20;i<30;i++){
//L("f: %p\n", data[i]);
			msl_free((void**)&(data[i]));
		}
	}

	return 0;
}




