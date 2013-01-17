/*
 * Malloc Trace Information Layer(interface)
 */

#ifndef __MTIL_H__
#define __MTIL_H__

#ifdef __cplusplus
extern "C"
{
#endif


#define malloc(p)\
	mt_malloc(p, "malloc", __FILE__, __LINE__)
#define calloc(p1,p2)\
	mt_calloc(p1, p2, "calloc", __FILE__, __LINE__)
#define realloc(p1,p2)\
	mt_realloc(p1, p2, "realloc", __FILE__, __LINE__)
#define free(p)\
	mt_free(p)
#define strdup(p)\
	mt_strdup(p, "strdup", __FILE__, __LINE__)

#define msl_alloc(p1,p2)\
(\
	*(p1) = mt_malloc(p2, "msl_alloc", __FILE__, __LINE__),\
	(*(p1) ? 0 : -1)\
)

#define msl_free(p)\
	mt_free(*(p))


void *mt_malloc(size_t size, char *callfunc, char *file, int line);
int mt_free(void *addr);
void *mt_calloc(size_t size1, size_t size2, char *callfunc, char *file, int line);
void *mt_realloc(void *addr, size_t size, char *callfunc, char *file, int line);
char *mt_strdup(char *str, char *callfunc, char *file, int line);

int mt_buf_print_info(void *addr);
int mt_all_print_info();
int mt_all_print_info_to_buf(char *buf);
int mt_buf_memory_dump(void *addr, const char *filename);

#ifdef __cplusplus
}
#endif

#endif

