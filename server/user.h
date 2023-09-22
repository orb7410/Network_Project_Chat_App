#ifndef __USER_H__
#define __USER_H__
#define MAGIC_NUM 245789654
#define PASSWORD_SIZE 9
#define USERNAME_SIZE 16
#include "../../Day28/itrFunctions/list.h" /*DLList funcions decleration*/
#include "../../Day28/itrFunctions/listItr.h" /*ItrList funcions decleration*/
#include "../../Day28/itrFunctions/listitrfun.h" /*ItrListForEch funcions decleration*/
typedef struct User User;

typedef enum UserResultCode {
	USER_SUCCESS = 0,
	USER_NOT_INITIALIZED,
    USER_ALLOCATION_FAILED,
    USER_FAIL,
    USER_DUPLICATION,
    USER_NOT_FOUND,
    USER_WRONG_PASSWORD
} UserResultCode;

/** 
 * @brief Create a new User with given userName user password and userId(socket number).
 * @param[in] _userName - user's userName.
 * @param[in] _password - user's pasword.
 * @param[in] _userId - userId(socket number)
 * @return newly created User or null on failure
 */
User* UserCreate (char* _userName, char* _password, int _userId);

/**
 * @brief destroy User
 * @param[in] _userManager : User to be destroyed
 */
 
void UserDestroy(void* _user);
/** 
 * @brief activate user
 * @param[in] _user- existed user
 * @retval  USER_SUCCESS	on success
 * @retval  USER_FAIL if already deactive
 */ 
UserResultCode Useractivate(User* _user);

/** 
 * @brief deactivate user
 * @param[in] _user- existed user
 * @retval  USER_SUCCESS	on success
 * @retval  USER_FAIL if already active
 * @retval  USER_WRONG_PASSWORD wrong password
 */ 
UserResultCode DeactivateUser(User* _user);


/** 
 * @brief Update New User Id After Log In
 * @param[in] _user- existed user
  * @param[in] _userId new id
 */ 
void UpdateNewUserIdAfterLogIn(User* _user, int _userId);

/** 
 * @brief insert group name to the user list of groups.
 * @param[in] _user- existed user
 * @param[in] _groupName- name of the group to add
 * @retval  USER_SUCCESS	on success
 * @retval  USER_FAIL on fail
 * @retval  USER_NOT_INITIALIZED invalid input
 */ 
 UserResultCode UserInsertGroupToGroupsList(User* _user, char* _groupName);

/** 
 * @brief Check if group is in the user group list.
 * @param[in] _user- existed user
 * @param[in] _groupName- name of the group to find
 * @param[in] _itr- variable to implant there the itr of the group in the list.
 * @retval  TRUE	exist in list
 * @retval  FALSE not in list
 */ 
 int IsGroupInUserList(User* _user, char* _groupName, void** _itr);

 /** 
 * @brief remove group from user groups list.
 * @param[in] _user- existed user
 * @param[in] _groupName- name of the group to remove
 * @retval  USER_NOT_INITIALIZED	invalid inputs
 * @retval  USER_NOT_FOUND group is not in the user list
 * @retval  USER_FAIL fail to remove
 * @retval  USER_SUCCESS in success
 */ 
 UserResultCode UserLeaveGroupRemoveFromList(User* _user,  char* _groupName);
 /** 
 * @brief get the status of the user
 * @param[in] _user- existed user
 * @retval  activation status 0/1. 0-not active 1-active
 * @retval -1 if invalid input
 */ 
int UserGetActiveStatus(User* _user);
/** 
 * @brief get the password of the user
 * @param[in] _user- existed user
 * @retval  the user password
 */
char* UserGetPassword(User* _user);
/** 
 * @brief get the groups user list
 * @param[in] _user- the user
 * @retval  the user list
 * @retval NULL if user is NULL
 */
List* UserGetListOfGroups(User* _user);
/** 
 * @brief remove all the groups from user
 * @param[in] _user- the user
 * @retval  USER_FAIL fail to remove the groups from user
 * @retval  USER_SUCCESS on success
 * @retval USER_NOT_INITIALIZED on invalid user
 */
UserResultCode UserRemoveAllGroupsFromUser(User* _user);
#endif /* __USER_H__ */

