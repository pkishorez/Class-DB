/**
 * @file lib_epoll.h
 * @author Kishore
 * @date June 25, 2015, 12:01 PM 
 */

#include "lib_types.h"
inline int epoll_add(int epfd, int fd, char *interest, int create_memory);
inline int epoll_modify(int epfd, int ep_index, char *interest);
inline int epoll_remove(int epfd, int ep_index);
inline struct epoll_member* epoll_get_member(struct epoll_event ev);