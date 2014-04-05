#define errno fnord
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <elf.h>
#include <stdlib.h>
#include <stdint.h>
#undef errno

#if (__WORDSIZE == 64)

#define phdr Elf64_Phdr
#define ehdr Elf64_Ehdr
#define shdr Elf64_Shdr
#define sym Elf64_Sym
#define dyn Elf64_Dyn
#define rela Elf64_Rela
#define R_SYM ELF64_R_SYM
#define R_TYPE ELF64_R_TYPE

#else

#define phdr Elf32_Phdr
#define ehdr Elf32_Ehdr
#define shdr Elf32_Shdr
#define sym Elf32_Sym
#define dyn Elf32_Dyn
#define rela Elf32_Rela
#define R_SYM ELF32_R_SYM
#define R_TYPE ELF32_R_TYPE

#endif

static int errno;
__attribute__((visibility("hidden"))) int* __errno_location(void) { return &errno; }

static size_t _strlen(const char* s) {
  size_t i;
  for (i=0; s[i]; ++i);
  return i;
}
static char* _stpcpy(char* dest,const char* src) {
  size_t i;
  for (i=0; src[i]; ++i)
    dest[i]=src[i];
  dest[i]=0;
  return dest+i;
}
static char* _strchr(char* s,char c) {
  size_t i;
  for (i=0; s[i] && s[i]!=c; ++i) ;
  if (s[i]==c) return s+i; else return NULL;
}
static int _strcmp(const void* str1,const void* str2) {
  const unsigned char* a=str1;
  const unsigned char* b=str2;
  size_t i;
  int r;
  for (i=0; (r=(a[i]-b[i]))==0 && a[i]; ++i) ;
  return r;
}
static int _memcmp(const void* dst,const void* src,size_t count) {
  const unsigned char* a=dst;
  const unsigned char* b=src;
  size_t i;
  int r;
  for (i=0; i<count && (r=(a[i]-b[i]))==0; ++i) ;
  return r;
}
static void* _memcpy(void* dst,const void* src,size_t len) {
  char* a=dst;
  const char* b=src;
  size_t i;
  for (i=0; i<len; ++i)
    a[i]=b[i];
  return dst;
}
static void _memset(void* dst,unsigned char c,size_t len) {
  unsigned char* a=dst;
  size_t i;
  for (i=0; i<len; ++i) a[i]=c;
}
static ssize_t __write1(const char* s) {
  write(1,s,_strlen(s));
}
static ssize_t __write2(const char* s) {
  write(2,s,_strlen(s));
}

ssize_t write(int fd,const void* buf,size_t len) __attribute__((visibility("hidden")));
int open(const char* pathname,int flags, ...) __attribute__((visibility("hidden")));
ssize_t read(int fd,void* buf,size_t len) __attribute__((visibility("hidden")));
int close(int fd) __attribute__((visibility("hidden")));
ssize_t pread64(int fd, void *buf, size_t count, off64_t offset) __attribute__((visibility("hidden")));;
void *mmap(void *__addr, size_t __len, int __prot, int __flags, int __fd, off_t __offset) __attribute__((visibility("hidden")));
int munmap(void *__addr, size_t __len) __attribute__((visibility("hidden")));
int mprotect (void *__addr, size_t __len, int __prot) __attribute__((visibility("hidden")));
void exit(int res) __attribute__((visibility("hidden")));

static struct page {
  struct page* next;
  size_t a;
  char data[4096-sizeof(size_t)-sizeof(struct page*)];
}* heap;

static void* _malloc(size_t l) {
  struct page** p;
  if (l>sizeof(heap->data)) return 0;
  if (l%(sizeof(void*)*2)) {
    l += sizeof(void*)*2;
    l -= l%(sizeof(void*)*2);
  }
  for (p=&heap; *p && (*p)->a<l; p=&((*p)->next)) ;
  if (!*p) {
    void* tmp=mmap(0,4096,PROT_READ|PROT_WRITE,MAP_ANONYMOUS|MAP_PRIVATE,-1,0);
    if (tmp==MAP_FAILED) return 0;
    *p=tmp;
    (*p)->a=sizeof(heap->data);
  }
  if (l <= (*p)->a) {
    char* tmp=(*p)->data+sizeof((*p)->data)-(*p)->a;
    (*p)->a-=l;
    return tmp;
  } else
    return 0;	// can't happen
}

static char path[100];
static char* ldlp;

static struct dll {
  struct dll* next;
  ehdr* e;
  void* code,* data;
  size_t codelen,datalen,codeplus;
  char name[1];		// asciiz of library name
} *dlls, dllroot;

static int map_sections(int fd,const ehdr* e,const phdr* p,struct dll* D) {
  size_t i;
  uintptr_t codeplus=0;

  for (i=0; i<e->e_phnum; ++i) {
    if (p[i].p_type==PT_LOAD) {
      size_t delta=p[i].p_offset%4096;
      size_t maplen=p[i].p_filesz+delta;
      size_t bssdiff=(p[i].p_filesz+delta)%4096;
      char* c;
      if ((p[i].p_flags&PF_W) && (p[i].p_flags&PF_X)) {
	__write2("section is both executable and writable, aborting!\n");
	return 1;
      }
      if (!(p[i].p_flags&PF_W)) {
	/* code segment */
	size_t ofs,len,rolen=0,nolen=0,rolen2=0,vaddr=p[i].p_vaddr,baseofs=0;
	/* the first segment will be the code segment, and it will have
	 * either a fixed address or 0 if it's a shared library. We
	 * insist that the mapping start at file offset 0, and we extend
	 * the mapping so it includes the section table */
	ofs=p[i].p_offset;
	len=p[i].p_filesz;
	if (ofs) {
	  __write2("can't happen error: ofs!=0\n");
	  exit(1);
	  if (vaddr)
	    vaddr-=ofs;
	  else
	    baseofs=ofs;
	  rolen=ofs;
	  len+=ofs;
	  ofs=0;
	}
	if (ofs+len < e->e_shoff+e->e_shnum*e->e_shentsize) {
	  size_t needed=e->e_shoff+e->e_shnum*e->e_shentsize;
	  /* if this mapping does not include the section table is not
	   * included, extend the mapping to include it */
	  rolen2=e->e_shnum*e->e_shentsize;
	  if (rolen2>needed-len)
	    /* we were almost there, part of the section table was
	     * already mapped */
	    rolen2=needed-len;
	  else
	    nolen=needed-len-rolen2;
	  /*
	   * +------------------------+
	   * | rolen       e->        | if the mapping did not start at beginning of file, this is the beginning of file, PROT_READ
	   * +------------------------+
	   * | len         base->     | this is the actual mapping, base points here, PROT_READ|PROT_EXEC
	   * +------------------------+
	   * | nolen                  | stuff we don't really need and will mprotect PROT_NONE
	   * +------------------------+
	   * | rolen2      e+e->shoff | the section header table, PROT_READ
	   * +------------------------+
	   */
	}
	c=mmap((char*)vaddr,rolen+len+nolen+rolen2,
		  ((p[i].p_flags&PF_R)?PROT_READ:0) |
		  ((p[i].p_flags&PF_X)?PROT_EXEC:0),
		  MAP_SHARED|(vaddr?MAP_FIXED:0),
		  fd,0);
	/* in case the can't happen branch ever happens */
	D->e=(ehdr*)c;
	D->code=c+rolen; D->codelen=len;
//	D->s=(shdr*)(c+e->e_shoff);
	if (rolen>=4096)	/* if we extended the mapping in the front, remove exec permissions */
	  mprotect(c,rolen&~4095,PROT_READ);
	if (!vaddr && !codeplus) codeplus=(uintptr_t)(c+rolen);
	if (nolen) {
	  /* We mapped junk in the middle.
	   * If there are full pages in there, map them PROT_NONE */
	  char* start=c+rolen+len;
	  size_t len=nolen;
	  size_t diff=(-(uintptr_t)start & 4095);
	  if (diff < nolen) {
	    /* diff is the part at the beginning we need to skip because
	     * it's on a page we actually need to be executable.
	     * Now find out if we overshoot onto a page we want */
	    size_t removeatend=((uintptr_t)c+rolen+len)&4095;
	    mprotect(start+diff,len-diff-removeatend,PROT_NONE);
	  }
	}
	if (rolen2) {
	  /* Now we want to mprotect PROT_READ the section table.
	   * What makes this complex is that mprotect granularity is one
	   * page.  First figure out the region we are interested in. */
	  char* start=c+rolen+len+nolen;
	  size_t len=rolen2;
	  /* we want to mprotect from start to start+len */
	  int diff=((uintptr_t)start)&4095;
	  /* This is the offset of start from the page start.  We need
	   * to mprotect the whole page -- unless it also has code on
	   * it, in which case we need to leave it alone */
	  if (diff > nolen) {
	    size_t roundup=(-(uintptr_t)start)&4095;
	    /* we need to skip the first page. Let's see if there is
	     * anything left in that case */
	    start+=roundup;
	    if (rolen2>roundup)
	      len-=roundup;
	    else
	      len=0;
	  } else {
	    start-=diff;
	    len+=diff;
	  }
	  if (len) mprotect(start,len,PROT_READ);
	}
      } else if (p[i].p_flags&PF_W) {	/* data segment */
	/* We have already mapped the code segment to base.
	 * For programs, the base mapping of the code and data segment
	 * is absolute and we just map there with MAP_FIXED.  For shared
	 * libraries however, the base mapping of the code segment is 0,
	 * and the vaddr of the data segment is not absolute but
	 * relative to the code segment */

	/* The data segment may not start on a page boundary.  Round
	 * start address if necessary. */
	size_t addr,ofs,len,memsetstart=0,memsetend=0,additional=0;
	addr=p[i].p_vaddr+codeplus;
	ofs=p[i].p_offset;
	len=p[i].p_filesz;
	if ((memsetstart=(ofs%4096))) {
	  len+=memsetstart;
	  ofs-=memsetstart;
	  addr-=memsetstart;
	}

	/* The data segment consists of actual data, but a part of it is
	 * data initialized to 0, the .bss section.  This section is not
	 * actually put in the file to save space, but the ELF loader
	 * (that's us) is expected to allocate that data at program
	 * start. */
	memsetend=p[i].p_memsz-p[i].p_filesz;
	if (memsetend) {
	  /* We have a .bss.  We need to handle two cases.  First: if
	   * the number of bytes is small enough to fit on the last page
	   * we already mapped for the data, all we need to do is memset
	   * it.  Otherwise we needto memset the rest of that page and
	   * map some additional pages after it. */
	  size_t bytes_on_last_page=(-len)&4095;
	  if (memsetend<bytes_on_last_page)
	    len+=memsetend;
	  else {
	    len+=bytes_on_last_page;
	    additional=memsetend-bytes_on_last_page;
	    memsetend=bytes_on_last_page;
	  }
	}

	c=mmap((char*)addr,len,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_FIXED,fd,ofs);
	if (c==MAP_FAILED) {
	  __write2("mmap failed!\n");
	  return 1;
	}

	if (memsetstart) _memset(c,0,memsetstart);
	if (memsetend) _memset(c+len-memsetend,0,memsetend);
	if (additional) {
	  char* tmp=mmap(c+len,additional,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
	  if (tmp==MAP_FAILED) {
	    __write2("mmap failed!\n");
	    return 1;
	  }
	}
	D->data=c+memsetstart; D->datalen=len-memsetstart;
	D->codeplus=codeplus;
      } else {
	__write2("can't happen error: LOAD segment that is neither code nor data.\n");
	return 1;
      }
    }
  }
  return 0;
}


static int __loadlibrary(const char* fn) {
  struct dll* D;
  int fd;
  char buf[1000];
  ehdr* e;
  phdr* p;
  shdr* s;
  size_t i;
  char* code=0,* data=0;
  char* base=0;
  size_t codelen,datalen;
#if 0
  __write1("trying ");
  __write1(fn);
  __write1("\n");
#endif
  fd=open(fn,O_RDONLY);
  if (fd==-1) return -1;
  if (read(fd,buf,1000)<1000) {
kaputt:
    close(fd);
    return -1;
  }
  if (_memcmp(buf,"\177ELF",4)) goto kaputt;
  e=(ehdr*)buf;

  if (e->e_ident[4] != (sizeof(size_t)/4) ||	// wrong word size
      e->e_type != ET_DYN ||			// not shared object
      e->e_machine != 				// for different architecture
#if defined(__x86_64__)
	EM_X86_64
#elif defined(__i386__)
	EM_386
#else
#error architecture not recognized, please add
#endif
                                ||
      e->e_phentsize != sizeof(phdr) ||		// wrong program header entry size
      e->e_phnum > 20 ||			// too many program header entries
      e->e_shentsize !=sizeof(shdr) ||		// wrong section header entry size
      e->e_shnum > 100)				// too many sections
    goto kaputt;


  if (e->e_phoff>=1000 || e->e_phentsize*e->e_phnum>1000-e->e_phoff) {
    size_t wanted=e->e_phentsize*e->e_phnum;
    p=alloca(wanted);
    if (pread64(fd,p,wanted,e->e_phoff)!=wanted)
      goto kaputt;
  } else
    p=(phdr*)(buf+e->e_phoff);

  struct dll dll;
  if (map_sections(fd,e,p,&dll)) {
    __write2("map_sections failed\n");
    goto kaputt;
  }

  close(fd);

  {
    const char* tmp;
    for (i=0; fn[i]; ++i)
      if (fn[i]=='/') tmp=fn+i;
    ++tmp;
    D=_malloc(sizeof(struct dll)+_strlen(tmp));
    _stpcpy(D->name,tmp);
  }
  D->next=0;
  D->code=dll.code; D->codelen=dll.codelen;
  D->data=dll.data; D->datalen=dll.datalen;
//  D->s=dll.s;
  D->e=dll.e;
  D->codeplus=dll.codeplus;
  {
    struct dll** x;
    for (x=&dlls; *x; x=&(*x)->next) ;
    *x=D;
  }
  return 0;
}

static int loadlibrary(const char* fn) {
  char lp[200];
  int r;
  char* c;
  const char* shortname=fn;
  struct dll* d;

  {
    size_t i;
    for (i=0; fn[i]; ++i)
      if (fn[i]=='/') shortname=fn+i+1;
  }

  if (_strlen(fn)>50) return -1;
  for (d=dlls; d; d=d->next)
    if (!_strcmp(d->name,shortname))
      return 0;

  __write1("loadlibrary(\"");
  __write1(fn);
  __write1("\")\n");
  if (fn[0]=='/') {
    return __loadlibrary(fn);
  }

  c=_stpcpy(lp,path);
  *c++='/';
  _stpcpy(c,fn);
  r=__loadlibrary(lp);
  if (r==0) return r;
  if (ldlp) {
    size_t i;
    char* d;
    c=ldlp;
    for (i=0; ; ++i) {
again:
      if (c[i]==':' || c[i]==0) {
	if (i<100) {
	  lp[i]='/';
	  _stpcpy(lp+i+1,fn);
	  r=__loadlibrary(lp);
	  if (r==0) return;
	}
	if (c[i]==0) break;
	c+=i+1; i=0; goto again;
      } else
	if (i<100) lp[i]=c[i];
    }
  }
  return r;
}

static int loadlibs(struct dll* D) {
  size_t i;
  phdr* p=(phdr*)((char*)D->e+D->e->e_phoff);
  dyn* d;
  size_t dnum,dynstrlen;
  char* dynstr;

  /* we know we have exactly one code and exactly one data segment,
   * otherwise we wouldn't have gotten this far */
  for (i=0; i<D->e->e_phnum; ++i)
    if (p[i].p_type==PT_DYNAMIC) {
      d=(dyn*)((char*)p[i].p_vaddr+D->codeplus);
      dnum=p[i].p_memsz/sizeof(dyn);
      break;
    }
  for (i=0; i<dnum; ++i)
    if (d[i].d_tag==DT_STRTAB) {
      dynstr=(char*)d[i].d_un.d_ptr+D->codeplus;
      break;
    } else if (d[i].d_tag==DT_NULL)
      break;

  /* we now have a dynamic section we can traverse */
  for (i=0; i<dnum; ++i) {
    if (d[i].d_tag==DT_NEEDED) {
      if (loadlibrary(dynstr+d[i].d_un.d_val)) {
	__write2("library ");
	__write2(dynstr+d[i].d_un.d_val);
	__write2(" not found!\n");
	exit(2);
      }
    } else if (d[i].d_tag==DT_NULL)
      break;
  }

  return 0;
}

static unsigned int elf_hash(const unsigned char *name) {
  unsigned int h=0, g;

  while (*name) {
    h = (h<<4) + *(name++);
    if ((g = h&0xf0000000)) h ^= g>>24;
    h ^= g;
  }
  return h;
}

static uint_fast32_t gnu_hash(const unsigned char *s) {
  unsigned char c;
  uint_fast32_t h=5381;
  for(c=*s;(c!='\0');c=*++s) {
//    h=h*33+c;
    h=((h<<5)+h)+c;
  }
  return (h&0xffffffff);
}

static char* dlsym_int(const char* symbol,struct dll* x) {
  for (; x; x=x->next) {
    size_t i;
    dyn* d;
    sym* sy;
    phdr* p=(phdr*)(x->e->e_phoff+(char*)x->e);
    const char* strtab;
    size_t dnum;
    int* hash=0;
    for (i=0; i<x->e->e_phnum; ++i)
      if (p[i].p_type==PT_DYNAMIC) {
	d=(dyn*)(x->codeplus + p[i].p_vaddr);
	dnum=p[i].p_memsz/sizeof(dyn);
	break;
      }

    for (i=0; i<dnum; ++i) {
      if (d[i].d_tag == DT_HASH)
	hash=(int*)((char*)x->codeplus + d[i].d_un.d_ptr);
      else if (d[i].d_tag == DT_SYMTAB)
	sy=(sym*)((char*)(x->codeplus+d[i].d_un.d_ptr));
      else if (d[i].d_tag == DT_STRTAB)
	strtab=(char*)(x->codeplus+d[i].d_un.d_ptr);
    }

    /* hash[0] is the number of buckets. */
    /* hash[1] is the hash chain length, not used here */
    size_t bhash=elf_hash(symbol)%hash[0];
    unsigned int* chain=hash+2+hash[0];
    unsigned int index;
    for (index=(hash+2)[bhash]; index; index=chain[index]) {
#if 0
      __write1(strtab+sy[index].st_name); __write1("\n");
#endif
      if (sy[index].st_value && sy[index].st_shndx!=SHN_UNDEF && !_strcmp(strtab+sy[index].st_name,symbol)) {
	return (char*)x->codeplus+sy[index].st_value;
      }
    }
#if 0
    if (x->next) {
      __write1(" ... next: ");
      __write1(x->next->name);
      __write1("\n");
    }
#endif
  }
  return 0;
}

static void* dlsym(const char* s) {
  return dlsym_int(s,&dllroot);
}

static void* _dlsym(const char* s) {
  void* x=dlsym(s);
  if (!x) {
    __write2("ld.so: lookup of symbol \"");
    __write2(s);
    __write2("\" failed.\n");
//    exit(1);
  }
  return x;
}

static void resolve(struct dll* D) {
  size_t i;
  phdr* p=(phdr*)((char*)D->e+D->e->e_phoff);
  dyn* d=0;
  size_t dnum,dynstrlen,rnum=0;
  char* dynstr=0, *pltgot=0, *pltrel=0;
  rela* r=0;
  sym* symtab=0;

  /* we know we have exactly one code and exactly one data segment,
   * otherwise we wouldn't have gotten this far */
  for (i=0; i<D->e->e_phnum; ++i)
    if (p[i].p_type==PT_DYNAMIC) {
      d=(dyn*)((char*)p[i].p_vaddr+D->codeplus);
      dnum=p[i].p_memsz/sizeof(dyn);
      break;
    }
  for (i=0; i<dnum; ++i)
    if (d[i].d_tag==DT_STRTAB)
      dynstr=(char*)d[i].d_un.d_ptr+D->codeplus;
    else if (d[i].d_tag==DT_RELA)
      r=(rela*)((char*)d[i].d_un.d_ptr+D->codeplus);
    else if (d[i].d_tag==DT_RELASZ)
      rnum=d[i].d_un.d_val/sizeof(rela);
    else if (d[i].d_tag==DT_SYMTAB)
      symtab=(sym*)((char*)d[i].d_un.d_ptr+D->codeplus);
    else if (d[i].d_tag==0)
      break;

  for (i=0; i<rnum; ++i) {
    size_t* x=(size_t*)((char*)(r[i].r_offset+D->codeplus));
    char* y;
    size_t sym=R_SYM(r[i].r_info);
    switch (R_TYPE(r[i].r_info)) {
#if defined(__x86_64__)
    case R_X86_64_64:
      *x=D->codeplus+symtab[sym].st_value;
      break;
    case R_X86_64_COPY:
      y=dlsym_int(symtab[sym].st_name+dynstr,D->next);
      if (!y && ELF32_ST_BIND(symtab[sym].st_info) != STB_WEAK) {
	__write2("symbol lookup failed: ");
	__write2(dynstr+symtab[sym].st_name);
	__write2("\n");
	exit(1);
      }
      _memcpy(x,y,symtab[sym].st_size);
      break;
    case R_X86_64_GLOB_DAT:
    case R_X86_64_JUMP_SLOT:
      y=dlsym(symtab[sym].st_name+dynstr);
      if (!y && ELF32_ST_BIND(symtab[sym].st_info) != STB_WEAK) {
	__write2("symbol lookup failed: ");
	__write2(dynstr+symtab[sym].st_name);
	__write2("\n");
	exit(1);
      }
      *x=(uintptr_t)y;
      break;
    case R_X86_64_RELATIVE:
      *x=r[i].r_addend+D->codeplus;
      break;
    case R_X86_64_32:
      *(uint32_t*)x=*(uint32_t*)_dlsym(symtab[sym].st_name+dynstr)+r[i].r_addend;
      break;
    default:
      __write2("unknown relocation type!\n");
      exit(1);
      break;
#else
#error fixme: add relocation types for your platform
#endif
    }
  }

  return;
}

int main(int argc,char* argv[],char* envp[]) {
  int fd;
  size_t l;
  char* m;
  char buf[1000];
  ehdr* e;
  phdr* p;
  phdr* code=0,* data=0;
  shdr* s;
  dyn* d;
  size_t dnum;
  char* dynstr;
  size_t dynstrlen;
  size_t i;

#if 0
  {
    fd=open("/proc/self/maps",O_RDONLY);
    if (fd!=-1) {
      size_t l;
      do {
	l=read(fd,buf,sizeof(buf));
	write(1,buf,l);
      } while (l==sizeof(buf));
      close(fd);
    }
  }
#endif
  if (argc<2) {
    __write2("usage: ld.so /path/to/binary\n");
    return 0;
  }
  fd=open("/etc/diet.ld.conf",O_RDONLY);
  if (fd!=-1) {
    int r=read(fd,path,99);
    if (r>0) path[r]=0;
    while (r>0 && path[r-1]=='\n') path[--r]=0;
    close(fd);
  }
  for (i=0; envp[i]; ++i) {
    if (_memcmp(envp[i],"LD_LIBRARY_PATH=",16)==0)
      ldlp=envp[i]+16;
  }
  fd=open(argv[1],O_RDONLY);
  if (fd==-1) {
    __write2("could not open \"");
    __write2(argv[1]);
    __write2("\".\n");
    return 1;
  }
  l=read(fd,buf,1000);
  if (l<sizeof(ehdr) || _memcmp(buf,"\177ELF",4)) {
kaputt:
    __write2("invalid ELF file \"");
    close(fd);
    return 1;
  }
  e=(ehdr*)buf;

  if (e->e_ident[4] != (sizeof(size_t)/4)) {
    __write2("wrong word size!\n");
    return 1;
  }
#if 0
  if (e->e_ident[7] != ELFOSABI_LINUX) {
    __write2("ABI not Linux!\n");
    return 1;
  }
#endif
  if (e->e_type != ET_EXEC) {
    __write2("not an executable!\n");
    return 1;
  }
  if (e->e_machine !=
#if defined(__x86_64__)
      EM_X86_64
#elif defined(__i386__)
      EM_386
#else
#error architecture not recognized, please add
#endif
     ) {
    __write2("invalid architecture!\n");
    return 1;
  }

  if (e->e_phentsize != sizeof(phdr)) {
    __write2("invalid phentsize!\n");
    return 1;
  }
  if (e->e_phnum > 20) {
    __write2("phnum too large!\n");
    return 1;
  }
  if (e->e_shentsize != sizeof(shdr)) {
    __write2("invalid shentsize!\n");
    return 1;
  }
  if (e->e_shnum > 100) {
    __write2("shnum too large!\n");
    return 1;
  }

  if (e->e_phoff>=l || e->e_phentsize*e->e_phnum>l-e->e_phoff) {
    size_t wanted=e->e_phentsize*e->e_phnum;
    p=alloca(wanted);
    if (pread64(fd,p,wanted,e->e_phoff)!=wanted)
      goto kaputt;
  } else
    p=(phdr*)(buf+e->e_phoff);

  dlls=&dllroot;
  if (map_sections(fd,e,p,&dllroot)) {
    __write2("map_sections failed!\n");
    return 1;
  }
  close(fd);

  loadlibs(&dllroot);

  /* now load the prerequisites of the libraries we loaded */
  {
    struct dll* x;
    for (x=dlls; x; x=x->next) {
      loadlibs(x);
    }
  }

  resolve(&dllroot);

  __write2("jumping...\n");

  {
    int (*_init)(int argc,char* argv[],char* envp[])=(void*)(e->e_entry+dllroot.codeplus);
    return _init(argc,argv,envp);
  }
#if 0
  {
    char* x=dlsym("theint");
    __write1("done\n");
  }
#endif

#if 0
  printf("jump to %p\n",e->e_entry);
  for (i=0; i<16; ++i) {
    printf("%02x ",((unsigned char*)e->e_entry)[i]);
  }
#endif

  return 0;
}
