#ifndef _UCONTEXT_H
#define _UCONTEXT_H

#include <sys/ucontext.h>

__BEGIN_DECLS

extern int getcontext (ucontext_t *c) __THROW;
extern int setcontext (const ucontext_t *c) __THROW;
extern int swapcontext (ucontext_t *__restrict o,
			const ucontext_t *__restrict n) __THROWNL;
extern void makecontext (ucontext_t *c, void (*__func) (void),
			 int argc, ...) __THROW;

__END_DECLS

#endif
