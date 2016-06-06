/* Software floating-point exception handling emulation.
   Copyright (C) 2002-2016 Free Software Foundation, Inc.
   Contributed by Aldy Hernandez <aldyh@redhat.com>, 2002.
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

#include <signal.h>
#include "soft-fp.h"

#ifdef WANT_TLS
/* Thread-local to store sticky exceptions.  */
__thread int __sim_exceptions_thread __attribute__ ((nocommon));

/* By default, no exceptions should trap.  */
__thread int __sim_disabled_exceptions_thread = 0xffffffff;

__thread int __sim_round_mode_thread __attribute__ ((nocommon));
#else
/* Thread-local to store sticky exceptions.  */
int __sim_exceptions_thread __attribute__ ((nocommon));

/* By default, no exceptions should trap.  */
int __sim_disabled_exceptions_thread = 0xffffffff;

int __sim_round_mode_thread __attribute__ ((nocommon));
#endif

void
__simulate_exceptions (int x)
{
  __sim_exceptions_thread |= x;
  if (x & ~__sim_disabled_exceptions_thread)
    raise (SIGFPE);
}
