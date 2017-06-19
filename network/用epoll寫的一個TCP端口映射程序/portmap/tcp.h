#ifndef _TCP_H
#define _TCP_H

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <syslog.h>
#include <signal.h>
#include <pthread.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/resource.h>

#define SA struct sockaddr

#define ALIGN (sizeof(unsigned long) -1)

#define err(msg) \
do { \
	if (verbose) \
		fprintf(stderr, "%s %s: %s\n", __FUNCTION__, msg, strerror(errno)); \
	else \
		syslog(LOG_ERR, "%s %s: %s\n", __FUNCTION__, msg, strerror(errno)); \
} while (0)

struct map_list
{
	struct map_list *next;
	struct sockaddr_in saddr;
	struct sockaddr_in daddr;
};

struct map_list_head
{	
	struct map_list *head;
	struct map_list *tail;
};

struct data_item
{
	struct data_item *next;
	int dlen;
	int offset;
	char buf[0];
};

struct data_item_head
{
	struct data_item *head;
	struct data_item *tail;
};

struct gepoll
{
	int epfd;
	pthread_t tid;
	sig_atomic_t user;	// inline user number
};

struct event
{
	int epfd;
	int fd;
	uint32_t events;
	void *arg;
	void (*callback)(int epfd, int fd, uint32_t events, void *arg, struct event *e);
};

struct pair
{
	struct event *e;
	struct data_item_head dh;
	sig_atomic_t disable;		// 1 represent disable
};	

#define COMMON 0
#define PRIVAT 1

struct session
{
	struct gepoll *g;
	struct pair pair[2];
	int sockfd;			// in child thread add this fd
	struct sockaddr_in daddr;	// no use
};

#endif














