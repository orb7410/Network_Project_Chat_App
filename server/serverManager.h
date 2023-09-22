#ifndef __SERVERMANAGER_H__
#define __SERVERMANAGER_H__
#define MAX_USERS 1000

typedef struct ServerManager ServerManager;

typedef enum ServerMenagerResult {
	SERVER_MENAGER_SUCCESS = 0,
    SERVER_MENAGER_FAIL,
	SERVER_MENAGER_SEND_FAIL,
	SERVER_MENAGER_UNINITIALIZED_ERROR, 		/*Uninitialized  error*/
	SERVER_MENAGER_ALLOCATION_ERROR			/* Allocation error*/
} ServerMenagerResult;

/** 
 * @brief create a new ServerManager object
 * @param[in]  none  
 * @return NULL  if any of the allocations or initializations fail, 
 *               or pointer to the newly created ServerManager object in seccess.
 */
ServerManager* CreateServerManger(void);


/** 
 * @brief destroy serverManager object
 * @param[in] _serverManager -double pointer to ServerManager object 
 * @return none
 */
void ServerMangerDestroy( ServerManager** _serverManager);

/** 
 * @brief starts the server and runs it until an error occurs or the server is stopped.
 * @param[in] _serverManager - pointer to a ServerManager object
 * @return SERVER_MENAGER_UNINITIALIZED_ERROR - if _serverManager NULL
 *         SERVER_MENAGER_FAIL -if function run in serverNet return SERVER_FAIL
 *         SERVER_MENAGER_UNINITIALIZED_ERROR- if function run in serverNet return SERVER_UNINITIALIZED_ERROR
 *         SERVER_MENAGER_SUCCESS- if function run in serverNet seccess
 */
ServerMenagerResult ServerMenagerRun(ServerManager* _serverManager);

void* ServerMangerGotMessage(int* _clientId, char* _buffer, int _massageSize, void* _context);

#endif /* __SERVERMANAGER_H__ */
