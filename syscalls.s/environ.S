.global environ
#ifdef __alpha__
.comm   environ,8
#else
.comm   environ,4
#endif
