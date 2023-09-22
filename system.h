#ifndef __SYSTEM_H__
#define __SYSTEM_H__

typedef struct Window
{
    int m_firstWindow;
    int m_secendWindow;
}Window;
/**  
 * @brief a function to open the chat windows
 * @param[in] _clientPort port for conection
 * @param[in] _clientIP IP for conection
 * @param[in] _userName the name of user that open the chat
 * @return a pointer to ClientManager struct
 */
Window ChatWindowsOpener(char* _clientPort, const char* _clientIP, char* _userName);

/**  
 * @brief a function to close chat windows 
 * @param[in] _window a pointer to windows pid struct to close chat
 * @return none
 */
void CloseWindows(Window _window);

/**  
 * @brief a function to creat a struct to save windows pid
 * @param[in] none
 * @return a pointer to Window struct
 */
Window* CreateWindowsPid();

/**  
 * @brief a function to set *_pWindow to NULL
 * @param[in] _pWindow struct to be destroyed 
 * @return none
 */
void WindowDestroy(Window** _pWindow);


#endif /* __SYSTEM_H__ */