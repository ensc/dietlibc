/*
 * malloc/free by O.Dreesen
 */

#include <linux/unistd.h>
#include <asm/mman.h>
#include <linux/errno.h>
#include <pthread.h>

#if 0
#include <sys/mman.h>
#define _LIBC
#include <errno.h>
#endif

#include <linux/types.h>

#if defined(MAP_ANONYMOUS) && !defined(MAP_ANON)
#define MAP_ANON MAP_ANONYMOUS
#endif

#ifndef MAP_FAILED
#define MAP_FAILED ((void*)-1)
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif

extern void * mmap(void *start, size_t length, int prot , int flags, int fd, off_t offset);
extern void *memset(void *s, int c, size_t n);
extern void *memcpy(void *dest, const void *src, size_t n);

typedef struct t_alloc_head {
  struct t_alloc_head *ptr;
  unsigned long size;
} alloc_head;

/* guess what ? the virtual block size */
#define MEM_BLOCK_SIZE	4096

/* minimum allocated bytes */
#define MEM_ALLOC_MIN	4

/* Initial start position in memory */
#define MEM_ALLOC_START	((char*)0x18000000)

/* Make every block align */
#define MEM_ALIGN(s)	(((s)+MEM_ALLOC_MIN-1)&(~(MEM_ALLOC_MIN-1)))
#define PAGE_ALIGN(s)	(((s)+MEM_BLOCK_SIZE-1)&(~(MEM_BLOCK_SIZE-1)))
#define PAGE_ALIGNP(p)	((char*)PAGE_ALIGN((size_t)(p)))

#define END_OF_BLOCK(p)	((alloc_head*)(((char*)(p))+((p)->size)))
#define START_BLOCK(p)	((alloc_head*)(((char*)(p))-sizeof(alloc_head)))
#define START_DATA(p)	(((char*)(p))+sizeof(alloc_head))
#define MIN_ALLOC(s)	(((((s)+sizeof(alloc_head)-1)/MEM_ALLOC_MIN)+1)*MEM_ALLOC_MIN)

/* freelist handler */
static alloc_head base = {&base,0};
static char *alloc_get_end = MEM_ALLOC_START;
static int inalloc=0;
static pthread_mutex_t mutex_alloc = PTHREAD_MUTEX_INITIALIZER;

void free(void *ptr)
{
  alloc_head *prev,*p,*block;

  if (ptr==NULL) return;

  block=START_BLOCK(ptr);

  if (!inalloc) pthread_mutex_lock(&mutex_alloc);
  prev=&base;
  for (p=prev->ptr ; ; prev=p, p=p->ptr)
  {
    if ((block>prev)&&(block<p)) break; /* found the gap block belongs */
    if ((prev>p)&&(block<p)) break;	  /* block pre freelist */
    if ((prev>p)&&(block>prev)) break;  /* block after freelist */

    /* emergency escape: freelist has ONLY one entry the freelist base */
    if (p->ptr==p) break;
  }
  prev->ptr = block;

  if (END_OF_BLOCK(block)==p)
  { /* join right neighbor */
    block->ptr   = p->ptr;
    block->size += p->size;
  }
  else
    block->ptr = p;

  if (END_OF_BLOCK(prev)==block)
  { /* join left neighbor */
    prev->size += block->size;
    prev->ptr   = block->ptr;
  }
  if (!inalloc) pthread_mutex_unlock(&mutex_alloc);
}

static void *alloc_get_mem(unsigned long size)
{
  char *tmp;
  alloc_head *ah;

  size=PAGE_ALIGN(size);

  /* map free pages @ alloc_get_end */
  tmp=mmap(alloc_get_end, size, PROT_READ|PROT_WRITE,
	   MAP_ANONYMOUS|MAP_PRIVATE|MAP_FIXED, -1, 0);
  if (tmp==MAP_FAILED)
  {
    /* OK we can't map free pages @ alloc_get_end so try free position */
    tmp=mmap(0, size, PROT_READ|PROT_WRITE,
	     MAP_ANONYMOUS|MAP_PRIVATE, -1, 0);
    if (tmp==MAP_FAILED)
    {
      errno = ENOMEM;
      return NULL;	/* PANIC ! */
    }
    alloc_get_end=tmp;
  }

  alloc_get_end+=size;

  /* make a header */
  ah=(alloc_head*)tmp;
  ah->ptr=ah;
  ah->size=size;

  /* link new free maped pages in freelist */
  free(START_DATA(tmp));

  return &base;
}

void *malloc(size_t size)
{
  alloc_head *p, *prev;
  size_t need;

  /* needed MEM_ALLOC_MIN */
  need=MIN_ALLOC(size);

  pthread_mutex_lock(&mutex_alloc);
  inalloc=1;
  prev=&base;
  for (p=prev->ptr;;prev=p,p=p->ptr)
  {
    if (p->size>=need)
    {
      if (p->size==need)
      { /* fit PERFECT */
	prev->ptr=p->ptr;	/* relink freelist */
      }
      else
      {
	alloc_head *tmp=(alloc_head*)(((char*)p)+need);
	if ((p->size-need)<sizeof(alloc_head))
	{ /* work around: if there is not enough space for freelist head.
	   * this waste some bytes ( < sizeof(alloc_head) ) */
	  need=p->size;
	  prev->ptr=p->ptr;	/* relink freelist */
	}
	else
	{
	  prev->ptr=tmp;
	  tmp->ptr=p->ptr;
	  tmp->size=p->size-need;	/* remaining size */
	}

	p->size=need;	/* set size */
      }
      p->ptr=p;		/* self-link */

      inalloc=0;
      pthread_mutex_unlock(&mutex_alloc);
      return (void*)START_DATA(p);
    }
    else if (p==&base)
    {
      if ((p=alloc_get_mem(need))==NULL) goto err_out;
    }
  }
err_out:
  inalloc=0;
  pthread_mutex_unlock(&mutex_alloc);
  return NULL;
}

void *calloc(size_t nmemb,size_t size)
{
  size_t n=nmemb*size;
  void *tmp=malloc(n);
  if (tmp) memset(tmp,0,n);
  return tmp;
}

void *realloc(void *ptr,size_t size)
{
  alloc_head *tmp=0,*tf=0;
  long need=0;
  long diff=0;

  if (ptr)
  {
    if (size)
    {
      tmp=START_BLOCK(ptr);
      need=MIN_ALLOC(size);  /* only this size will survive */
      diff=tmp->size-need;
      if (diff<0)
      {
	if ((tf=malloc(size)))
	{
	  memcpy(tf,ptr,tmp->size-sizeof(alloc_head));
	  free(ptr);
	  return tf;
	}
	return NULL;
      }
      if (diff>=sizeof(alloc_head))
      {
	tmp->size=need;
	tf=END_OF_BLOCK(tmp);
	tf->ptr=tf;
	tf->size=diff;
	free(START_DATA(tf));
      }
      return ptr;
    }
    else
      free(ptr);
  }
  else if (size>0)
    return malloc(size);
  return NULL;
}
