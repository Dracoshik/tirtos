
 /**
  * Include files
  */
#include "ti/transport/ndk/nimu/example/ftpApp/modules.h"

#ifdef FTP_SERVER_MODULE_INCLUDED

#include <ti/csl/csl_error.h>
#include <ti/fs/fatfs/ff.h>
#include <ti/transport/ndk/nimu/example/ftpApp/ftpserver/ftpserver.h>

int32_t ftp_filerout_read(io_handler_t *ioh, char *path)
{
    uint32_t i;

    for (i=0; i<5000;i++)
    {
        send(ioh->data_socket, ioh->DataBuf, DATA_BUFFER_SIZE, 0);
        Task_yield();
    }

    return CSL_SOK;
}

int32_t ftp_filerout_write(io_handler_t *ioh, char *path)
{
    uint32_t bytesRead = 0;

    do
    {
        bytesRead = recv(ioh->data_socket, ioh->DataBuf, DATA_BUFFER_SIZE, 0);

        if (bytesRead > 0)
        {
            Task_yield();
        }
    } while (bytesRead > 0);

    return 0;
}

#endif
