#!/bin/sh
(gcc -E - << EOF | grep -q WANT_THREAD_SAFE) || echo libpthread/pthread_*.c
#include "dietfeatures.h"
WANT_THREAD_SAFE
EOF
