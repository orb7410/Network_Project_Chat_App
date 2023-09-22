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

typedef int (*CommunicateFunc)(int _sock, Server* _server);

struct Fdset
{
    fd_set m_master;
    fd_set m_tempMaster;
    int m_activity;
};

struct Server
{
    NewClient m_newClientFunc;
    GotMessage m_gotMessageFunc;
    CloseClientFunc m_closeClientFunc;
    Failed m_failFunc;
    StopRun m_stopRunFunc;
    Fdset m_fdsetStruct;
    List* m_clientsList;
    char m_buffer[BUFFER_SIZE];
    int m_bufferSize;
    int m_flag;
    int m_removerFlag;
    int m_sock;
    void* m_context;
};

static int CheakNewClient(Server* _server);
static int CheakCurrentClient(Server* _server);
static int ServerSocket();
static int ServerAccept(int _Sock);
static int ServerRecv(int _clientSock);
static int ServerInitialization();
static int ServerSetSockeOPT(int _sock);
static int ServerBind(int _sock);
static int ServerListen(int _sock);
static int NoBlocking(int _sock);
static void CloseClient(void* _client);
static int ServerAction(void* _clientSock, void* _context);
static void CloseClient(void* _client);

Server* ServerCreate(NewClient _newClientFunc, GotMessage _gotMessageFunc, CloseClientFunc _closeClientFunc, Failed _failFunc, void* _context)
{
    Server* pServer;
    Fdset* pFdset;
    List* pList;
    int sock;
    if (NULL == _gotMessageFunc)
    {
        return NULL;
    }
    if(NULL == (pServer = (Server*)malloc(sizeof(Server) * 1)))
    {
        return NULL;
    }
    if(NULL == (pList = ListCreate()))
    {
        free(pServer);
        return NULL;
    }
    
    if((sock = ServerInitialization() )== SERVER_FAIL)
    {
        free(pFdset);
        free(pServer);
        ListDestroy(&pList, NULL);
        return NULL;
    }
    FD_ZERO(&(pServer->m_fdsetStruct.m_master));
    FD_SET(sock,&(pServer->m_fdsetStruct.m_master));
    
    pServer->m_newClientFunc = _newClientFunc;
    pServer->m_gotMessageFunc = _gotMessageFunc;
    pServer->m_closeClientFunc = _closeClientFunc;
    pServer->m_failFunc = _failFunc; 
    pServer->m_clientsList = pList;
    pServer->m_flag = 1; 
    pServer->m_sock = sock; 
    pServer->m_bufferSize = BUFFER_SIZE;
    pServer->m_context = _context;

    return pServer;
}
/*........................................*/
void ServerDestroy(Server** _server)
{
    if(NULL == _server || NULL == *_server)
    {
        return;
    }
    ListDestroy(&((*_server) -> m_clientsList), CloseClient);
    close((*_server)->m_sock);
    free((*_server)-> m_clientsList);
    free(*_server);
    *_server = NULL;
}
/*...............................................*/
Server_Result ServerRun(Server* _server)
{
    Fdset fdData;
    int counter = 0;
    if(NULL == _server)
    {
        return SERVER_UNINITIALIZED_ERROR;
    }
    while(_server->m_flag)
    {
        (_server->m_fdsetStruct.m_tempMaster) = (_server -> m_fdsetStruct.m_master);
        (_server -> m_fdsetStruct.m_activity) =( select(MAX_FD, &(_server->m_fdsetStruct.m_tempMaster), NULL, NULL , NULL));
        if((_server->m_fdsetStruct.m_activity < 0) && (errno != EINTR))
        {
            perror("select error");
            return SERVER_FAIL;
        }
        if(FD_ISSET(_server->m_sock, &(_server->m_fdsetStruct.m_tempMaster)))
        {
            if(CheakNewClient(_server) < 0)
            {
                return SERVER_FAIL;
            }
        }
        if(_server->m_fdsetStruct.m_activity > 0)
        {
            CheakCurrentClient(_server);
        }
    }
    return SERVER_SUCCESS;
}
/*...............................................*/
int ServerSend(int _sock, char* _message, int _massageSize)
{
    int sendBytes;
    if(_sock <= 0 || _message == NULL || _massageSize <= 0)
    {
        return -1;
    }
    sendBytes = send(_sock, _message, _massageSize, 0);
    return sendBytes;
}
/*...............................................*/
int StopRunServer(Server* _server)
{
    if(_server == NULL)
    {
        return SERVER_UNINITIALIZED_ERROR;
    }
    _server->m_flag = 0;
    _server->m_fdsetStruct.m_activity = 0;
    return SERVER_SUCCESS;
}


/*..................HELP FUNC.....................*/
/*...............................................*/
static void CloseClient(void* _client)
{
    close(*(int*)_client);
}
/*...........................................*/
static int ServerAction(void* _clientSock, void* _context)
{
    Server* pServer = ((Server*)_context);
    char buffer[BUFFER_SIZE];
    int bufferSize = sizeof(buffer);
    int readBytes;
    pServer->m_removerFlag = 0;
    if(!FD_ISSET(*(int*) _clientSock, &(((Server*)_context)->m_fdsetStruct.m_tempMaster)))
    {
        return 1; 
    }
    --(pServer->m_fdsetStruct.m_activity);
    if((readBytes = recv(*(int*)_clientSock, buffer, bufferSize, 0)) > 0)
    {
        if(pServer -> m_gotMessageFunc(((int*)_clientSock), buffer, readBytes,pServer->m_context) == NULL)
        {
            /*printf("send massage faild \n");*/
        }
        printf("%s \n", buffer);
        return 1;
    }

    if(readBytes <= 0 || pServer-> m_fdsetStruct.m_activity == 0)
    {
        if(readBytes <= 0)
        {
            pServer->m_removerFlag = 1;
        }
        return 0;
    }
    return 1;
}
/*......................................................*/
static int ServerInitialization()
{
   int sock = ServerSocket();
   if(sock < 0)
   {
        return SERVER_FAIL;
   }
   if(NoBlocking(sock) != SERVER_SUCCESS)
   {
        close(sock);
        return SERVER_FAIL;
   }
   if(ServerSetSockeOPT(sock) != SERVER_SUCCESS)
   {
        close(sock);
        return SERVER_FAIL;
   }
   if(ServerBind(sock) != SERVER_SUCCESS)
   {
        close(sock);
        return SERVER_FAIL;
   }
   if(ServerListen(sock) != SERVER_SUCCESS)
   {
        close(sock);
        return SERVER_FAIL;
   }
   return sock;
}
/*.............................................*/
static int ServerSetSockeOPT(int _sock) /*relise a ocupited port to an imidiat use*/
{
    int optval =1;
    if (setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
    {
        perror("reuse failed");
        return SERVER_FAIL;
    }
    return SERVER_SUCCESS;
}
/*.............................................*/
static int ServerBind(int _sock)
{
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(SERVER_PORT);
    if(bind(_sock, (struct sockaddr*) &sin, sizeof(sin)) < 0) 
    {
        perror("bind failed");
        return SERVER_FAIL;
    }
    return SERVER_SUCCESS;
}
/*.............................................*/
static int ServerListen(int _sock) /*make the socket avilabel for clients*/
{
    if(listen(_sock, BACK_LOG) < 0)
    {
        perror("listen failed");
        return SERVER_FAIL;
    }
    return SERVER_SUCCESS;
}
/*.............................................*/
static int NoBlocking(int _sock) 
{
    int flags;
    if(-1 == (flags = fcntl(_sock, F_GETFL)))
    {
        perror("error at fcntl. F_GETFL.");
        return SERVER_FAIL;
    }
    if(-1 == fcntl(_sock, F_SETFL, flags | O_NONBLOCK))
    {
        perror("error at fcntl. F_SETFL.");
        return SERVER_FAIL;
    }
    return SERVER_SUCCESS;
}
/*......................................*/
static int CheakNewClient(Server* _server)
{
    int* clientSock;
    if(NULL == (clientSock = (int*)malloc(sizeof(int))))
    {
        return SERVER_FAIL;
    }
    if ((*clientSock = ServerAccept(_server->m_sock)) < 0)
    {
        if(errno != EAGAIN && errno != EWOULDBLOCK)
        {
            perror("accept failed");
            return SERVER_FAIL;
        }
    }
    if(*clientSock == SERVER_FAIL)
    {
        free(clientSock);
        return SERVER_FAIL;
    }
    if(ListPushTail(_server->m_clientsList, clientSock) != LIST_SUCCESS)
    {
        close(*clientSock);
        free(clientSock);
        return SERVER_FAIL;
    }
    FD_SET(*clientSock,&(_server->m_fdsetStruct.m_master));
    return *clientSock;
}
/*.....................................................*/
static int CheakCurrentClient(Server* _server)
{
    void* clientToRemove;
    ListItr runner, end, temp;
    runner = ListItrBegin(_server-> m_clientsList);
    end = ListItrEnd(_server-> m_clientsList);
    while (runner != end)
    {
        runner = ListItrForEach(runner, end, ServerAction, _server);
        if(runner != end && (_server->m_removerFlag == 1))
        {
            temp = runner;
            runner = ListItrNext(runner);
            clientToRemove = ListItrRemove(temp);
            FD_CLR(*(int*)clientToRemove, &(_server-> m_fdsetStruct.m_master));
            close((*(int*)clientToRemove));
            free(clientToRemove);
        }
        if(_server-> m_fdsetStruct.m_activity == 0)
        {
            break;
        }
    }
    return SERVER_SUCCESS;
}
/*................................................*/
static int ServerSocket()
{
    int sock;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("socket failed");
    }
    return sock;
}
/*................................................*/
static int ServerAccept(int _Sock)
{
    struct sockaddr_in clientSin;
    socklen_t clientSinSize = sizeof(clientSin);
    int clientSock;
    clientSock = accept(_Sock, (struct sockaddr *) &clientSin, &clientSinSize);
    if(clientSock < 0)
    {
        return SERVER_FAIL; 
    }
    return clientSock;
}
/*.............................................*/
static int ServerRecv(int _clientSock)
{
    char buffer[BUFFER_SIZE];
    int bufferSize = sizeof(buffer);
    int readBytes = recv(_clientSock, buffer, bufferSize, 0);
    if(readBytes < 0)
    {
        return SERVER_FAIL;
    }
    else if(readBytes == 0)
    {
        return SERVER_FAIL;
    }

    printf("%s \n", buffer);
    return SERVER_SUCCESS;
}

