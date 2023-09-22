#ifndef __GROUPMANAGER_H__
#define __GROUPMANAGER_H__
#include <stddef.h>
#include "vector.h"
#define TRUE 1
#define FALSE 0
#define PORT "1025"

typedef enum GroupManagerResultCode {
	GROUPMANAGER_SUCCESS = 0,
	GROUPMANAGER_NOT_INITIALIZED,
    GROUPMANAGER_ALLOCATION_FAILED,
    GROUPMANAGER_FAIL,
    GROUPMANAGER_DUPLICATION,
    GROUPMANAGER_NOT_FOUND,
    GROUPMANAGER_CLOSE_GROUP_FAIL,
    GROUPMANAGER_IS_EMPTY
} GroupManagerResultCode;

typedef struct GroupManager GroupManager;

/** 
 * @brief Create a new GroupManager with given maximum number of groups.
 * @param[in] _maxUsers - Expected max number of groups. 
 * @return newly created GroupManager or null on failure
 */
GroupManager* GroupManagerCreate(size_t _maxNumOfGroups);
/**
 * @brief destroy GroupManager and set *_groupManager to null
 * @param[in] _userManager : GroupManager to be destroyed
 */
void GroupManagerDestroy (GroupManager** _groupManager);
/** 
 * @brief Add new group. The IP and the PORT numbers are been copying to the input values.
 * @param[in] _groupManager
 * @param[in] _groupName - the group name
 * @param[in] _port - a char array to hold the given port
 * @param[in] _addressIP  - a char array to hold the given IP address
 * @retval  GROUPMANAGER_DUPLICATION if the group already exist
 * @retval  GROUPMANAGER_FAIL if fail to add the group 
 * @retval  GROUPMANAGER_SUCCESS on succes
 */
 GroupManagerResultCode GroupManagerOpenGroup(GroupManager* _groupManager, char* _groupName, char* , char* _port);
 /** 
 * @brief checks if the group is already exist
 * @param[in] _groupManager
 * @param[in] _groupKey group name of the user that request to log in
 * @param[in] _group the group that found will be in this variable
 * @retval  TRUE  group is in the system
 * @retval  FALSE group is not in the system
 */
int IsGroupExist(GroupManager* _groupManager, char* _groupKey, void** _group);
/** 
 * @brief join to exist group, the IP and the PORT numbers are been copying to the input values.
 * @param[in] _groupManager
 * @param[in] _groupName username of the user that request to log in
 * @param[in] _addressIP - a char array to hold the given IP address
 * @param[in] _port - a char array to hold the given port
 * @retval  GROUPMANAGER_SUCCESS	on success
 * @retval  GROUPMANAGER_FAIL on fail to join
 * @retval  GROUPMANAGER_NOT_FOUND group is not exist
 * @retval  GROUPMANAGER_NOT_INITIALIZED one of the inputs is NULL;
 */
GroupManagerResultCode GroupManagerJoinToGroup(GroupManager* _groupManager, char* _groupName, char* _addressIP, char* _port);
/** 
 * @brief fill given vector with name of existed groups.
 * @param[in] _groupManager
 * @param[in] _vector privously created vector
 * @retval  GROUPMANAGER_SUCCESS on success
 * @retval GROUPMANAGER_IS_EMPTY the vector is empty, no groups
 * @retval GROUPMANAGER_NOT_INITIALIZED invalid input
 
 */
GroupManagerResultCode GroupManagerGiveListOfGroups(GroupManager* _groupManager, Vector* _vector);
/** 
 * @brief leave a group, if it was the last user on the group, the group is closed.
 * @param[in] _groupManager
 * @param[in] _groupName username of the user that request to log in
 * @retval  GROUPMANAGER_SUCCESS on success
 * @retval  GROUPMANAGER_FAIL on fail to leave
 * @retval  GROUPMANAGER_NOT_FOUND group is not exist
 * @retval  GROUPMANAGER_NOT_INITIALIZED one of the inputs is NULL;
 * @retval  GROUPMANAGER_CLOSE_GROUP_FAIL cuoldent close the group;
 */
GroupManagerResultCode GroupManagerleaveGroup(GroupManager* _groupManager, char* _groupName);


#endif /* __GROUPMANAGER_H__ */

