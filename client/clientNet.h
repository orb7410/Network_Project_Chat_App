#ifndef __CLIENTNET_H__
#define __CLIENTNET_H__
#define BUFFER_SIZE 1000

typedef struct Client Client;

typedef enum Client_Net_Result 
{
	CLIENT_RECV_ERROR = -1,
    CLIENT_SUCCESS = 0,
    CLIENT_CONNECTION_ERROR,
    CLIENT_SEND_ERROR,
	CLIENT_UNINITIALIZED_ERROR, 		 					
	CLIENT_ALLOCATION_ERROR 			
} ClientNetResult;

/**  
 * @brief a function to creat a client.net struct
 * @param[in] _clientPort port for conection
 * @param[in] _clientIP IP for conection
 * @return a pointer to Clientnet struct
 */
Client* ClientCreate(int _clientPort, const char* _clientIP);

/**  
 * @brief a function to set *_pClient to NULL
 * @param[in] _pClient struct to be destroyed 
 * @return none
 */
void ClientDestroy(Client** _pClient);

/** 
 * @brief send data to server
 * @param[in] _client - struct client
 * @param[in] _data -  str to send
 * @param[in] _dataSize - the len of _data
 * @return  Success indicator
 * @retval  CLIENT_UNINITIALIZED_ERROR	if _client or _data or is NULL or _dataSize <=0
 * @retval  CLIENT_SUCCESS	on success
 */
ClientNetResult ClientNetRunSend(Client* _client, char* _data, int _dataSize);

/** 
 * @brief receive data from server
 * @param[in] _client - struct client
 * @param[in] _data -  empty str to receive
 * @param[in] _dataSize - the len of _data
 * @return  Success indicator 
 * @retval  CLIENT_UNINITIALIZED_ERROR	if _client or _data or is NULL or _dataSize <=0
 * @retval  CLIENT_SUCCESS	on success
 */
 ClientNetResult ClientNetRunRecv(Client* _client, char* _data, int _dataSize, int *_massegeSize);


#endif /* __CLIENTNET_H__ */
