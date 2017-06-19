#include "tcp.h"

static int count = 10;		// epoll and thread number, default 10
static int verbose;		// debug

struct gepoll *ge;

struct map_list_head mh = { .head = NULL, .tail = NULL, };

static void usage(void)
{
	printf("Usage: ./tcp or ./tcp -v -n optarg\n");
}

static int unlimit(void)
{
	struct rlimit rlim;
	
	rlim.rlim_cur = rlim.rlim_max = 1 << 16;

	return setrlimit(RLIMIT_NOFILE, &rlim);
}

static void sig_int(int sig)
{
	int i;

	for (i = 0; i < count; ++i)
		printf("ge[%d].uer = %d\n", i, ge[i].user);
}

static int sig_handle(int sig, void (*sigcall)(int))
{
	struct sigaction act;
	
	act.sa_handler = sigcall;
	act.sa_flags = SA_RESTART;
	sigemptyset(&act.sa_mask);

	return sigaction(sig, &act, NULL);
}

static int init_signal_limit(void)
{
	if (unlimit() == -1)	
		err("unlimit");
	
	if (!verbose)
		sig_handle(SIGPIPE, SIG_IGN);

	return sig_handle(SIGINT, sig_int);
}

static int get_ip_port(char *str, char *ip, int *port)
{
	char *p;

	if ((p = strchr(str, ':')) != NULL)	
	{
		*p = ' ';
		memset(ip, '\0', sizeof(ip));
		sscanf(str, "%s %d", ip, port);
		return 0;
	}
	else
		err("strchr");

	return -1;
}

static void compose_addr(struct sockaddr_in *addr, char *ip, int port)
{
	addr->sin_family = AF_INET;
	addr->sin_port = htons(port);
	if (inet_pton(AF_INET, ip, &addr->sin_addr) <= 0)
		err("inet_pton");
}

static void map_item_enqueue(struct map_list_head *h, struct map_list *item)
{
	if (!h->head)
		h->head = h->tail = item;
	else
	{
		h->tail->next = item;
		h->tail = item;
	}
	item->next = NULL;
}

static void parse_addr_str(struct map_list_head *h, char *saddr, char *daddr)
{
	char sip[16], dip[16];
	int sport, dport;
	struct map_list *item;	

	if (get_ip_port(saddr, sip, &sport) == 0 && get_ip_port(daddr, dip, &dport) == 0)
	{	
		item = (struct map_list *)malloc(sizeof(struct map_list));	
		if (item)
		{
			memset(item, '\0', sizeof(struct map_list));
			compose_addr(&item->saddr, sip, sport);
			compose_addr(&item->daddr, dip, dport);
			map_item_enqueue(h, item);
		}
		else	
			err("malloc");	
	}
	else	
		err("get_ip_port");
}

static void output_map_list(struct map_list_head *h)
{
	struct map_list *item;
	char ip[16];

	for (item = h->head; item != NULL; item = item->next)
	{
		printf("saddr: %s:%d, daddr: %s:%d\n", 
			inet_ntop(AF_INET, &item->saddr.sin_addr, ip, sizeof(ip)),
			ntohs(item->saddr.sin_port),
			inet_ntop(AF_INET, &item->daddr.sin_addr, ip, sizeof(ip)),
			ntohs(item->daddr.sin_port));
	}
}

static int init_map_list(struct map_list_head *h)
{
	FILE *fp;
	char buf[1024], saddr[32], daddr[32];
	
	if ((fp = fopen("./config", "r")) == NULL)
	{
		err("fopen");
		return -1;
	}

	for (;;)
	{
		memset(buf, '\0', sizeof(buf));
		if (fgets(buf, sizeof(buf), fp) == NULL)
			break;

		memset(saddr, '\0', sizeof(saddr));
		memset(daddr, '\0', sizeof(daddr));
		sscanf(buf, "%s %s", saddr, daddr);
		parse_addr_str(h, saddr, daddr);
	}
	
	if (verbose)
		output_map_list(h);

	return 0;
}

static int init_gepoll(int num)
{
	int i;
	
	ge = (struct gepoll *)calloc(num, sizeof(struct gepoll));
	if (ge)
	{
		for (i = 0; i < num; ++i)
		{	
			if ((ge[i].epfd = epoll_create(8192)) == -1)
				err("epoll_create");
		}

		return 0;
	}

	return -1;
}

static void event_dispatch_loop(int epfd, int size)
{
	struct epoll_event ev[size];
	int nfds, i;
	struct event *e;
	uint32_t events;

	nfds = epoll_wait(epfd, ev, size, -1);
	if (nfds <= 0)
	{
		err("epoll_wait");
		return;
	}

	for (i = 0; i < nfds; ++i)
	{
		events = ev[i].events;
		e = ev[i].data.ptr;
		if (e && e->callback)		
			e->callback(e->epfd, e->fd, events, e->arg, e);
		else
			err("e is null");
	}
}

static void * child_thread(void *arg)
{
	struct gepoll *g = (struct gepoll *)arg;
	int epfd;

	if (!g)
	{
		err("g is null");
		return NULL;
	}
	
	epfd = g->epfd;
	for (;;)
		event_dispatch_loop(epfd, 8192);

	return NULL;
}

static int init_thread(int num)
{
	pthread_attr_t attr;
	pthread_t tid;
	int i;

	if (pthread_attr_init(&attr) != 0)
		err("pthread_attr_init");

	if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED) != 0)
		err("pthread_attr_setdetachstate");
	
	if (pthread_attr_setstacksize(&attr, 20 << 20) != 0)
		err("pthread_attr_setstacksize");
		
	for (i = 0; i < num; ++i)
	{
		if (pthread_create(&tid, &attr, child_thread, &ge[i]) != 0)
			err("pthread_create");
		else
			ge[i].tid = tid;
	}	

	return 0;
}

static int tcp_listen(struct sockaddr_in *addr)
{		
	int fd;
	int on = 1;

	if ((fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
	{
		err("socket");
		return -1;
	}
		
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
		err("setsockopt");
	
	if (bind(fd, (SA *)addr, sizeof(struct sockaddr_in)) == -1)
	{
		err("bind");
		goto err;
	}

	if (listen(fd, 8192) == -1)
	{
		err("listen");
		goto err;
	}

	return fd;
err:
	close(fd);
	return -1;
}

static struct event * event_set(int epfd, int fd, uint32_t events, void *arg,
			void (*call)(int, int, uint32_t, void *,struct event *))
{
	struct event *e;
	
	e = (struct event *)malloc(sizeof(struct event));
	if (e)
	{
		e->epfd = epfd;
		e->fd = fd;
		e->events = events;
		e->arg = arg;
		e->callback = call;
	}

	return e;
}

static int event_add(struct event *e)
{
	struct epoll_event ev;
	
	if (!e)
	{	
		err("e is null");
		return -1;
	}

	ev.events = e->events;
	ev.data.ptr = e;
	
	return epoll_ctl(e->epfd, EPOLL_CTL_ADD, e->fd, &ev);
}

static int set_nonblock(int fd)
{
	int flags;
	
	if ((flags = fcntl(fd, F_GETFL, 0)) == -1)
	{
		err("fcntl");
		return -1;
	}
	
	return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

static int get_request(int fd, struct sockaddr_in *addr, socklen_t *len)
{
	int sockfd;

	sockfd = accept(fd, (SA *)addr, len);
	if (sockfd > 0)
	{
		if (set_nonblock(sockfd) == -1)
			err("set_nonblock");
		return sockfd;	
	}

	return -1;
}

static int tcp_connect(struct sockaddr_in *addr)
{
	int sockfd;
	
	if ((sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
	{	
		err("socket");
		return -1;
	}
	
	if (set_nonblock(sockfd) == -1)	
		err("set_nonblock");
	
	if (connect(sockfd, (SA *)addr, sizeof(struct sockaddr_in)) == -1)
	{
		if (errno != EINPROGRESS)
		{
			err("connect");
			goto err;
		}
	}

	return sockfd;
err:
	close(sockfd);
	return -1;
}

static struct session * alloc_new_session(void)
{
	struct session *s;
		
	s = (struct session *)malloc(sizeof(struct session));
	if (s)
	{
		memset(s, '\0', sizeof(struct session));
		s->g = NULL;	
		s->pair[COMMON].e = s->pair[PRIVAT].e = NULL;
		s->pair[COMMON].disable = s->pair[PRIVAT].disable = 0;
		s->pair[COMMON].dh.head = s->pair[COMMON].dh.tail = NULL;
		s->pair[PRIVAT].dh.head = s->pair[PRIVAT].dh.tail = NULL;
	}

	return s;
}

static struct gepoll * get_slack_epoll(struct session *s)
{
	struct gepoll *ptr = &ge[0];
	int i;
	
	if (s->g)		// add to the same child thread
		return s->g;

	for (i = 0; i < count; ++i)
	{	
		if (ge[i].user <= 0)
		{
			ptr = &ge[i];
			break;
		}
		if (ge[i].user < ptr->user)
			ptr = &ge[i];
	}

	s->g = ptr;
	ptr->user++;
	return ptr;
}

static int add_to_session(struct session *s, int fd, uint32_t events, int which, 
			void (*call)(int, int, uint32_t, void *,struct event *))
{
	struct event *e;
	int epfd;

	s->g = get_slack_epoll(s);
	if (!s->g)
	{
		err("s->g is null");
		return -1;
	}
	
	epfd = s->g->epfd;
	e = event_set(epfd, fd, events, s, call);
	if (!e || event_add(e) == -1)
	{
		err("event_set or event _add");
		s->g->user--;
		free(e);
		return -1;
	}
	else
		s->pair[which].e = e;

	return 0;
}

static struct pair * get_which_pair(struct session *s, struct event *e, int which)
{	
	struct pair *me = (s->pair[COMMON].e == e) ? (&s->pair[COMMON]) : (&s->pair[PRIVAT]);
	struct pair *other = (s->pair[COMMON].e == e) ? (&s->pair[PRIVAT]) : (&s->pair[COMMON]);

	if (which)
		return me;
	else
		return other;
}

static int event_events_change(struct event *e, uint32_t events)
{
	struct epoll_event ev;

	ev.events = events;
	ev.data.ptr = e;

	return epoll_ctl(e->epfd, EPOLL_CTL_MOD, e->fd, &ev);
}

static int comm_read_failure(struct pair *me, struct pair *other, char *buf, int n)
{
	me->disable = 1;

	if (event_events_change(me->e, 0) == -1)
		err("event_events_change");

	if (!other->disable)
	{
		if (event_events_change(other->e, EPOLLOUT) == -1)
			err("event_events_change");

		return 0;
	}

	return -1;	// represent failure
}

static struct data_item * alloc_data_item(int dlen)
{	
	struct data_item *item;
	int size;

	size = (sizeof(struct data_item) + dlen + ALIGN) & ~ALIGN;
	
	item = (struct data_item *)malloc(size);
	if (item)
	{	
		memset(item, '\0', size);
		item->next = NULL;
		item->dlen = dlen;	
		item->offset = 0;
	}

	return item;
}

static void data_item_enqueue(struct data_item_head *h, struct data_item *item)
{
	if (!h->head)
		h->head = h->tail = item;
	else
		h->tail = h->tail->next = item;
}

static int comm_read_success(struct pair *me, struct pair *other, char *buf, int n)
{
	struct data_item *item;
	struct data_item_head *oh = &(other->dh);

	if ((item = alloc_data_item(n)) == NULL)
	{
		err("alloc_data_item");
		return -1;
	}

	memcpy(item->buf, buf, n);
	data_item_enqueue(oh, item);

	if (!other->disable)	
	{
		if (event_events_change(other->e, EPOLLIN | EPOLLOUT) == -1)
			err("event_events_change");
	}

	return 0;
}

static int handle_comm_read(int fd, struct session *s, struct event *e)
{
	struct pair *me, *other;
	char buf[4096];
	ssize_t n;

	me = get_which_pair(s, e, 1);
	other = get_which_pair(s, e, 0);
	
	if (me->disable)	// already close
		return -1;

	memset(buf, '\0', sizeof(buf));
	n = read(fd, buf, sizeof(buf));
	if (n <= 0)
		return comm_read_failure(me, other, buf, n);	// return -1
	else
		return comm_read_success(me, other, buf, n);	// return 0
}

struct data_item * data_item_peek(struct data_item_head *h)
{
	return h->head;
}

static int comm_write_failure(struct pair *me, struct pair *other)
{
	me->disable = 1;
	if (event_events_change(me->e, 0) == -1)
		err("event_events_change");

	if (!other->disable)
	{
		if (event_events_change(other->e, EPOLLOUT) == -1)
			err("event_events_change");

		return 0;
	}

	return -1;
}

static struct data_item * data_item_dequeue(struct data_item_head *h)
{
	struct data_item *result = NULL;
		
	if (h->head)
	{
		result = h->head;
		h->head = h->head->next;
		h->tail = h->head ? h->tail : h->head;
	}

	return result;
}

static int comm_write_success(struct data_item_head *h, struct data_item *item, int len)
{	
	item->offset += len;
	item->dlen -= len;
	if (item->dlen == 0)
	{
		item = data_item_dequeue(h);
		free(item);
	}

	return 0;
}

static int nothing_write(struct pair *me, struct pair *other)
{
	uint32_t events;
	
	if (other->disable)
	{
		me->disable = 1;
		return -1;
	}

	if (event_events_change(me->e, EPOLLIN) == -1)
		err("event_events_change");

	events = other->dh.head ? (EPOLLIN | EPOLLOUT) : EPOLLIN;
	if (event_events_change(other->e, events) == -1)
		err("event_events_change");

	return 0;
}

static int handle_comm_write(int fd, struct session *s, struct event *e)
{
	struct pair *me, *other;
	struct data_item_head *mh;
	struct data_item *item;
	int n;

	me = get_which_pair(s, e, 1);
	other = get_which_pair(s, e, 0);
	mh = &me->dh;
	
	if (me->disable)
		return -1;
	
	item = data_item_peek(mh);
	if (item)
	{
		write(STDOUT_FILENO, item->buf, item->dlen);
		n = write(fd, item->buf + item->offset, item->dlen);
		if (n == -1)
			return comm_write_failure(me, other);
		else
		{
			return comm_write_success(mh, item, n);	
		}
	}
	else
		return nothing_write(me, other);
}

static void free_session(struct session *s)
{
	struct data_item *item;

	if (s->pair[COMMON].disable == 1 && s->pair[PRIVAT].disable == 1)
	{
		close(s->pair[COMMON].e->fd);	
		close(s->pair[PRIVAT].e->fd);
		s->g->user--;
		while ((item = data_item_dequeue(&s->pair[COMMON].dh)) != NULL)
			free(item);
		while ((item = data_item_dequeue(&s->pair[PRIVAT].dh)) != NULL)
			free(item);
		free(s->pair[COMMON].e);
		free(s->pair[PRIVAT].e);
		free(s);
	}
}

static void comm_transmit(int epfd, int fd, uint32_t events, void *arg,struct event *e)
{
	struct session *s = (struct session *)arg;

	if (!s)
	{
		err("s is null");
		return;
	}

	if (events & EPOLLIN)
	{
		if (handle_comm_read(fd, s, e) == -1)
			goto err;
	}
	if (events & EPOLLOUT)
	{
		if (handle_comm_write(fd, s, e) == -1)
			goto err;
	}

	return;
err:
	free_session(s);
}

static int event_callback_change(struct event *e, 
			void (*call)(int, int, uint32_t, void *,struct event *))
{
	if (!e || !call)
		return -1;

	e->callback = call;
	return 0;
}

static void priv_write(int epfd, int fd, uint32_t events, void *arg,struct event *e)
{
	struct session *s = (struct session *)arg;
	int cfd;
	int optval;
	socklen_t optlen = sizeof(optval);
	
	if (!s)
	{
		err("s is null");
		return;
	}

	if (events & EPOLLIN)	// no should
	{
		if (getsockopt(fd, SOL_SOCKET, SO_ERROR, &optval, &optlen) == -1)
			err("getsockopt");
		
		if (optval)	// socket error
		{
			err("optval is not 0");
			goto err;
		}
	}

	cfd = s->sockfd;
	if (add_to_session(s, cfd, EPOLLIN | EPOLLOUT, COMMON, comm_transmit) == -1)
	{
		err("add_to_session");
		goto err;
	}
		
	if (event_callback_change(s->pair[PRIVAT].e, comm_transmit) == -1)
		err("event_callback_change");

	return;
err:
	close(fd);
	close(s->sockfd);
	s->g->user--;
	free(s->pair[PRIVAT].e);
	free(s);
}

static void call_event_listen(int epfd, int fd, uint32_t events, void *arg,struct event *e)
{
	struct sockaddr_in *daddr = (struct sockaddr_in *)arg;
	int cfd, pfd;
	struct session *s;
		
	if (!daddr)
	{	
		err("daddr is null");
		return;
	}

	if ((cfd = get_request(fd, NULL, NULL)) == -1)
	{
		err("get_request");
		return;
	}

	if ((pfd = tcp_connect(daddr)) == -1)
	{
		err("tcp_connect");
		goto connect_err;
	}

	s = alloc_new_session();
	if (s)
	{
		s->sockfd = cfd;
		if (add_to_session(s, pfd, EPOLLIN | EPOLLOUT, PRIVAT, priv_write) == -1)
		{
			err("add_to_session");
			goto add_err;
		}
	}
	else
	{	
		err("alloc_new_session");
		goto alloc_err;
	}

	return;
add_err:
	free(s);
alloc_err:
	close(pfd);
connect_err:
	close(cfd);
}

static void start_do(void)
{
	struct map_list *item;	
	int epfd, fd;
	struct event *e;
	
	if ((epfd = epoll_create(8192)) == -1)
	{			
		err("epoll_create");
		return;
	}

	for (item = mh.head; item != NULL; item = item->next)
	{
		if ((fd = tcp_listen(&item->saddr)) == -1)
		{
			err("tcp_listen");
			continue;
		}

		e = event_set(epfd, fd, EPOLLIN, &item->daddr, call_event_listen);
		if (!e || event_add(e) == -1)
		{
			err("event_set or event_add");
			free(e);	// if e is null, free nothing to do
		}
	}
	
	for (;;)
		event_dispatch_loop(epfd, 8192);
}

int main(int argc, char **argv)
{
	int ch;
	
	while (argc > 1 && ((ch = getopt(argc, argv, "vn:")) != -1))
	{
		switch (ch)
		{
			case 'v':
				verbose = 1;
				break;	
			case 'n':
				count = atoi(optarg);
				break;
			case '?':
				usage();
				exit(1);
		}
	}
		
	if (verbose == 0 && daemon(1, 1) == -1)
	{
		err("daemon");
		return -1;
	}
	
	if (init_signal_limit() == -1)
		err("init_signal_limit");
	
	if (init_map_list(&mh) == -1)
	{
		err("init_map_list");
		return -1;
	}

	if (init_gepoll(count) == -1)
	{
		err("init_gepoll");
		return -1;
	}
	if (init_thread(count) == -1)
	{	
		err("init_thread");
		return -1;
	}

	start_do();

	return 0;
}
