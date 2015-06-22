#include</usr/include/i386-linux-gnu/sys/epoll.h>
#include<fcntl.h>
#include</usr/include/i386-linux-gnu/sys/socket.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
#include</usr/include/i386-linux-gnu/sys/sendfile.h>
#include </usr/include/i386-linux-gnu/sys/stat.h>
//#include<iostream>
#include<time.h>
#include</usr/include/i386-linux-gnu/sys/time.h>

#include"aio_rwfun.h"

#define MAXFD (1024)
#define SERVERIP "127.0.0.1"
#define SERVERPORT (8080)
#define LISTENNQ (20)
#define MAXEVENT (20)
#define TIMEOUT (-1)
#define MAXLINE (1024)
#define BUFFSIZE 1024
static int offset = 0;
//static int index = 0;
static int total = 0;
static int filesize = 0;
static int nbytes = 0;
static int afd;
static char *wbuf;
static struct afdata *piocb;
static struct timeval t_start;
int Set_Nonblocking(int sock) //将文件描述符设置为非阻塞模式
{
	int opts;
	if ((opts = fcntl(sock, F_GETFL) < 0)) {
		perror("fcntl get");
		return (opts);
	}
	opts = opts | O_NONBLOCK;
	if (fcntl(sock, F_SETFL, opts) < 0) {
		perror("fcntl set");
		return (-1);
	}
	return (0);
}

int main() {
	int i, listenfd, connfd, sockfd, epfd, nfds;
	struct epoll_event ev, events[20];

	if ((epfd = epoll_create(MAXFD)) < 0) {//创建EPOLL描述符
		perror("epoll_create");
		return (epfd);
	}

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	Set_Nonblocking(listenfd);
	ev.data.fd = listenfd;
	ev.events = EPOLLIN | EPOLLET;
	epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);

	struct sockaddr_in serveraddr;
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_aton(SERVERIP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVERPORT);
	bind(listenfd, (struct sockaddr*) &serveraddr, sizeof(serveraddr));

	listen(listenfd, LISTENNQ);

	struct sockaddr_in clientaddr;
	socklen_t clilen;
	int readbytes;
	char readbuf[MAXLINE];
	for (;;) {
		if ((nfds = epoll_wait(epfd, events, MAXEVENT, TIMEOUT)) < 0) {
			perror("epoll_wati");
			break;
		} else if (nfds == 0) {
			printf("no event or timeout\n");
			continue;
		}

		for (i = 0; i < nfds; i++) {
			if (events[i].data.fd == listenfd) {
				if ((connfd = accept(listenfd, (struct sockaddr*) &clientaddr,
						&clilen)) < 0) {
					perror("accept");
					return (connfd);
				}

				Set_Nonblocking(connfd);

				char *cliaddr = inet_ntoa(clientaddr.sin_addr);
				printf("connect from:%s\n", cliaddr);
				ev.data.fd = connfd;
				ev.events = EPOLLIN | EPOLLET;
				epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev);
				continue;
			} else if (events[i].events & EPOLLIN) {
				if ((sockfd = events[i].data.fd) < 0)
					continue;
				memset(readbuf, 0, MAXLINE);
				

                if(events[i].data.fd == afd)
				{
					printf("afd event\n");
					int nbuf;
					char **buf = io_obt_filebuf(piocb,afd,total,BUFFSIZE,&nbuf);
					int i;
					wbuf = malloc(filesize);
					for(i = 0;i < nbuf; i++)
					{
						//printf("\n%d:\n%s\n",i,buf[i]);
						memcpy(wbuf + i * BUFFSIZE,buf[i],BUFFSIZE);
					}
					int n = filesize / BUFFSIZE;
				    io_destroy_afdata(piocb,n);
					close(afd);

					n = send(connfd,wbuf,filesize,0);
                    if(n == -1)
					{
						perror("send");
					}
                    else
					{
						if(n == filesize)
						{
							;//free(wbuf);	
						}
						else
						{
							wbuf = wbuf + n;
							nbytes += n;
							ev.events = EPOLLOUT | EPOLLET;
							epoll_ctl(epfd, EPOLL_CTL_MOD, connfd, &ev);
							printf("send:%d,%d\n",n,filesize);
						}
                    }
					continue;
				}

				if ((readbytes = recv(sockfd, readbuf, MAXLINE, 0)) < 0) {
					if (errno == ECONNRESET) {
						perror("connectreset");
						close(sockfd);
						events[i].data.fd = -1;
						epoll_ctl(epfd, EPOLL_CTL_DEL, sockfd, NULL);
					} else {
						perror("read err\n");
						continue;
					}
				} else if (readbytes == 0) {
					printf("socket close\n");
					close(sockfd);
					events[i].data.fd = -1;
					epoll_ctl(epfd, EPOLL_CTL_DEL, sockfd, NULL);
					continue;
				}
				
				struct stat filestat;
				stat(readbuf, &filestat);
				int fd = open(readbuf, O_RDONLY);
				gettimeofday(&t_start,NULL);
				total = filestat.st_size / BUFFSIZE + 1;
				filesize = filestat.st_size;
				
				fprintf(stdout, "creating an eventfd ...\n");
				if ((afd = eventfd(0)) == -1) {
					perror("eventfd");
					return 2;
				}
                
				piocb = io_prep_afddata(fd,afd,total);	
                if(io_submit_read(piocb,fd,total,BUFFSIZE,afd) == -1)
				{
					perror("io_submit_read");
				}
				
				//Set_Nonblocking(afd);
				ev.data.ptr = NULL;
				ev.events = EPOLLIN | EPOLLET;
				ev.data.fd = afd;
				if(epoll_ctl(epfd, EPOLL_CTL_ADD, afd, &ev))
				{
					perror("epoll add");
				}

			} else if (events[i].events & EPOLLOUT) {
				sockfd = events[i].data.fd;
				int ret = send(connfd,wbuf,filesize - nbytes,0);
				if(ret == -1)
				{
					perror(strerror(errno));
				}
				else
				{
					printf("%d\n",ret);
				    if(nbytes < filesize)
					{
						ev.events = EPOLLOUT | EPOLLET;
						epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev);
						nbytes  +=ret;		
						wbuf += ret;
						printf("send:%d,%d\n",ret,filesize);
					}
					else
					{
     					struct timeval t_end;
					    gettimeofday(&t_end,NULL);
						long start = ((long)t_start.tv_sec)*1000 + (long)t_start.tv_usec/1000;

						long end = ((long)t_end.tv_sec)*1000 + (long)t_end.tv_usec/1000;
						printf("cost time:%ld\n",end - start);
						ev.events = EPOLLIN | EPOLLET;
						epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev);
						nbytes  = 0;
						printf("success:%d\n",nbytes);
						//close(fd);

					}
				}
			} else if ((events[i].events & EPOLLHUP) || (events[i].events
					&EPOLLERR)) {
				printf("fd close or err\n");
				sockfd = events[i].data.fd;
				close(sockfd);
				epoll_ctl(epfd, EPOLL_CTL_DEL, sockfd, NULL);
			}
		}
	}
	printf("end:");
	perror(strerror(errno));
	return (0);
}
