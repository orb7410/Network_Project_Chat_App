#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__
#include "../../Day26/genericheapCR/vector.h"

#define TAG_SIZE 2
#define BUFFER_SIZE 1000
#define NAME_SIZE 8
#define GROUP_NAME_SIZE 15
#define PASS_SIZE 14
#define PORT_SIZE 5
#define IP_SIZE 12

typedef struct UserDetail UserDetail;
typedef struct GroupDetail GroupDetail;

struct UserDetail
{
    char m_userName[NAME_SIZE];
    char m_password[PASS_SIZE];
};

struct GroupDetail
{
    int m_status;
    char m_ip[IP_SIZE];
    char m_port[PORT_SIZE];
};

typedef enum Tag {
    REGISTR_TAG =1,
    LOG_IN_TAG =2,
    OPEN_GROUP_TAG=3,
    JOIN_GROUP_TAG=4,
    PRESENT_ACTIVE_GROUPS_LIST_TAG=5,
    LIVE_GROUP_TAG=6,
    LOG_OUT_TAG=7,
    REGISTR_REPLAY_TAG,
    LOG_IN_REPLAY_TAG,
    OPEN_GROUP_REPLAY_TAG,
    JOIN_GROUP_REPLAY_TAG,
    PRESENT_ACTIVE_GROUPS_LIST_REPLAY_TAG,
    LIVE_GROUP_REPLAY_TAG,
    LOG_OUT_REPLAY_TAG
}Tag;


typedef enum status {
	STATUS_SUCCESS = 0,
    STATUS_FAIL,
    STATUS_DUPLICATION,
    STATUS_NAME_GROUP_NOT_FOUND,
    STATUS_WRONG_USERNAME_OR_PASSWORD,
    STATUS_LIST_OF_GROUPS_IS_EMPTY
} status;

/** 
 * @brief retrieve tag
 * @param[in] _buffer - 
 * @retval  USERMANAGER_SUCCESS	on success
 * 
 */
int RetrieveTag(char* _buffer);

/** 
 * @brief packing message into a buffer in registration case
 * @param[in] _userDetail pointer to userDetail struct
 * @param[in] _buffer to store the packed 
 * @param[in] _bufferSize size of the buffer
 * @retval  total length of packed 
 *          return 0 if _userDetail is NULL or if _bufferSize <= 0
 */ 
int PackRegistration(UserDetail* _userDetail, char* _buffer,  int _bufferSize);

/** 
 * @brief unpacking message from a buffer in registration case
 * @param[in] _buffer pointer to buffer which contains the packed message
 * @param[in] _bufferSize size of the buffer
 * @retval  userDetail struct with unpacked messsage which contain username and password
 *          if the tag of the pack wrong or _bufferSize <= 0 , username and password will be empty.
 *          
 */ 
UserDetail UnpackRegistration(char* _buffer, int _bufferSize);

/** 
 * @brief packing  replay message into buffer to replay in registration case
 * @param[in] _status to store the packed 
 * @param[in] _buffer pointer to buffer
 * @retval total length of pack  
 */ 
int PackRegistrationReplay(int _status, char* _buffer);

/** 
 * @brief unpacking replay message from a buffer regisration case
 * @param[in] _buffer pointer to buffer which contains the packed message
 * @param[in] _bufferSize size of the buffer
 * @retval  return status 
 *          return -1 if tag wrong or _bufferSize <= 0
 */ 
int UnPackRegistrationReplay(int _bufferSize, char* _buffer);

/** 
 * @brief packing message into a buffer in login case
 * @param[in] _userDetail pointer to userDetail struct
 * @param[in] _buffer to store the packed 
 * @retval  total length of packed 
 *          return 0 if _userDetail null or if _bufferSize <= 0
 */ 
int PackLogIn(UserDetail* _userDetail, char* _buffer,  int _bufferSize);

/** 
 * @brief unpacking message from a buffer in login case
 * @param[in] _buffer pointer to buffer which contains the packed message
 * @param[in] _bufferSize size of the buffer
 * @retval  userDetail struct with unpacked messsage
 *          if the tag of the pack wrong or _bufferSize <= 0 , username and password will be empty.
 */  
UserDetail UnpackLogIn(char* _buffer, int _bufferSize);

/** 
 * @brief packing raplay message into a buffer to replay in login case
 * @param[in] _structMessage pointer to userDetail struct
 * @param[in] _buffer to store the packed 
 * @retval  total length of packed
 */ 
int PackLogInReplay(int _status, char* _buffer);

/** 
 * @brief unpacking replay message from a buffer in login case
 * @param[in] _buffer pointer to buffer which contains the packed message
 * @param[in] _bufferSize size of the buffer
 * @retval  return status 
 *          return -1 if tag wrong or _bufferSize <= 0
 */ 
int UnPackLogInReplay(int _bufferSize, char* _buffer);

/** 
 * @brief packing  message into a buffer in open Group case
 * @param[in] _buffer to store the packed
 * @param[in] _bufferSize size of the buffer
 * @param[in] _groupName the name group to be pack into the buffer
 * @retval  total length of packed 
 *          return 0 if _groupName null or if _bufferSize <= 0
 */
int PackOpenGroup(char* _groupName, char* _buffer,  int _bufferSize);

/** 
 * @brief unpacking message from a buffer in open group case
 * @param[in] _buffer pointer to buffer which contains the packed message
 * @param[in] _bufferSize size of the buffer
 * @param[in] _nameGroup to store the name group  
 * @retval  total length of packed 
 *          return 0 if tag wrong or if _bufferSize <= 0
 */  
int UnPackOpenGroup(char* _buffer, int _bufferSize, char* _nameGroup);

/** 
 * @brief packing  message into a buffer to replay in open group case
 * @param[in] _port the port number to be packed into the buffer
 * @param[in] _ip the ip address to be packed into the buffer
 * @param[in] _buffer to store the packed
 * @param[in] _status size of the buffer
 * @retval  total length of packed
 *          return 0 if _port or _ip NULL
 */ 
int PackOpenGroupReplay(char* _port, char* _ip ,int _status, char* _buffer);

/** 
 * @brief unpacking replay message from a buffer to replay in open group case
 * @param[in] _buffer pointer to buffer which contains the packed message
 * @param[in] _bufferSize size of the buffer
 * @retval GroupDetail struct which contain the status:
 *                                           STATUS_FAIL- if open group fail, 
 *                                           STATUS_SUCCESS- if open group seccess, 
 *                                           STATUS_DUPLICATION - if the the name is already taken
 *                                           status =-1 if tag wrong or _bufferSize <= 0
 */ 
GroupDetail UnPackOpenGroupReplay(int _bufferSize, char* _buffer) ;

/** 
 * @brief packing  message into a buffer in join Group case
 * @param[in] _buffer to store the packed
 * @param[in] _bufferSize size of the buffer
 * @param[in] _groupName the name group to be pack into the buffer
 * @retval  total length of packed 
 *          return 0 if _groupName null or if _bufferSize <= 0
 */
int PackJoinGroup(char* _groupName, char* _buffer,  int _bufferSize);

/** 
 * @brief unpacking message from a buffer in join group case
 * @param[in] _buffer pointer to buffer which contains the packed message
 * @param[in] _bufferSize size of the buffer
 * @param[in] _nameGroup to store the name group  
 * @retval  total length of packed 
 */  
int UnPackJoinGroup(char* _buffer, int _bufferSize, char* _groupName);

/** 
 * @brief packing  message into a buffer to replay in join group case
 * @param[in] _port the port number to be packed into the buffer
 * @param[in] _ip the ip address to be packed into the buffer
 * @param[in] _buffer to store the packed
 * @param[in] _status size of the buffer
 * @retval  total length of packed or 0 if tag wrong or _bufferSize <=0
 */ 
int PackJoinGroupReplay(char* _port, char* _ip ,int _status, char* _buffer);

/** 
 * @brief unpacking replay message from a buffer to replay in join group case
 * @param[in] _buffer pointer to buffer which contains the packed message
 * @param[in] _bufferSize size of the buffer
 * @retval GroupDetail struct which contain the status:
 *                                           STATUS_FAIL- if join group fail, 
 *                                           STATUS_SUCCESS- if join group seccess, 
 *                                           STATUS_NAME_GROUP_NOT_FOUND - if the the nameGroup is already taken
 *                                           status =-1 if tag wrong or _bufferSize <= 0
 */ 
GroupDetail UPackJoinGroupReplay(int _bufferSize, char* _buffer);

/** 
 * @brief packing  message into a buffer in persent groups list case
 * @param[in] _buffer to store the packed
 * @param[in] _bufferSize size of the buffer
 * @retval  total length of packed or 0 if _bufferSize <=0
 */ 
int PackListGroups(char* _buffer,  int _bufferSize);

/** 
 * @brief unpacking message from a buffer in persent groups list case
 * @param[in] _buffer pointer to buffer which contains the packed message
 * @param[in] _bufferSize size of the buffer
 * @retval  total length of packed or 0 if _bufferSize <=0
 */  
int UnPackListGroups(char* _buffer,  int _bufferSize);

/** 
 * @brief packing  message into a buffer in persent groups list case
 * @param[in] _buffer to store the packed
 * @param[in] _bufferSize size of the buffer
 * @param[in] _pVector pointer to a Vector that containing the group names
 * @retval  total length of packed  
 *          0 if vector is empty or _bufferSize <=0 or if VectorRemove fail
 */ 
int PackListGroupsReplay(Vector* _pVector ,int _bufferSize, char* _buffer);

/** 
 * @brief unpacking message into a buffer in persent groups list case
 * @param[in] _buffer pointer to buffer which contains the packed message
 * @param[in] _bufferSize size of the buffer
 * @param[in] _pVector pointer to  empty Vector- to add the groups name inside
 * @retval  total length of packed
 *          0 if vector is not empty or _bufferSize <=0 or if VectorAppend fail or if tag wrong
 */
int UnPackListGroupsReplay(Vector* _pVector, int _bufferSize, char* _buffer);

/** 
 * @brief packing  message into a buffer in leave Group case
 * @param[in] _buffer to store the packed
 * @param[in] _bufferSize size of the buffer
 * @param[in] _groupName the name group client want to leave to be pack into the buffer
 * @retval  total length of packed 
 *          return 0 if _groupName null or if _bufferSize <= 0
 */
int PackLeaveGroup(char* _groupName, char* _buffer,  int _bufferSize);

/** 
 * @brief unpacking message from a buffer in leave group case
 * @param[in] _buffer pointer to buffer which contains the packed message
 * @param[in] _bufferSize size of the buffer
 * @param[in] _nameGroup to store the name group  
 * @retval  total length of packed 
 *          return 0 if tag wrong or if _bufferSize <= 0
 */ 
int UnPackLeaveGroup(char* _buffer, int _bufferSize, char* _groupName);

/** 
 * @brief packing  replay message into buffer to replay in leave group case
 * @param[in] _status to store the packed 
 * @param[in] _buffer pointer to buffer
 * @retval total length mof pack  
 */ 
int PackLeaveGroupReplay(int _status, char* _buffer);

/** 
 * @brief unpacking replay message from a buffer in leave group case
 * @param[in] _buffer pointer to buffer which contains the packed message
 * @param[in] _bufferSize size of the buffer
 * @retval  return status 
 *          return -1 if tag wrong or _bufferSize <= 0
 */ 
int UnPackLeaveGroupReplay(int _bufferSize, char* _buffer);

/** 
 * @brief packing  message into a buffer in log out case
 * @param[in] _buffer to store the packed
 * @param[in] _bufferSize size of the buffer
 * @retval  total length of packed or 0 if _bufferSize <=0
 */ 
int PackLogOut(char* _buffer,  int _bufferSize);

/** 
 * @brief unpacking message from a buffer in log out case
 * @param[in] _buffer pointer to buffer which contains the packed message
 * @param[in] _bufferSize size of the buffer
 * @retval  total length of packed or 0 if _bufferSize <=0
 */  
int UnPackLogOut(char* _buffer,  int _bufferSize);

/** 
 * @brief packing  replay message into buffer to replay in log out case
 * @param[in] _status to store the packed 
 * @param[in] _buffer pointer to buffer
 * @retval total length mof pack  
 */
int PackLogOutReplay(int _status, char* _buffer);

/** 
 * @brief unpacking replay message from a buffer in log out case
 * @param[in] _buffer pointer to buffer which contains the packed message
 * @param[in] _bufferSize size of the buffer
 * @retval  return status 
 *          return -1 if tag wrong or _bufferSize <= 0
 */
int UnPackLogOutReplay(int _bufferSize, char* _buffer);

#endif /* __PROTOCOL_H__ */




