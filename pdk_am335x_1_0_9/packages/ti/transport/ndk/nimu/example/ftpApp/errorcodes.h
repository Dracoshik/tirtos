/**
 * errorcodes.h
 *
 */

#ifndef _ERRORCODES_H_
#define _ERRORCODES_H_

/* Base address for errorcodes (for each module) */
#define ERROR_BASE_INIT_MODULE			(0x1000)
#define ERROR_BASE_CCXML_MODULE			(0x2000)
#define ERROR_BASE_FLASH_MODULE			(0x3000)
#define ERROR_BASE_HTTPD_MODULE			(0x4000)
#define ERROR_BASE_TELNET_MODULE		(0x5000)

/* General errorcodes */
#define ERROR_SUCCESS					(0)
#define ERROR_MEMORY_ALLOC				(-1)
#define ERROR_PARAMETERS				(-2)
#define ERROR_TASK_CREATE				(-3)
#define ERROR_MUTEX_CREATE				(-4)
#define ERROR_SEMA_CREATE				(-5)
#define ERROR_NOT_INITIALIZED			(-6)
#define ERROR_ALREADY_INITIALIZED		(-7)
#define ERROR_NULL_POINTER				(-8)
#define ERROR_MAILBOX_CREATE			(-9)

#endif /* _ERRORCODES_H_ */

