#include <ucontext.h>
#include <stdarg.h>

extern void __setcontext_trampoline();

void makecontext(ucontext_t* c, void (*func)(void), int argc, ...) {
  size_t* sp;
  va_list v;
  int i;
#ifdef __hppa__
#define PUSH(val) *++sp=(val);
#else
#define PUSH(val) *--sp=(val);
#endif
  /* we are called like this:

    if (getcontext(&uctx_func1) == -1)
        handle_error("getcontext");
    uctx_func1.uc_stack.ss_sp = func1_stack;
    uctx_func1.uc_stack.ss_size = sizeof(func1_stack);
    uctx_func1.uc_link = &uctx_main;
    makecontext(&uctx_func1, func1, 0);
  */

  /* first order of business: set up the stack */
  /* the &-16 aligns it to 16 bytes */
  sp=(size_t*)((size_t)(c->uc_stack.ss_sp + c->uc_stack.ss_size)&-16);
  /* note that if the provided stack is to small, we will just crash in here */
  va_start(v,argc);
#ifdef __i386__
  /* if there are no args, we need to make sure there is space on the
   * stack for the trampoline to pretend uc_link was passed as stack
   * argument */
  if (!argc) PUSH(0);
  /* we set up the context so we jump to it directly, and we leave
   * __setcontext_trampoline as return address on the stack. */
  /* __setcontext_trampoline needs to know the c->uc_link, so we put
   * that into ebx, which is callee-saved */
  c->uc_mcontext.ebx=(size_t)c->uc_link;
  sp-=argc;
  for (i=0; i<argc; ++i)
    sp[i]=va_arg(v,size_t);
  PUSH((size_t)__setcontext_trampoline);
  c->uc_mcontext.esp=(size_t)sp;
  c->uc_mcontext.eip=(size_t)func;
#elif defined __x86_64__
  /* __setcontext_trampoline needs to know the c->uc_link, so we put
   * that into ebx, which is callee-saved */
  c->uc_mcontext.rbx=(size_t)c->uc_link;
  for (i=0; i<argc; ++i) {
    switch(i) {
    case 0: c->uc_mcontext.rdi=va_arg(v,size_t); break;
    case 1: c->uc_mcontext.rsi=va_arg(v,size_t); break;
    case 2: c->uc_mcontext.rdx=va_arg(v,size_t); break;
    case 3: c->uc_mcontext.rcx=va_arg(v,size_t); break;
    case 4: c->uc_mcontext.r8=va_arg(v,size_t); break;
    case 5: c->uc_mcontext.r9=va_arg(v,size_t); break;
    default: /* the rest goes on the stack */
      PUSH(va_arg(v,size_t));
    }
  }
  PUSH((size_t)__setcontext_trampoline);
  c->uc_mcontext.rsp=(size_t)sp;
  c->uc_mcontext.rip=(size_t)func;
#else
#warning platform not supported yet in makecontext
#endif
  va_end(v);
}
