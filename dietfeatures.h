
/* feel free to comment some of these out to reduce code size */

#if 1

#define WANT_FLOATING_POINT_IN_PRINTF
#define WANT_FLOATING_POINT_IN_SCANF
#define WANT_NULL_PRINTF

/* #define SLASH_PROC_OK */

#define WANT_BUFFERED_STDIO

/* use errno_location instead of errno */
/* #define WANT_THREAD_SAVE */

/* make the startcode, etc. dynamic aware */
/* #undef WANT_DYNAMIC */

/* do you want smaller or faster string routines? */
/* #define WANT_FASTER_STRING_ROUTINES */

#endif

/* stop uncommenting here ;-) */
#ifndef WANT_FASTER_STRING_ROUTINES
#define WANT_SMALL_STRING_ROUTINES
#endif
