#ifndef __GROUP_H__
#define __GROUP_H__
#define MAGIC_NUM 245789657


#define GROUP_NAME_SIZE 8
#define PORT_SIZE 5
#define IP_SIZE 12
typedef struct Group Group;
typedef enum GroupResultCode {
	GROUP_SUCCESS = 0,
	GROUP_NOT_INITIALIZED,
    GROUP_FAIL,
    GROUP_IS_EMPTY
} GroupResultCode;

/** 
 * @brief Create a new Group with given groupName, port number and IP address.
 * @param[in] _groupName - groupName
 * @param[in] _port port number
 * @param[in] _addressIP IP address
 * @return newly created Group or null on failure
 */
Group* GroupCreate (char* _groupName, char* _port/*same to all*/, char* _addressIP);

/**
 * @brief destroy Group 
 * @param[in] _userManager : Group to be destroyed
 */
void GroupDestroy(void* _group);

/** 
 * @brief Increas the Number Of Users in the Group by 1.
 * @param[in] _group - the group to update.
 * @retval GROUP_NOT_INITIALIZED on un initialize input. 
 * @retval GROUP_SUCCESS on success
 */
GroupResultCode IncreasNumOfGroupUsers(Group* _group);

/** 
 * @brief get the group IP address in to the input char*.
 * @param[in] _group - the group.
 * @param[in] _addressIP - the char* to implant to the IP address.
 * @retval GROUP_NOT_INITIALIZED on un initialize input. 
 * @retval GROUP_SUCCESS on success
 * 
 */
GroupResultCode GroupGetIPAddress(Group* _group, char* _addressIP);
/** 
 * @brief get the group name into the given char*.
 * @param[in] _group - the group.
 * @param[in] _groupName - the char* to implant to the group name.
 * @retval GROUP_NOT_INITIALIZED on un initialize input. 
 * @retval GROUP_SUCCESS on success
 */
GroupResultCode GroupGetGroupName(Group* _group, char* _groupName);
/** 
 * @brief get the group PORT address in to the input char*.
 * @param[in] _group - the group.
 * @param[in] _addressIP - the char* to implant to the port address.
 * @retval GROUP_NOT_INITIALIZED on un initialize input. 
 * @retval GROUP_SUCCESS on success
 * 
 */
GroupResultCode GroupGetPort(Group* _group, char* _port);
/** 
 * @brief get the group number of users.
 * @param[in] _group - the group to check.
 * @retval int- number of users 
 */
int GroupGetNumOfUsers(Group* _group);
/** 
 * @brief decreas the Number Of Users in the Group by 1.
 * @param[in] _group - the group to update.
 * @retval GROUP_NOT_INITIALIZED on un initialize input. 
 * @retval GROUP_SUCCESS on success
 * @retval GROUP_IS_EMPTY ther is no users in the group.
 */
GroupResultCode DecreasNumOfGroupUsers(Group* _group);
#endif /* __GROUP_H__ */

