#include <stdio.h>
#include "../libdl/_dl_int.h"

struct elf_aux {
  unsigned long type;
  unsigned long val;
};

unsigned long ph_size;
unsigned long ph_num;

unsigned long pg_size;

void (*dyn_start)();

void ldso_start(void);
extern void (*fini_entry)(void);

static void _fini_run(struct _dl_handle * tmp) {
  if (tmp->fini) tmp->fini();
  if (tmp->next) _fini_run(tmp->next);
}
static struct _dl_handle* dlh;
static void tt_fini(void) {
  DEBUG("dyn fini\n");
  _fini_run(dlh);
}

int main(int argc, char**argv, char**envp)
{
  int i;
  unsigned int *ui=(unsigned int*)envp;
  struct elf_aux *ea;

  Elf32_Phdr *ph32=0;

  /* --- */
  unsigned int o=0, s=0;
  /* --- */

  dlh = _dl_get_handle();

  fini_entry = tt_fini;

  while (*ui) ++ui;	/* while (still an env-pointer) next */
  /* now *ui points to the tailing NULL-pointer of the envirioment */

  /* print the elf_aux table */
  for (ea=(struct elf_aux*)(ui+1); ea->type; ea++)
  {
    switch (ea->type) {
    case AT_EXECFD:
    case AT_NOTELF:
      //puts("N.F.Y. This you do without ME !");
      _exit(42);
      break;

    case AT_PHDR:
      ph32=(Elf32_Phdr*)ea->val;
      DEBUG("program header @ %08lx\n",(long)ph32);
      break;
    case AT_PHENT:
      ph_size=ea->val;
      DEBUG("program header size %08lx\n",ph_size);
      break;
    case AT_PHNUM:
      ph_num=ea->val;
      DEBUG("program header # %ld\n",ph_num);
      break;

#if 0
    case AT_BASE:
      printf("base: %08x\n",ea->val);
      break;
    case AT_FLAGS:
      printf("flags %08x\n",ea->val);
      break;

    case AT_UID:
      printf(" UID: %d\n",ea->val);
      break;
    case AT_EUID:
      printf("EUID: %d\n",ea->val);
      break;
    case AT_GID:
      printf(" GID: %d\n",ea->val);
      break;
    case AT_EGID:
      printf("EGID: %d\n",ea->val);
      break;
#endif

    case AT_PAGESZ:
      pg_size=ea->val;
      DEBUG("page size %ld\n",pg_size);
      break;

    case AT_ENTRY:
      dyn_start=(void(*)())ea->val;
      DEBUG("start program  @ %08lx\n",(long)dyn_start);
      break;

#if 0
    case AT_PLATFORM:
      printf("CPU: %s\n",ea->val);
      break;
    case AT_HWCAP:
      printf("CPU capabilities: %08x\n",ea->val);
      break;
    case AT_CLKTCK:
      printf("CLK per sec %d\n", ea->val);
      break;
#endif

    default:
      DEBUG("%08lx: %08lx %08lx\n",(long)ea,(long)ea->type,(long)ea->val);
      break;
    }
  }

  for (i=0; (i<ph_num); i++)
  {
    if (ph32[i].p_type==PT_DYNAMIC)
    {
      o = ph32[i].p_vaddr;
      s = ph32[i].p_memsz;
      break;
    }
  }

  /* dynamic scan from _dl_load must be called here */

  dlh = _dl_dyn_scan(dlh,(void*)o,0);

  if (!dlh) {
    DEBUG("error in dyn_scan");
    _exit(23);
  }

  dlh->name=0;
  dlh->fini=0;

  if (dyn_start==ldso_start) {
    DEBUG("error in dyn_scan");
    _exit(42);
  }

  _dl_open_dep();

  /* all depending libs have been loaded, now start the program or die */
  if (dyn_start) return (int)dyn_start; /* found an AT_ENTRY in table -> jump to it */
  _exit(23);
}
