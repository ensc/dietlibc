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

static _fini_run(struct _dl_handle * tmp) {
  if (tmp->fini) tmp->fini();
  if (tmp->next) _fini_run(tmp->next);
}
static struct _dl_handle * dlh;
static void tt_fini(void) {
#ifdef DEBUG
  puts("dyn fini");
#endif
  _fini_run(dlh);
}

int main(int argc, char**argv, char**envp)
{
  int i;
  unsigned int *ui=(unsigned int*)envp;
  struct elf_aux *ea;

  Elf32_Phdr *ph32;
  Elf32_Sym  *es32;

  char *str;

  /* --- */
  unsigned int n=0, o=0, s=0;
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
      puts("N.F.Y. This you do without ME !");
      _exit(42);
      break;

    case AT_PHDR:
      ph32=(Elf32_Phdr*)ea->val;
#ifdef DEBUG
      printf("program header @ %08x\n",ph32);
#endif
      break;
    case AT_PHENT:
      ph_size=ea->val;
#ifdef DEBUG
      printf("program header size %08x\n",ph_size);
#endif
      break;
    case AT_PHNUM:
      ph_num=ea->val;
#ifdef DEBUG
      printf("program header # %d\n",ph_num);
#endif
      break;

#ifdef DEBUG
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
#ifdef DEBUG
      printf("page size %d\n",pg_size);
#endif
      break;

    case AT_ENTRY:
      dyn_start=(void(*)())ea->val;
#ifdef DEBUG
      printf("start program  @ %08x\n",dyn_start);
#endif
      break;

#ifdef DEBUG
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
#ifdef DEBUG
      printf("%08x: %08x %08x\n",ea,ea->type,ea->val);
#endif
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

  dlh = _dl_dyn_scan(dlh,o,0);

  if (!dlh) {
#ifdef DEBUG
    printf("error in dyn_scan");
#endif
    _exit(23);
  }

  dlh->name=0;
  dlh->fini=0;

  if (dyn_start==ldso_start) {
#ifdef DEBUG
    printf("error in dyn_scan");
#endif
    _exit(42);
  }

  _dl_open_dep();

  /* all depending libs have been loaded, now start the program or die */
  if (dyn_start) return (int)dyn_start; /* found an AT_ENTRY in table -> jump to it */
  _exit(23);
}
