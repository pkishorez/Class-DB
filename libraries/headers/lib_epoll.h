/**
 * @file lib_epoll.h
 * @author Kishore
 * @date June 25, 2015, 12:01 PM 
 */

inline int epoll_add(int epfd, int fd, char *interest, int data);
inline int epoll_modify(int epfd, int fd, char *interest, int data);
inline int epoll_remove(int epfd, int fd);