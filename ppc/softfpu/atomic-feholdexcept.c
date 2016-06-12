/* Store current floating-point environment and clear exceptions for
   atomic compound assignment.  e500 version.
   Copyright (C) 2004-2016 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library.  If not, see
   <http://www.gnu.org/licenses/>.  */

#include <fenv_libc.h>
#include <stdlib.h>
#include <sys/prctl.h>

void
__atomic_feholdexcept (fenv_t *envp)
{
  fenv_union_t u;
  int r;

  /* Get the current state.  */
  r = prctl (PR_GET_FPEXC, (unsigned long)&u.l[0]);
  if (r < 0)
    abort ();

  u.l[1] = fegetenv_register ();
  *envp = u.fenv;

  /* Clear everything except for the rounding mode and trapping to the
     kernel.  */
  u.l[0] &= ~(PR_FP_EXC_DIV
	      | PR_FP_EXC_OVF
	      | PR_FP_EXC_UND
	      | PR_FP_EXC_RES
	      | PR_FP_EXC_INV);
  u.l[1] &= SPEFSCR_FRMC | (SPEFSCR_ALL_EXCEPT_ENABLE & ~SPEFSCR_FINXE);

  /* Put the new state in effect.  */
  fesetenv_register (u.l[1]);
  r = prctl (PR_SET_FPEXC, u.l[0] | PR_FP_EXC_SW_ENABLE);
  if (r < 0)
    abort ();
}
