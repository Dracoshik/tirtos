/**
 * ftpserver.h
 *
 *   This file contains:
 *   - Server implementation
 *   .
 *   which are relevant for the ftpserver module.
 *
 */

#ifndef __FTPSERVER_H_
#define __FTPSERVER_H_

#include "ti/transport/ndk/nimu/example/ftpApp/modules.h"

#include <ti/csl/csl_error.h>
#include <ti/fs/fatfs/ff.h>

/**
 * Defines
 */
#define INPUT_STRING_SIZE				(1024)
#define BUFFER_SIZES					(1024)
#define NAME_AND_PASSWD_LEN				(64)
#define DATA_BUFFER_SIZE				(512) //1560*3


/**
 * Type definitions
 */
typedef struct io_handler_T {
    SOCKET socket;
    SOCKET data_socket;
    boolean_t running;
	char *send_buffer;
	char *receive_buffer;
	char *current_working_dir_path;		/* Cache aligned ???? */
	char *temp;		/* Cache aligned ???? */
	char *DataBuf;
	DIR DirObj;
	FILINFO FileInfo;
	FIL FileObj;
	char user[NAME_AND_PASSWD_LEN];
	char password[NAME_AND_PASSWD_LEN];
	int32_t authenticated;
} io_handler_t;

typedef struct ftp_cmd_handler_T
{
	char *cmd;
	uint8_t len;
	int32_t (*func)(io_handler_t *);
} ftp_cmd_handler_t;

int32_t ftp_filerout_read(io_handler_t *ioh, char *path);
int32_t ftp_filerout_write(io_handler_t *ioh, char *path);

int32_t ftpserver_init(void);

#endif


