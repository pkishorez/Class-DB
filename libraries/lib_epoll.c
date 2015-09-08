/**
 * @file lib_epoll.c
 * @author Kishore
 * @date June 25, 2015, 12:00 PM
 * 
 * @brief Library for wrapping epoll functionality
 * with simple to use API.
 */

#include "headers/headers.h"
#include "headers/lib_utilities.h"
#include "headers/lib_epoll.h"


static struct epoll_event * _epoll_interest_list(char *, int data);
static struct epoll_event * _epoll_interest_list(char *interest, int data)
{
	struct epoll_event ev;
	ev.events = 0;
	ev.events |= EPOLLET;
	if (interest[0]=='r' || interest[1]=='r')
		ev.events |= EPOLLIN;
	if (interest[0]=='w' || interest[1]=='w')
		ev.events |= EPOLLOUT;
	ev.data.fd = data;
	return &ev;
}

/**
 * Adds a file descriptor to the epoll interest list.
 * @param epfd : epoll instance.
 * @param fd : file descriptor that is to be added to the interest list.
 * @param interest : string of r and w which denotes read or write interest.
 * @param data : union that needs to be stored along with file descriptor.
 * The data could only be added here. Cannot be modified later.
 * @returns 0 on success and -1 on error.
 * 
 * The data parameter given is integer stored in ev.data.fd field. To
 * retrieve data later access ev.data.fd field to get stored data.
 */
inline int epoll_add(int epfd, int fd, char *interest, int data)
{
	struct epoll_event *ev = _epoll_interest_list(interest, data);
	return epoll_ctl(epfd, EPOLL_CTL_ADD, fd, ev);
}


/**
 * Modifies the interest list of file descriptor in epoll instance.
 * @param epfd : the epoll instance where the file descriptors are monitored.
 * @param fd : file descriptor whose interest list should be modified.
 * @param interest : string combination of r and w.
 * @returns 0 on success and -1 on failure.
 * 
 * The data parameter given is integer stored in ev.data.fd field. To
 * retrieve data later access ev.data.fd field to get stored data.
 */
inline int epoll_modify(int epfd, int fd, char *interest, int data)
{
	struct epoll_event *ev = _epoll_interest_list(interest, data);
	return epoll_ctl(epfd, EPOLL_CTL_MOD, fd, ev);
}

/**
 * Removes the file descriptor from list of monitoring in epoll.
 * @param epfd : epoll instance where the file descriptors are monitored.
 * @param fd : file descriptor that should be removed from the list.
 * @returns 0 on success and -1 on failure.
 */
inline int epoll_remove(int epfd, int fd)
{
	return epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
}


/**
 * Check if read event occurred on events integer.
 * @param events : integer field in structure `(epoll_event).ev`
 * @returns 1 if read event has occured else returns 0.
 */
inline int epoll_can_read(uint32_t events)
{
	if ((events & EPOLLIN) || (events & EPOLLPRI || (events & EPOLLRDBAND) || (events & EPOLLRDNORM)))
		return 1;
	return 0;
}

/**
 * Check if write event occurred on events integer.
 * @param events : integer field in structure `(epoll_event).ev`
 * @returns 1 if write event has occured else returns 0.
 */
inline int epoll_can_write(uint32_t events)
{
	if ((events & EPOLLOUT) || (events & EPOLLWRNORM) || (events & EPOLLWRBAND))
		return 1;
	return 0;
}

/**
 * Check if shutdown event occurred on events integer.
 * @param events : integer field in structure `(epoll_event).ev`
 * @returns 1 if shutdown event has occured else returns 0.
 */

inline int epoll_is_shutdown(uint32_t events)
{
	if ((events & EPOLLRDHUP) || (events & EPOLLHUP))
		return 1;
	return 0;
}

/**
 * Check if error event occurred on events integer.
 * @param events : integer field in structure `(epoll_event).ev`
 * @returns 1 if error event has occured else returns 0.
 */

inline int epoll_has_error(uint32_t events)
{
	if (events & EPOLLERR)
		return 1;
	return 0;
}