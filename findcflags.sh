#!/bin/sh
version=`${1:-gcc} -v 2>&1 |sed -n '/gcc version/ {s/gcc version //;p}'`
case $version in
  2.9*) echo -march=i386 -Os -fomit-frame-pointer -malign-functions=0 -malign-jumps=0 -malign-loops=0 -mpreferred-stack-boundary=2 ;;
  3.0*) echo -march=i386 -Os -fomit-frame-pointer -malign-functions=0 -malign-jumps=0 -malign-loops=0 -mpreferred-stack-boundary=2 ;;
  3*) echo -Os -fomit-frame-pointer -falign-functions=0 -falign-jumps=0 -falign-loops=0 -mpreferred-stack-boundary=2;;
  *) echo -O2 -pipe -fomit-frame-pointer ;;
esac
