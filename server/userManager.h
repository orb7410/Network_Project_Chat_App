#ifndef __USERMANAGER_H__
#define __USERMANAGER_H__
#include <stddef.h>
#include "user.h"
#include "../../Day28/itrFunctions/list.h" /*DLList funcions decleration*/
#include "../../Day28/itrFunctions/listItr.h" /*ItrList funcions decleration*/
#include "../../Day28/itrFunctions/listitrfun.h" /*ItrListForEch funcions decleration*/
#define TRUE 1
#define FALSE 0
typedef struct UserManager UserManager;

typedef enum UserManagerResultCode {
	USERMANAGER_SUCCESS = 0,
	USERMANAGER_NOT_INITIALIZED,
    USERMANAGER_ALLOCATION_FAILED,
    USERMANAGER_FAIL,
    USERMANAGER_DUPLICATION,
    USERMANAGER_USER_NOT_FOUND,
    USERMANAGER_WRONG_PASSWORD,
    USERMANAGER_NOT_SAVED,
    USERMANAGER_GROUP_NOT_FOUND,
    USERMANAGER_NOT_ACTIVE
} UserManagerResultCode;


/** 
 * @brief Create a new UserManager with given maximum number of users.
 * @param[in] _maxUsers - Expected max number of users. 
 * @return newly created UserManager or null on failure
 * @details  all the register users are update to created UserManager.
 */
UserManager* UserManagerCreate (size_t _maxUsers);
/**
 * @brief destroy UserManager and set *_userManager to null
 * @param[in] _userManager : UserManager to be destroyed
 */
void UserManagerDestroy (UserManager** _userManager);

/** 
 * @brief Add new user.
 * @param[in] _userManager - _userId UserManager 
 * @param[in] _userName - username of the user that request to sign up
 * @param[in] _password  - password of the user that request to sign up
 * @param[in] _userId - user id (sock number).
 * @retval  USERMANAGER_SUCCESS	on success
 * @retval  USERMANAGER_FAIL on fail
 * @retval  USERMANAGER_DUPLICATION if user is allredy signed up
 * 
 */
 UserManagerResultCode UserManagerAddNewUser(UserManager* _userManager, char* _userName, char* _password,  int _userId);

/** 
 * @brief log in the user to the system
 * @param[in] _userManager
 * @param[in] _userName username of the user that request to log in
 * @param[in] _password password of the user that request to log in
 * @retval  USERMANAGER_SUCCESS	on success
 * @retval  USERMANAGER_FAIL on fail to log in because already active
 * @retval  USERMANAGER_USER_NOT_FOUND user is not registered
 * @retval  USERMANAGER_WRONG_PASSWORD wrong password
 * @retval  USERMANAGER_NOT_INITIALIZED one of the inputs is NULL;
 * @retval  USERMANAGER_NOT_SAVED- the user diteils were not saved on disk, the user will not save when the system down.
 */
UserManagerResultCode UserManagerLogin(UserManager* _userManager, char* _userName, char* _password, int _userId);
/** 
 * @brief checks if user is already sighned up
 * @param[in] _userManager
 * @param[in] _userKey username of the user that request to log in
 * @param[in] _user the user that found will be in this variable
 * @retval  TRUE	user is in the system
 * @retval  FALSE user is not in the system
 */
int IsUserExist(UserManager* _userManager, char* _userKey, void** _user);
/** 
 * @brief insert group name to the user list of groups.
 * @param[in] _userManager
 * @param[in] _userName username of the user that request to add group to his list
 * @param[in] _groupName the group want to add to list
 * @retval  USERMANAGER_NOT_INITIALIZED invalid input
 * @retval  USERMANAGER_USER_NOT_FOUND username is not correct
 * @retval USERMANAGER_FAIL on fail
 * @retval USERMANAGER_SUCCESS on seccses
 * @retval USERMANAGER_NOT_ACTIVE the user is not active, cant join group
 */
UserManagerResultCode UserManagerInsertGroupToUser(UserManager* _userManager, char* _userName, char* _groupName);
/** 
 * @brief remove group from user list of groups.
 * @param[in] _userManager
 * @param[in] _userName username of the user that request to add group to his list
 * @param[in] _groupName the group want to add to list
 * @retval  USERMANAGER_NOT_INITIALIZED invalid input
 * @retval  USERMANAGER_USER_NOT_FOUND username is not correct
 * @retval  USERMANAGER_GROUP_NOT_FOUND the given group is not at the list 
 * @retval USERMANAGER_FAIL on fail to remove the group from user list
 * @retval USERMANAGER_SUCCESS on seccses to remove the group from user list
 * @retval USERMANAGER_NOT_ACTIVE the user is not active, cant leave group
 */
UserManagerResultCode LeaveGroup(UserManager* _userManager, char* _userName, char* _groupName);

/** 
 * @brief checks if username is correct.
 * @param[in] _user the user
 * @param[in] _password the password to check
 * @retval TRUE if password ok
 * @retval FALSE if password wrong or invalid inputs
 */
int IsPasswordCorrect(User* _user, char* _password);
/** 
 * @brief get the list of groups of the user 
 * @param[in] _user the user
 * @param[in] _userName the username of the user
 * @retval List of user's groups
 * @retval NULL if fail
 */
List* UserManagerGetGroupsList(UserManager* _userManager, char* _userName);
/** 
 * @brief  remove all the groups from user 
 * @param[in] _userManager the user manager
 * @param[in] _userName the username of the user
 * @retval  USERMANAGER_FAIL fail to remove the groups from user
 * @retval  USERMANAGER_USER_NOT_FOUND if iser is not exist
 * @retval  USER_NOT_INITIALIZED on invalid inputs
 * @retval  USERMANAGER_NOT_ACTIVE if user is not active
 * @retval  USERMANAGER_SUCCESS on success
 */
UserManagerResultCode UserManagerCleanUserGroupsForLogOut(UserManager* _userManager, char* _userName);

#endif /* __USERMANAGER_H__ */
