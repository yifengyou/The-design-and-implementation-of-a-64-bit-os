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

#ifndef __FCNTL_H__

#define __FCNTL_H__

#define	O_RDONLY	00000000	/* Open read-only */
#define	O_WRONLY	00000001	/* Open write-only */
#define	O_RDWR		00000002	/* Open read/write */
#define	O_ACCMODE	00000003	/* Mask for file access modes */

#define	O_CREAT		00000100	/* Create file if it does not exist */
#define	O_EXCL		00000200	/* Fail if file already exists */
#define	O_NOCTTY	00000400	/* Do not assign controlling terminal */
#define	O_TRUNC		00001000	/* If the file exists and is a regular file, and the file is successfully opened O_RDWR or O_WRONLY, its length shall be truncated to 0 */
#define	O_APPEND	00002000	/* the file offset shall be set to the end of the file */
#define	O_NONBLOCK	00004000	/* Non-blocking I/O mode */

#define	O_EXEC		00010000	/* Open for execute only (non-directory files) */
#define	O_SEARCH	00020000	/* Open directory for search only */
#define	O_DIRECTORY	00040000	/* must be a directory */
#define	O_NOFOLLOW	00100000	/* Do not follow symbolic links */

int open(const char *path, int oflag);

#endif
