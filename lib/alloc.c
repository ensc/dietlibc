/*
 * malloc/free by O.Dreesen
 */

#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include "dietfeatures.h"
#include <sys/types.h>

#ifndef MAP_FAILED
#define MAP_FAILED ((void*)-1)
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif

extern void *memset(void *s, int c, size_t n);
extern void *memcpy(void *dest, const void *src, size_t n);

typedef struct t_alloc_head {
  unsigned long magic;
  unsigned long size;
} alloc_head;

typedef struct t_free_head {
  unsigned long magic;
  unsigned long size;
  struct t_free_head *next;
  struct t_free_head *prev;
} free_head;

/* guess what ? the virtual block size (this is the common page size) */
#define MEM_BLOCK_SIZE	8192

/* minimum allocated bytes */
#define MEM_ALLOC_MIN	(sizeof(free_head))

/* what's the time to use mmu based mallocs ? def.: >128K */
#define USE_BIG_MALLOC	(16*MEM_BLOCK_SIZE)

/* Initial start position in memory */
#define MEM_ALLOC_START	((char*)0x18000000)

/* Make every block align */
#define PAGE_ALIGN(s)	(((s)+MEM_BLOCK_SIZE-1)&(~(MEM_BLOCK_SIZE-1)))
#define PAGE_ALIGNP(p)	((char*)PAGE_ALIGN((size_t)(p)))

#define END_OF_BLOCK(p)	((free_head*)(((char*)(p))+((p)->size)))
#define START_BLOCK(p)	((alloc_head*)(((char*)(p))-sizeof(alloc_head)))
#define START_DATA(p)	(((char*)(p))+sizeof(alloc_head))
#define MIN_ALLOC(s)	(((((s)+sizeof(alloc_head)-1)/MEM_ALLOC_MIN)+1)*MEM_ALLOC_MIN)

#define FREE_MAGIC	0xdead2bad
#define ALLOC_MAGIC	0xf82dead1
#define BIG_MAGIC	0x2b162f81

/* a simple mmap :) */
static inline void *do_mmap(void *addr, unsigned long size)
{
  register int flags = MAP_ANONYMOUS|MAP_PRIVATE;
  if (addr) flags|=MAP_FIXED;
  return mmap(addr, size, PROT_READ|PROT_WRITE, flags, -1, 0);
}

/* malloc BIG chunks / let the mmu make this for us ... */
static void __big_free(void *ptr)
{
  alloc_head *ah;
  if (ptr==NULL) return;
  ah=START_BLOCK(ptr);
  if (ah->magic!=BIG_MAGIC) return;
  munmap(ah,ah->size);
}

static void *__big_malloc(size_t size)
{
  alloc_head *ah;
  size+=sizeof(alloc_head);
  size=PAGE_ALIGN(size);
  ah=do_mmap(0, size);
  if (ah==MAP_FAILED)
  {
    errno = ENOMEM;
    return NULL;	/* PANIC ! */
  }
  ah->magic	= BIG_MAGIC;
  ah->size	= size;
  return START_DATA(ah);
}

static void *__big_realloc(void *ptr, size_t size)
{
  alloc_head *ah=START_BLOCK(ptr);
  size+=sizeof(alloc_head);
  size=PAGE_ALIGN(size);
  if (ah->size!=size) {
    if ((ah=mremap(ah,ah->size,size,MREMAP_MAYMOVE))==MAP_FAILED)
      return 0;
    ah->size=size;
  }
  return START_DATA(ah);
}

/* freelist handler */
static free_head *base;

static char *alloc_get_end = MEM_ALLOC_START;

#ifdef DEBUG
static void run_list()
{
  free_head *tmp;
  printf("base: %08x\n",base);
  for (tmp=base;tmp;tmp=tmp->next)
  {
    printf("%08x: %08x %08x %d\n",tmp,tmp->prev,tmp->next,tmp->size);
  }
}
#endif

void __libc_free(void *ptr)
{
  register alloc_head *t;
  free_head *next,*tmp,*block;

  if (ptr==NULL) return;

  t=START_BLOCK(ptr);
  if (t->magic == BIG_MAGIC) { __big_free(ptr); return; }
  if (t->magic != ALLOC_MAGIC) return;

  block=(free_head*)t;
  block->magic=FREE_MAGIC;
  block->next=block->prev=0;

  if (base)
  {
    if (block<base)
    { /* pre freelist */
      block->next=base;
      tmp=base=block;
    }
    else
    { /* in or after last freelistentry */
      for (tmp=base; tmp->next ;tmp=tmp->next)
      {
	if (block<tmp->next) break;	/* FOUND */
      }
      block->next=tmp->next;
      block->prev=tmp;
    }

    next=tmp->next;
    /* join code */
    if (END_OF_BLOCK(block)==next)
    { /* join right neighbor */
      block->size+= next->size;
      next=next->next;
    }
    block->next = next;
    if (next) next->prev = block;

    if (END_OF_BLOCK(tmp)==block)
    { /* join left neighbor */
      tmp->size += block->size;
      tmp->next  = block->next;
      if (next) next->prev = tmp;
    }
  }
  else
  {
    base=block; /* start freelist */
  }
#ifdef DEBUG
  run_list();
#endif
}
void free(void *ptr) __attribute__((weak,alias("__libc_free")));

static void *alloc_get_mem(unsigned long size)
{
  char *tmp;
  alloc_head *ah;

  size=PAGE_ALIGN(size);

  /* map free pages @ alloc_get_end */
  tmp=do_mmap(alloc_get_end, size);
  if (tmp==MAP_FAILED)
  {
    /* OK we can't map free pages @ alloc_get_end so try free position */
    tmp=do_mmap(0, size);
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
  ah->magic=ALLOC_MAGIC;
  ah->size=size;

  /* link new free maped pages in freelist */
  __libc_free(START_DATA(tmp));

  return base;
}

void *__libc_malloc(size_t size)
{
  free_head *tmp;
  size_t need;

  /* needed MEM_ALLOC_MIN */
  need=MIN_ALLOC(size);

  if (size>USE_BIG_MALLOC) { return __big_malloc(size); }

  if (base==NULL)
  {
    if (alloc_get_mem(need)==NULL) goto err_out;
  }
  tmp=base;
  while(1)
  {
    if (tmp->size>=need)
    {
      /* relink freelist */
      if (tmp==base) base=base->next;
      if (tmp->prev) tmp->prev->next=tmp->next;
      if (tmp->next) tmp->next->prev=tmp->prev;

      if (tmp->size!=need)
      { /* fit is not perfect */
	alloc_head *tofree=(alloc_head*)(((char*)tmp)+need);
	tofree->magic=ALLOC_MAGIC;
	tofree->size=tmp->size-need;
	free(START_DATA(tofree));
	tmp->size=need;
      }
      tmp->magic=ALLOC_MAGIC;
      return (void*)START_DATA(tmp);
    }
    else if (tmp->next==NULL)
    {
      if (alloc_get_mem(need)==NULL) goto err_out;
      tmp=base;
    }
    else
      tmp=tmp->next;
  }
err_out:
  return NULL;
}
void *malloc(size_t size) __attribute__((weak,alias("__libc_malloc")));

void *calloc(size_t nmemb,size_t size)
{
  size_t n=nmemb*size;
  void *tmp=malloc(n);
  if (tmp) memset(tmp,0,n);
  return tmp;
}

void *realloc(void *ptr,size_t size)
{
  alloc_head *tmp=0;
  free_head *tf=0;
  long need=0;
  long diff=0;

  if (ptr)
  {
    if (size)
    {
      tmp=START_BLOCK(ptr);
      if (tmp->magic == BIG_MAGIC) return __big_realloc(ptr,size);

      need=MIN_ALLOC(size);  /* only this size will survive */
      diff=tmp->size-need;
      if (diff<0)
      {
	tf=END_OF_BLOCK(tmp);
	if ((tf->magic==FREE_MAGIC)&&(need<USE_BIG_MALLOC))
	{
	  if ((tf->size<(-diff))&&(tf->next==NULL))
	  { /* next ist last block ? */
	    alloc_get_mem(need);
	  }
	  if (tf->size>(-diff))
	  {
	    free_head *f=(free_head*)(((char*)tmp)+need);
#ifdef DEBUG
	    printf("found free block to realloc: %08x %08x %d %08x %d %08x %d\n",ptr,tmp,tmp->size,tf,tf->size,f,tf->size+diff);
	    printf("new: %08x %d\n",tmp,need);
	    run_list();
#endif
	    /* delink */
	    if (tf==base) base=tf->next;
	    if (tf->prev) tf->prev->next = tf->next;
	    if (tf->next) tf->next->prev = tf->prev;

	    if (tf->size+diff)
	    { /* free unused part */
	      f->magic=ALLOC_MAGIC;
	      f->size=tf->size+diff;
	      free(START_DATA(f));
	    }

	    tmp->size=need;
	    return (void*)START_DATA(tmp);
	  }
	}
	if ((tf=malloc(size)))
	{
	  memcpy(tf,ptr,tmp->size-sizeof(alloc_head));
	  free(ptr);
	  return tf;
	}
	return NULL;
      }
      if (diff>=(long)sizeof(alloc_head))
      {
	tmp->size=need;
	tf=END_OF_BLOCK(tmp);
	tf->magic=ALLOC_MAGIC;
	tf->next=tf;
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
