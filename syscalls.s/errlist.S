#include <endian.h>

#
#  My gcc tries to align some of the strings if written the code in C. This costs a lot of bytes.
#

#ifdef __DYN_LIB
.section	.data
#else
.section	.rodata
#endif

.globl sys_errlist
	.align 4
#ifdef __arm__
	.type	 sys_errlist,object
	.type	 sys_nerr,object
#else
	.type	 sys_errlist,@object
	.type	 sys_nerr,@object
#endif

#if (__WORDSIZE == 64)
#define deflong .quad
#else
#define deflong .long
#endif

sys_errlist:
	deflong .LC000
	deflong .LC001
	deflong .LC002
	deflong .LC003
	deflong .LC004
	deflong .LC005
	deflong .LC006
	deflong .LC007
	deflong .LC008
	deflong .LC009
	deflong .LC010
	deflong .LC011
	deflong .LC012
	deflong .LC013
	deflong .LC014
	deflong .LC015
	deflong .LC016
	deflong .LC017
	deflong .LC018
	deflong .LC019
	deflong .LC020
	deflong .LC021
	deflong .LC022
	deflong .LC023
	deflong .LC024
	deflong .LC025
	deflong .LC026
	deflong .LC027
	deflong .LC028
	deflong .LC029
	deflong .LC030
	deflong .LC031
	deflong .LC032
	deflong .LC033
	deflong .LC034
	deflong .LC035
	deflong .LC036
	deflong .LC037
	deflong .LC038
	deflong .LC039
	deflong .LC040
	deflong .LC041
	deflong .LC042
	deflong .LC043
	deflong .LC044
	deflong .LC045
	deflong .LC046
	deflong .LC047
	deflong .LC048
	deflong .LC049
	deflong .LC050
	deflong .LC051
	deflong .LC052
	deflong .LC053
	deflong .LC054
	deflong .LC055
	deflong .LC056
	deflong .LC057
	deflong .LC058
	deflong .LC059
	deflong .LC060
	deflong .LC061
	deflong .LC062
	deflong .LC063
	deflong .LC064
	deflong .LC065
	deflong .LC066
	deflong .LC067
	deflong .LC068
	deflong .LC069
	deflong .LC070
	deflong .LC071
	deflong .LC072
	deflong .LC073
	deflong .LC074
	deflong .LC075
	deflong .LC076
	deflong .LC077
	deflong .LC078
	deflong .LC079
	deflong .LC080
	deflong .LC081
	deflong .LC082
	deflong .LC083
	deflong .LC084
	deflong .LC085
	deflong .LC086
	deflong .LC087
	deflong .LC088
	deflong .LC089
	deflong .LC090
	deflong .LC091
	deflong .LC092
	deflong .LC093
	deflong .LC094
	deflong .LC095
	deflong .LC096
	deflong .LC097
	deflong .LC098
	deflong .LC099
	deflong .LC100
	deflong .LC101
	deflong .LC102
	deflong .LC103
	deflong .LC104
	deflong .LC105
	deflong .LC106
	deflong .LC107
	deflong .LC108
	deflong .LC109
	deflong .LC110
	deflong .LC111
	deflong .LC112
	deflong .LC113
	deflong .LC114
	deflong .LC115
	deflong .LC116
	deflong .LC117
	deflong .LC118
	deflong .LC119
	deflong .LC120
	deflong .LC121
	deflong .LC122
	deflong .LC123
	deflong .LC124
	deflong 0

.globl sys_nerr
	.align 4
	.size	 sys_nerr,4

sys_nerr:
	.long 125


#ifdef __DYN_LIB
.section	.rodata
#endif

.LC000:	.string	"Success"
.LC001:	.string	"Operation not permitted"
.LC002:	.string	"No such file or directory"
.LC003:	.string	"No such process"
.LC004:	.string	"Interrupted system call"
.LC005:	.string	"I/O error"
.LC006:	.string	"No such device or address"
.LC007:	.string	"Arg list too long"
.LC008:	.string	"Exec format error"
.LC009:	.string	"Bad file number"
.LC010:	.string	"No child processes"
.LC011:	.string	"Try again"
.LC012:	.string	"Out of memory"
.LC013:	.string	"Permission denied"
.LC014:	.string	"Bad address"
.LC015:	.string	"Block device required"
.LC016:	.string	"Device or resource busy"
.LC017:	.string	"File exists"
.LC018:	.string	"Cross-device link"
.LC019:	.string	"No such device"
.LC020:	.string	"Not a directory"
.LC021:	.string	"Is a directory"
.LC022:	.string	"Invalid argument"
.LC023:	.string	"File table overflow"
.LC024:	.string	"Too many open files"
.LC025:	.string	"Not a typewriter"
.LC026:	.string	"Text file busy"
.LC027:	.string	"File too large"
.LC028:	.string	"No space left on device"
.LC029:	.string	"Illegal seek"
.LC030:	.string	"Read-only file system"
.LC031:	.string	"Too many links"
.LC032:	.string	"Broken pipe"
.LC033:	.string	"Math argument out of domain of func"
.LC034:	.string	"Math result not representable"
.LC035:	.string	"Resource deadlock would occur"
.LC036:	.string	"File name too long"
.LC037:	.string	"No record locks available"
.LC038:	.string	"Function not implemented"
.LC039:	.string	"Directory not empty"
.LC040:	.string	"Too many symbolic links encountered"
.LC041:	.string	"Operation would block"
.LC042:	.string	"No message of desired type"
.LC043:	.string	"Identifier removed"
.LC044:	.string	"Channel number out of range"
.LC045:	.string	"Level 2 not synchronized"
.LC046:	.string	"Level 3 halted"
.LC047:	.string	"Level 3 reset"
.LC048:	.string	"Link number out of range"
.LC049:	.string	"Protocol driver not attached"
.LC050:	.string	"No CSI structure available"
.LC051:	.string	"Level 2 halted"
.LC052:	.string	"Invalid exchange"
.LC053:	.string	"Invalid request descriptor"
.LC054:	.string	"Exchange full"
.LC055:	.string	"No anode"
.LC056:	.string	"Invalid request code"
.LC057:	.string	"Invalid slot"
.LC058:	.string	"File locking deadlock error"
.LC059:	.string	"Bad font file format"
.LC060:	.string	"Device not a stream"
.LC061:	.string	"No data available"
.LC062:	.string	"Timer expired"
.LC063:	.string	"Out of streams resources"
.LC064:	.string	"Machine is not on the network"
.LC065:	.string	"Package not installed"
.LC066:	.string	"Object is remote"
.LC067:	.string	"Link has been severed"
.LC068:	.string	"Advertise error"
.LC069:	.string	"Srmount error"
.LC070:	.string	"Communication error on send"
.LC071:	.string	"Protocol error"
.LC072:	.string	"Multihop attempted"
.LC073:	.string	"RFS specific error"
.LC074:	.string	"Not a data message"
.LC075:	.string	"Value too large for defined data type"
.LC076:	.string	"Name not unique on network"
.LC077:	.string	"File descriptor in bad state"
.LC078:	.string	"Remote address changed"
.LC079:	.string	"Can not access a needed shared library"
.LC080:	.string	"Accessing a corrupted shared library"
.LC081:	.string	".lib section in a.out corrupted"
.LC082:	.string	"Attempting to link in too many shared libraries"
.LC083:	.string	"Cannot exec a shared library directly"
.LC084:	.string	"Illegal byte sequence"
.LC085:	.string	"Interrupted system call should be restarted"
.LC086:	.string	"Streams pipe error"
.LC087:	.string	"Too many users"
.LC088:	.string	"Socket operation on non-socket"
.LC089:	.string	"Destination address required"
.LC090:	.string	"Message too long"
.LC091:	.string	"Protocol wrong type for socket"
.LC092:	.string	"Protocol not available"
.LC093:	.string	"Protocol not supported"
.LC094:	.string	"Socket type not supported"
.LC095:	.string	"Operation not supported on transport endpoint"
.LC096:	.string	"Protocol family not supported"
.LC097:	.string	"Address family not supported by protocol"
.LC098:	.string	"Address already in use"
.LC099:	.string	"Cannot assign requested address"
.LC100:	.string	"Network is down"
.LC101:	.string	"Network is unreachable"
.LC102:	.string	"Network dropped connection because of reset"
.LC103:	.string	"Software caused connection abort"
.LC104:	.string	"Connection reset by peer"
.LC105:	.string	"No buffer space available"
.LC106:	.string	"Transport endpoint is already connected"
.LC107:	.string	"Transport endpoint is not connected"
.LC108:	.string	"Cannot send after transport endpoint shutdown"
.LC109:	.string	"Too many references: cannot splice"
.LC110:	.string	"Connection timed out"
.LC111:	.string	"Connection refused"
.LC112:	.string	"Host is down"
.LC113:	.string	"No route to host"
.LC114:	.string	"Operation already in progress"
.LC115:	.string	"Operation now in progress"
.LC116:	.string	"Stale NFS file handle"
.LC117:	.string	"Structure needs cleaning"
.LC118:	.string	"Not a XENIX named type file"
.LC119:	.string	"No XENIX semaphores available"
.LC120:	.string	"Is a named type file"
.LC121:	.string	"Remote I/O error"
.LC122:	.string	"Quota exceeded"
.LC123:	.string	"No medium found"
.LC124:	.string	"Wrong medium type"
