/*
 * name:
 *	Malloc Trace Information
 *
 * environment:
 *	ANSI C
 *
 * author:
 *	you21979
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mti.h"

#undef malloc
#undef free

/* declare struct and grobal */

struct malloc_trace_info{
	const char *head;	/* head mark */
	mti *prev;		/* prev chain */
	mti *next;		/* next chain */
	size_t size;		/* addr length */
	char *callfunc;		/* use function name */
	char *file;		/* use function filename */
	int line;		/* use function file line */
};

static mti *___memory_trace_info_head___;	/* MTI Start Address */
static mti *___memory_trace_info_tail___;	/* Next Allocate Position */
static const char *___mti_check_header___ = "MTI";	/* mti memory header */

/*
 * declare macros
 */

#define mti_head	___memory_trace_info_head___
#define mti_tail	___memory_trace_info_tail___

#define MTI_HEAD_MARK	___mti_check_header___
#define MTI_INIT	0xcc
#define MTI_CLEAR	0xdd
#define MTI_TAIL_SIZE	1

/* get users memory from mti address */
#define MTI_GET_ADDR_BY_USR(p) (void *)(((char*)p) + sizeof(mti))

/* get mti address from users memory */
#define MTI_GET_ADDR_BY_MTI(p) (mti*)(((char*)p) - sizeof(mti))

#define MTI_CHK_BUF_OVER_RUN(p)\
(*((unsigned char*)p + sizeof(mti) + p->size) != MTI_INIT)

/*
 * functions
 */

/*
 * name:
 *	mt_register
 * argument:
 *	size_t size, char *callfunc, char *file, int line
 * return:
 *	void *
 * comment:
 *	memory allocate and memory information register
 */
void *mt_register(size_t size, char *callfunc, char *file, int line)
{
	mti *n, *p;

	if(size <= 0){
		return NULL;
	}

	if(!mti_head){
		p = NULL;
		n = NULL;
	}else{
		p = mti_tail;
		n = mti_tail->next;
	}

	while(1){
		if(!n){
			break;
		}
		p = n;
		n = n->next;
	}

	n = (mti*)malloc(sizeof(mti) + size + MTI_TAIL_SIZE);
	if(n == NULL){
		return NULL;
	}

	memset(n, MTI_INIT, sizeof(mti) + size + MTI_TAIL_SIZE);
	if(p)p->next = n;
	n->prev = p;
	n->next = NULL;
	mti_tail = n;
	if(!mti_head){
		mti_head = n;
	}


	n->head = MTI_HEAD_MARK;
	n->size = size;
	n->callfunc = callfunc;
	n->file = file;
	n->line = line;

	return MTI_GET_ADDR_BY_USR(n);
}

/*
 * name:
 *	mt_unregister
 * argument:
 *	void *addr
 * return:
 *	int
 * comment:
 *	memory free and memory information unregister
 */
int mt_unregister(void *addr)
{
	mti *n;

	if(!addr){
		fprintf(stderr, "%s\n", "Warrning: buffer addr is NULL");
		return 0;
	}

	n = mt_get_addr_by_mti(addr);
	if(n == NULL){
		/* non register addr */
		fprintf(stderr, "%s\n%s\n",
			"Warrning: buffer addr in non mti header",
			"if destroy memory table ? or mti register no use allocate buffer"
		);
		free(addr);	/* destroy memory tbl? */
		return -1;
	}
	if(MTI_CHK_BUF_OVER_RUN(n)){
		fprintf(stderr, "%s : allocator %s : size %ld : %s(%d)\n",
			"Warrning: buffer over run",
			n->callfunc, n->size, n->file, n->line
		);
	}


	if(n->prev)n->prev->next = n->next;
	if(n->next)n->next->prev = n->prev;

	if(mti_head == n){
		mti_head = mti_head->next;
	}
	if(mti_tail == n){
		mti_tail = mti_tail->prev;
	}

	memset(n, MTI_CLEAR, sizeof(mti) + n->size);
	free(n);

	return 0;
}

/*
 * name:
 *	mt_get_addr_info
 * argument:
 *	mti **mt
 * return:
 *	int
 * comment:
 *	get mti head address
 */
int mt_get_addr_info(mti **mt)
{
	if(!mt || !mti_head){
		return -1;
	}
	*mt = mti_head;
	return 0;
}

/*
 * name:
 *	mt_get_addr_next_info
 * argument:
 *	mti **mt
 * return:
 *	int
 * comment:
 *	next mti address
 */
int mt_get_addr_next_info(mti **mt)
{
	if(!mt || !*mt){
		return -1;
	}
	*mt = (*mt)->next;
	return 0;
}

/*
 * name:
 *	mt_get_addr
 * argument:
 *	mti *mt, void **addr, size_t *size, char **callfunc, char **file, int *line
 * return:
 *	int
 * comment:
 *	get mti information
 */
int mt_get_addr(mti *mt, void **addr, size_t *size, char **callfunc, char **file, int *line)
{
	if(!mt){
		return -1;
	}

	if(addr){
		*addr = MTI_GET_ADDR_BY_USR(mt);
	}

#define SET(p)\
	if(p)*p = mt->p

	SET(size);
	SET(callfunc);
	SET(file);
	SET(line);
#undef SET
	return 0;
}

/*
 * name:
 *	mt_get_count
 * argument:
 *	void
 * return:
 *	int
 * comment:
 *	get mti allocation count
 */
int mt_get_count(void)
{
	mti *mt;
	int i = 0;

	if(mt_get_addr_info(&mt) != 0){
		return 0;
	}
	i++;
	while(1){
		mt_get_addr_next_info(&mt);
		if(!mt){
			break;
		}
		i++;
	}
	return i;
}

/*
 * name:
 *	mt_get_buf_size
 * argument:
 *	void *addr
 * return:
 *	size_t
 * comment:
 *	get allocate address size from (mti) users memory
 */
size_t mt_get_buf_size(void *addr)
{
	mti *mt;
	mt = mt_get_addr_by_mti(addr);
	if(!mt){
		return 0;
	}
	return mt->size;
}

/*
 * name:
 *	mt_get_addr_by_mti
 * argument:
 *	void *addr
 * return:
 *	mti*
 * comment:
 *	get mti address from users memory
 */
mti* mt_get_addr_by_mti(void *addr)
{
	mti *mt;
	int i;
	mt = MTI_GET_ADDR_BY_MTI(addr);
	if(!mt){
		return NULL;
	}

	i = memcmp(mt->head, MTI_HEAD_MARK, strlen(MTI_HEAD_MARK) + 1);
	return (i == 0 ? mt : NULL);
}




