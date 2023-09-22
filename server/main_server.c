#include <stdio.h> 
#include <string.h>
#include <errno.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h> 
#include <sys/time.h>
#include "list.h" /*DLList funcions decleration*/
#include "listItr.h" /*ItrList funcions decleration*/
#include "listitrfun.h" /*ItrListForEch funcions decleration*/ 
#include "server.h"



void* GotMesage(int* _clientId, char* _message, int _massageSize, void* _context)
{
    char data[20] = "message from server";
    int dataSize = sizeof(data);

    if(ServerSend(*_clientId, data, dataSize) < 0)
    {
        return NULL;
    }
   
    return _message;
}


int main()
{
    Server* pServer = ServerCreate(NULL,GotMesage, NULL, NULL);
    if(pServer == NULL)
    {
        perror("socket initialization failed");
        return -1;
    }
    if(ServerRun(pServer) == SERVER_FAIL)
    {
        return SERVER_FAIL;        
    }
   
    ServerDestroy(&pServer);
    return 0;
}

