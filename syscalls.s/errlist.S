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

sys_errlist:
	.long .LC000
	.long .LC001
	.long .LC002
	.long .LC003
	.long .LC004
	.long .LC005
	.long .LC006
	.long .LC007
	.long .LC008
	.long .LC009
	.long .LC010
	.long .LC011
	.long .LC012
	.long .LC013
	.long .LC014
	.long .LC015
	.long .LC016
	.long .LC017
	.long .LC018
	.long .LC019
	.long .LC020
	.long .LC021
	.long .LC022
	.long .LC023
	.long .LC024
	.long .LC025
	.long .LC026
	.long .LC027
	.long .LC028
	.long .LC029
	.long .LC030
	.long .LC031
	.long .LC032
	.long .LC033
	.long .LC034
	.long .LC035
	.long .LC036
	.long .LC037
	.long .LC038
	.long .LC039
	.long .LC040
	.long .LC041
	.long .LC042
	.long .LC043
	.long .LC044
	.long .LC045
	.long .LC046
	.long .LC047
	.long .LC048
	.long .LC049
	.long .LC050
	.long .LC051
	.long .LC052
	.long .LC053
	.long .LC054
	.long .LC055
	.long .LC056
	.long .LC057
	.long .LC058
	.long .LC059
	.long .LC060
	.long .LC061
	.long .LC062
	.long .LC063
	.long .LC064
	.long .LC065
	.long .LC066
	.long .LC067
	.long .LC068
	.long .LC069
	.long .LC070
	.long .LC071
	.long .LC072
	.long .LC073
	.long .LC074
	.long .LC075
	.long .LC076
	.long .LC077
	.long .LC078
	.long .LC079
	.long .LC080
	.long .LC081
	.long .LC082
	.long .LC083
	.long .LC084
	.long .LC085
	.long .LC086
	.long .LC087
	.long .LC088
	.long .LC089
	.long .LC090
	.long .LC091
	.long .LC092
	.long .LC093
	.long .LC094
	.long .LC095
	.long .LC096
	.long .LC097
	.long .LC098
	.long .LC099
	.long .LC100
	.long .LC101
	.long .LC102
	.long .LC103
	.long .LC104
	.long .LC105
	.long .LC106
	.long .LC107
	.long .LC108
	.long .LC109
	.long .LC110
	.long .LC111
	.long .LC112
	.long .LC113
	.long .LC114
	.long .LC115
	.long .LC116
	.long .LC117
	.long .LC118
	.long .LC119
	.long .LC120
	.long .LC121
	.long .LC122
	.long .LC123
	.long .LC124
	.long 0

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
