#ifndef __SERVER_H__
#define __SERVER_H__

#define SERVER_PORT 1025
#define BACK_LOG 500 /*Queue size*/
#define BUFFER_SIZE 1000
#define SERVER_PORT 1025
#define SERVER_NO_CLIENT 2
#define MAX_FD 1024

typedef struct Fdset Fdset;
typedef struct Server Server;

typedef enum Server_Result {
	SERVER_SUCCESS = 0,
    SERVER_FAIL,
	SERVER_UNINITIALIZED_ERROR, 		/*Uninitialized  error*/
	SERVER_ALLOCATION_ERROR 			/* Allocation error*/
} Server_Result;



/** 
 * @brief recive a message from client and option to respond.
 * @param[in] _clientID - client number- send it to send message function to respond
 * @param[in] _message - the message from the client
 * @param[in] _massageSize - message size- limit to BUFFER_SIZE
 * @param[in] _context - any usefulcontext to operate recive and responde
 * @return none/NULL for any error
 */
typedef void* (*GotMessage)(int* _clientId, char* _message, int _massageSize, void* _context);

/** 
 * @brief 
 * @param[in] _element - 
 * @param[in] _context - 
 * @return 
 */
typedef int (*CloseClientFunc)(void* _element, void* _context);

/** 
 * @brief 
 * @param[in] _element - 
 * @param[in] _context - 
 * @return 
 */
typedef int (*Failed)(void* _element, void* _context);

/** 
 * @brief 
 * @param[in] _element - 
 * @param[in] _context - 
 * @return 
 */
typedef int (*StopRun)(void* _element, void* _context);

/** 
 * @brief 
 * @param[in] _element - 
 * @param[in] _context - 
 * @return 
 */
typedef int (*NewClient)(void* _element, void* _context);

/** 
 * @brief create new server 
 * @param[in] _newClientFunc - function
 * @param[in] _gotMessageFunc -function that recive and send massge
 * @param[in] _closeClientFunc - function
 * @param[in] _failFunc - function
 * @param[in] _stopRunFunc - function
 * @return pointer to the new server / NULL for any fail
 */
Server* ServerCreate(NewClient _newClientFunc, GotMessage _gotMessageFunc, CloseClientFunc _closeClientFunc, Failed _failFunc, void* _context);

/** 
 * @brief destroy server
 * @param[in] _server - double pointer to server 
 * @return none
 */
void ServerDestroy(Server** _server);

/** 
 * @brief run server 
 * @param[in] _server -  pointer to created server 
 * @return 
 */
Server_Result ServerRun(Server* _server);

/** 
 * @brief send message to client 
 * @param[in] _sock - 
 * @param[in] _message - 
 * @param[in] _massageSize - 
 * @return 
 */
int ServerSend(int _sock, char* _message, int _massageSize);

/** 
 * @brief stop server running
 * @param[in] _server -  pointer to created server 
 * @return 
 */
int StopRunServer(Server* _server);


#endif /* __SERVER_H__ */