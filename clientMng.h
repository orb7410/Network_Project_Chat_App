#ifndef __CLIENTMNG_H__
#define __CLIENTMNG_H__
#include "protocol.h" /*protocol function decleration*/
#define TRUE 1
#define FALSE 0


typedef enum ClientManagerResult 
{
	MANAGER_SUCCESS = 0,
    MANAGER_CONNECTION_ERROR,
	MANAGER_UNINITIALIZED_ERROR, 		 					
	MANAGER_ALLOCATION_ERROR,
	MANAGER_SEND_ERROR,
	MANAGER_RECV_ERROR,
	MANAGER_MESSAGE_ERROR,
	OPEN_CHAT_ERROR,
    FIND_CHAT_ERROR,
    REMOVE_CHAT_ERROR,
    SAVE_CHAT_ERROR,
	MANAGER_CLIEN_EXIT,
	LOG_OUT			
} ClientManagerResult;

typedef struct ClientManager ClientManager;

/**  
 * @brief a function to creat a client manager  struct
 * @param[in] _clientPort port for conection
 * @param[in] _clientIP IP for conection
 * @return a pointer to ClientManager struct
 */
ClientManager* ClientManagerCreate(int _clientPort, const char* _clientIP);


/**  
 * @brief a function to set *_pClientMng to NULL
 * @param[in] ClientMng struct to be destroyed 
 * @return none
 */
void ClientManagerDestroy(ClientManager** _pClientMng);

/** 
 * @brief Run the first menu of the program, enter app
 * @param[in] _clientMng - struct to use for data
 * @return  Success indicator
 * @retval  MANAGER_SUCCESS	on success
 * @retval  MANAGER_UNINITIALIZED_ERROR	if _clientMng is NULL
 * @retval  MANAGER_CLIEN_EXIT if client exit program
 */
ClientManagerResult ClientManagerRunRegistration(ClientManager* _clientMng);

/** 
 * @brief Run the secend menu of the program, open or joine grup
 * @param[in] _clientMng - struct to use for data
 * @return  Success indicator
 * @retval  MANAGER_SUCCESS	on success
 * @retval  MANAGER_UNINITIALIZED_ERROR	if _clientMng is NULL
 * @retval  MANAGER_CLIEN_EXIT if client exit program
 */
ClientManagerResult ClientManagerRunGroup(ClientManager* _clientMng);

/**  
 * @brief a function to creat a client details struct
 * @param[in] _userName input by the client of his name 
 * @param[in] _password input by the client of his password
 * @return a pointer to UserDetail struct
 */
UserDetail* UserDetailCreate(char _userName[], char _password[]);

/**  
 * @brief a function to set *_pUserDetail to NULL
 * @param[in] _pUserDetail struct to be destroyed 
 * @return none
 */
void UserDetailDestroy(UserDetail** _pUserDetail);

/** 
 * @brief Insert save chat pid .
 * @param[in] _clientMng - struct to save data
 * @param[in] _groupName - unique name to serve as index 
 * @param[in] _windowPid - chat pid data 
 * @return  Success indicator
 * @retval  MANAGER_SUCCESS	on success
 * @retval  SAVE_CHAT_ERROR on failer to save chat pid 
 * @retval  MANAGER_UNINITIALIZED_ERROR if _clientMng or _groupName or _windowPid is NULL
 * 
 * @warning _groupName must be unique and destinct
 */
ClientManagerResult InsertChatPid(ClientManager* _clientMng, char* _groupName, void* _windowPid);

/** 
 * @brief Insert save chat pid .
 * @param[in] _clientMng - struct to save data
 * @param[in] _groupName - unique name to serve as index 
 * @param[in] _pValue - empty fointer to save the data 
 * @return  Success indicator
 * @retval  MANAGER_SUCCESS	on success
 * @retval  FIND_CHAT_ERROR on failer to find chat pid 
 * @retval  MANAGER_UNINITIALIZED_ERROR if _clientMng or _groupName or _pValue is NULL
 */
ClientManagerResult FindChatPid(ClientManager* _clientMng, char* _groupName, void** _pValue);

/** 
 * @brief Insert save chat pid .
 * @param[in] _clientMng - struct to save data
 * @param[in] _groupName - unique name to serve as index 
 * @param[in] _pValue - empty fointer to save the data
 * @param[in] _pKey - empty fointer to save the key that removed 
 * @return  Success indicator
 * @retval  MANAGER_SUCCESS	on success
 * @retval  REMOVE_CHAT_ERROR on failer to remove chat pid 
 * @retval  MANAGER_UNINITIALIZED_ERROR if _clientMng or _groupName or _pValue is NULL
 */
ClientManagerResult RemoveChatPid(ClientManager* _clientMng, char* _groupName,void** _pKey, void** _pValue);

size_t ChakNumOfGroups(ClientManager* _clientMng);


#endif /* __CLIENTMNG_H__ */

