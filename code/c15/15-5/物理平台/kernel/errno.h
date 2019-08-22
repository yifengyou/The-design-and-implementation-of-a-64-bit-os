/***************************************************
*		版权声明
*
*	本操作系统名为：MINE
*	该操作系统未经授权不得以盈利或非盈利为目的进行开发，
*	只允许个人学习以及公开交流使用
*
*	代码最终所有权及解释权归田宇所有；
*
*	本模块作者：	田宇
*	EMail:		345538255@qq.com
*
*
***************************************************/

#ifndef __ERRNO_H__
#define __ERRNO_H__

#define	E2BIG			1		/* Argument list too long or Lack of space in an output buffer or Argument is greater than the system-imposed maximum */
#define	EACCES			2		/* Permission denied */
#define	EADDRINUSE		3		/* Address in use */
#define	EADDRNOTAVAIL	4		/* Address not available */
#define	EAFNOSUPPORT	5		/* Address family not supported */
#define	EAGAIN			6		/* Resource temporarily unavailable */
#define	EALREADY		7		/* Connection already in progress */
#define	EBADF			8		/* Bad file descriptor */
#define	EBADMSG			9		/* Bad message */

#define	EBUSY			10		/* Resource busy */
#define	ECANCELED		11		/* Operation canceled */
#define	ECHILD			12		/* No child process */
#define	ECONNABORTED	13		/* Connection aborted */
#define	ECONNREFUSED	14		/* Connection refused */
#define	ECONNRESET		15		/* Connection reset */
#define	EDEADLK			16		/* Resource deadlock would occur */
#define	EDESTADDRREQ	17		/* Destination address required */
#define	EDOM			18		/* Domain error */
#define	EDQUOT			19		/* Reserved */

#define	EEXIST			20		/* File exists */
#define	EFAULT			21		/* Bad address */
#define	EFBIG			22		/* File too large */
#define	EHOSTUNREACH	23		/* Host is unreachable */
#define	EIDRM			24		/* Identifier removed */
#define	EILSEQ			25		/* Illegal byte sequence */
#define	EINPROGRESS		26		/* Operation in progress or O_NONBLOCK is set for the socket file descriptor and the connection cannot be
immediately established */
#define	EINTR			27		/* Interrupted function call */
#define	EINVAL			28		/* Invalid argument */
#define	EIO				29		/* Input/output error */

#define	EISCONN			30		/* Socket is connected */
#define	EISDIR			31		/* Is a directory */
#define	ELOOP			32		/* Symbolic link loop */
#define	EMFILE			33		/* File descriptor value too large or too many open streams */
#define	EMLINK			34		/* Too many links */
#define	EMSGSIZE		35		/* Message too large or Inappropriate message buffer length */
#define	EMULTIHOP		36		/* Reserved */
#define	ENAMETOOLONG	37		/* Filename too long */
#define	ENETDOWN		38		/* Network is down */
#define	ENETRESET		39		/* The connection was aborted by the network */

#define	ENETUNREACH		40		/* Network unreachable */
#define	ENFILE			41		/* Too many files open in system */
#define	ENOBUFS			42		/* No buffer space available */
#define	ENODATA			43		/* No message available */
#define	ENODEV			44		/* No such device */
#define	ENOENT			45		/* No such file or directory */
#define	ENOEXEC			46		/* Executable file format error */
#define	ENOLCK			47		/* No locks available */
#define	ENOLINK			48		/* Reserved */
#define	ENOMEM			49		/* Not enough space */

#define	ENOMSG			50		/* No message of the desired type */
#define	ENOPROTOOPT		51		/* Protocol not available */
#define	ENOSPC			52		/* No space left on a device */
#define	ENOSR			53		/* No STREAM resources */
#define	ENOSTR			54		/* Not a STREAM */
#define	ENOSYS			55		/* Function not implemented */
#define	ENOTCONN		56		/* Socket not connected */
#define	ENOTDIR			57		/* Not a directory */
#define	ENOTEMPTY		58		/* Directory not empty */
#define	ENOTRECOVERABLE	59		/* State not recoverable */

#define	ENOTSOCK		60		/* Not a socket */
#define	ENOTSUP			61		/* Not supported */
#define	ENOTTY			62		/* Inappropriate I/O control operation */
#define	ENXIO			63		/* No such device or address */
#define	EOPNOTSUPP		64		/* Operation not supported on socket */
#define	EOVERFLOW		65		/* Value too large to be stored in data type */
#define	EOWNERDEAD		66		/* Previous owner died */
#define	EPERM			67		/* Operation not permitted */
#define	EPIPE			68		/* Broken pipe */
#define	EPROTO			69		/* Protocol error */

#define	EPROTONOSUPPORT	70		/* Protocol not supported */
#define	EPROTOTYPE		71		/* Protocol wrong type for socket */
#define	ERANGE			72		/* Result too large or too small */
#define	EROFS			73		/* Read-only file system */
#define	ESPIPE			74		/* Invalid seek */
#define	ESRCH			75		/* No such process */
#define	ESTALE			76		/* Reserved */
#define	ETIME			77		/* STREAM ioctl( ) timeout */
#define	ETIMEDOUT		78		/* Connection timed out or Operation timed out */
#define	ETXTBSY			79		/* Text file busy */

#define	EWOULDBLOCK		80		/* Operation would block */
#define	EXDEV			81		/* Improper link */



#endif
