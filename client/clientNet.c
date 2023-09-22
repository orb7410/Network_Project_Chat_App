#include <stdio.h> /*printf, NULL*/
#include <string.h>
#include <errno.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h> /*malloc*/
#include "clientNet.h"/*client functions decleration*/

static int ClientSocket();
static int ClientConnect(Client* _client);
static int ClientSend(Client* _client, char* _data, int _dataSize);
static int ClientRecv(Client* _client, char* _data, int _dataSize);

struct Client
{
    int           m_sock;
    int           m_clientPort;
    const char*   m_clientIP;
};

Client* ClientCreate(int _clientPort, const char* _clientIP)
{
    Client* ptrClient;
    int sock;
	if (NULL == _clientIP || _clientPort <= 0)
	{
		return NULL;
	}
    if(NULL == (ptrClient = (Client*)malloc(sizeof(Client))))
	{
		return NULL;
	}
    sock = ClientSocket();
    if(sock < 0)
    {
        free(ptrClient);
        return NULL;
    }
    ptrClient -> m_sock = sock;
    ptrClient -> m_clientPort = _clientPort;
    ptrClient -> m_clientIP = _clientIP;
    if(ClientConnect(ptrClient) != CLIENT_SUCCESS)
    {
        free(ptrClient);
        return NULL;
    }
    return ptrClient;
}

void ClientDestroy(Client** _pClient)
{
	if (NULL == _pClient || NULL == *_pClient )
	{
		return;
	}
    close((*_pClient) -> m_sock);
	free(*_pClient);
	*_pClient = NULL;
}

ClientNetResult ClientNetRunSend(Client* _client, char* _data, int _dataSize)
{
    if (NULL == _client || NULL == _data || _dataSize <=0 )
	{		 					
		return CLIENT_UNINITIALIZED_ERROR;
    }
    if(ClientSend(_client, _data, _dataSize) != CLIENT_SUCCESS)
    {
        close(_client -> m_sock);
    }
    return CLIENT_SUCCESS;
}

ClientNetResult ClientNetRunRecv(Client* _client, char* _data, int _dataSize, int* _massegeSize)
{
    int readBytes;
    if (NULL == _client)
	{		 					
		return CLIENT_UNINITIALIZED_ERROR;
    }
    readBytes = ClientRecv(_client, _data, _dataSize);
    if(readBytes == CLIENT_RECV_ERROR)
    {
        close(_client -> m_sock);
    }
    *_massegeSize = readBytes;
    return CLIENT_SUCCESS;
}

/***************************************************************************************/
static int ClientSocket()
{
    int sock;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("socket failed");
    }
    return sock;
}

static int ClientConnect(Client* _client)
{
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(_client -> m_clientIP);
    sin.sin_port = htons(_client -> m_clientPort);
    if(connect(_client -> m_sock, (struct sockaddr*) &sin, sizeof(sin)) < 0)
    {
        perror("connection failed");
        return CLIENT_CONNECTION_ERROR;
    }
    return CLIENT_SUCCESS;
}

static int ClientSend(Client* _client, char* _data, int _dataSize)
{
    int sendBytes;
    sendBytes = send(_client -> m_sock, _data, _dataSize, MSG_NOSIGNAL); /*print broken pipe insted of break the run*/
    if(sendBytes < 0)
    {
        perror("send failed");
        return CLIENT_SEND_ERROR;
    }
    return CLIENT_SUCCESS;
}

static int ClientRecv(Client* _client, char* _data, int _dataSize)
{
    int readBytes = recv(_client -> m_sock, _data , _dataSize, 0);
    if(readBytes == 0)
    {
        return CLIENT_RECV_ERROR;
    }
    else if(readBytes < 0)
    {
        perror("recv failed");
        return CLIENT_RECV_ERROR;
    }
    return readBytes;
}
