#ifndef _DIETFEATURES_H
#define _DIETFEATURES_H

/* feel free to comment some of these out to reduce code size */

/* #define WANT_FLOATING_POINT_IN_PRINTF
#define WANT_FLOATING_POINT_IN_SCANF */
#define WANT_CHARACTER_CLASSES_IN_SCANF
#define WANT_NULL_PRINTF

/* #define SLASH_PROC_OK */

#define WANT_BUFFERED_STDIO

/* use errno_location instead of errno */
/* #define WANT_THREAD_SAVE */

/* make the startcode, etc. dynamic aware */
/* #undef WANT_DYNAMIC */

/* do you want smaller or faster string routines? */
/* #define WANT_FASTER_STRING_ROUTINES */

/* do you want ungetc?  makes fgetc more complex */
#define WANT_UNGETC

#define WANT_LINKER_WARNINGS

/* you need to define this if you want to run your programs with large
 * file support on kernel 2.2 or 2.4 */
#define WANT_LARGEFILE_BACKCOMPAT

/* do you want localtime(3) to read /etc/localtime?
 * Needed for daylight saving time etc. */
#define WANT_TZFILE_PARSER

/* stop uncommenting here ;-) */
#ifndef WANT_FASTER_STRING_ROUTINES
#define WANT_SMALL_STRING_ROUTINES
#endif

#endif
