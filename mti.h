/*
 * Malloc Trace Information
 */

#ifndef __MTI_H__
#define __MTI_H__

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct malloc_trace_info mti;

/* pointer address viewer */
#define PV(p)\
(\
	fprintf(stderr, "%s=%p\n", #p, p),\
	fflush(stderr)\
)

void *mt_register(size_t size, char *callfunc, char *file, int line);
int mt_unregister(void *addr);

int mt_get_addr_info(mti **mt);
int mt_get_addr_next_info(mti **mt);
int mt_get_addr(mti *mt, void **addr, size_t *size, char **callfunc, char **file, int *line);
int mt_get_count(void);
size_t mt_get_buf_size(void *addr);
mti *mt_get_addr_by_mti(void *addr);

#ifdef __cplusplus
}
#endif

#endif

