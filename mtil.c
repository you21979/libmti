/*
 * Malloc Trace Information Layer(interface)
 *			create by you21979
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mti.h"
#include "mtil.h"

/* malloc wrapper */
void *mt_malloc(size_t size, char *callfunc, char *file, int line)
{
	return mt_register(size, callfunc, file, line);
}

/* free wrapper */
int mt_free(void *addr)
{
	return mt_unregister(addr);
}

/* realloc wrapper */
void *mt_realloc(void *addr, size_t size, char *callfunc, char *file, int line)
{
	void *buf;
	buf = mt_malloc(size, callfunc, file, line);
	if(buf){
		memmove(buf, addr, mt_get_buf_size(addr));
		mt_free(addr);
	}
	return buf;
}

/* calloc wrapper */
void *mt_calloc(size_t size1, size_t size2, char *callfunc, char *file, int line)
{
	void *buf;
	buf = mt_malloc(size1 * size2, callfunc, file, line);
	if(buf){
		memset(buf, 0x00, size1 * size2);
	}
	return buf;
}

/* strdup wrapper */
char *mt_strdup(char *str, char *callfunc, char *file, int line)
{
	char *buf;
	buf = (char*)mt_malloc(sizeof(char) * (strlen(str) + 1), callfunc, file, line);
	if(buf){
		strcpy(buf, str);
	}
	return buf;
}

#define MSG_FORMAT "addr:%p, size:%ld, allocator: %s, file %s at line %d \n"

/* -------------- */
/* TRACE FUNCTION */
/* -------------- */
int mt_buf_print_info(void *addr)
{
	mti *m;
	char *buf;
	size_t s;
	char *callfunc;
	char *file;
	int line;
	m = mt_get_addr_by_mti(addr);
	if(m == NULL){
		return 0;
	}
	if(mt_get_addr(m, (void**)&buf, &s, &callfunc, &file, &line) == 0){
		fprintf(stderr, MSG_FORMAT, buf, s, callfunc, file, line);
	}
	return 0;
}

int mt_all_print_info()
{
	mti *m;
	char *buf;
	size_t s;
	char *callfunc;
	char *file;
	int line;

	fprintf(stderr, "allocate count: %d\n", mt_get_count());

	if(mt_get_addr_info(&m) != 0){
		return 0;
	}
	while(1){
		if(mt_get_addr(m, (void**)&buf, &s, &callfunc, &file, &line) == 0){
			fprintf(stderr, MSG_FORMAT, buf, s, callfunc, file, line);
		}
		if(mt_get_addr_next_info(&m) != 0){
			break;
		}
	}
	return 0;
}

int mt_all_print_info_to_buf(char *data)
{
	mti *m;
	char *buf;
	size_t s;
	char *callfunc;
	char *file;
	int line;
	char tmp[1024];

	sprintf(data, "allocate count: %d\n", mt_get_count());

	if(mt_get_addr_info(&m) != 0){
		return 0;
	}
	while(1){
		if(mt_get_addr(m, (void**)&buf, &s, &callfunc, &file, &line) == 0){
			sprintf(tmp, MSG_FORMAT, buf, s, callfunc, file, line);
			strcat(data, tmp);
			mt_buf_memory_dump(buf, "/tmp/ebr_mem_leak_dump.log");
		}
		if(mt_get_addr_next_info(&m) != 0){
			break;
		}
	}
	return 0;
}

int mt_buf_memory_dump(void *addr, const char *filename)
{
	mti *m;
	char *buf;
	size_t s;
	char *callfunc;
	char *file;
	int line;
	FILE *fp;

	m = mt_get_addr_by_mti(addr);
	if(m == NULL){
		return 0;
	}

	fp = fopen(filename, "a+");
	if(fp == NULL){
		return -1;
	}

	if(mt_get_addr(m, (void**)&buf, &s, &callfunc, &file, &line) == 0){
		size_t i;
		fprintf(fp, "||addr:%p, size:%ld, allocator: %s, "
			"file %s at line %d, over run status :%x, dump start-->||",
			buf, s, callfunc, file, line, *((unsigned char *)buf + s));
		for(i=0; i<s; i++){
			fprintf(fp, "%c", ((unsigned char *)buf)[i]);
		}
		fprintf(fp, "||<-||\n");
	}

	fclose(fp);

	return 0;
}


