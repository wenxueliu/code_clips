#include<stdio.h>
#include<netdb.h>
#include</usr/include/i386-linux-gnu/sys/socket.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<fcntl.h>
#define MAXADDRLEN 256
#define BUFLEN 10240

int main() {
	int sockfd, err;
	char buf[BUFLEN];
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8080);
	strncpy(buf, "127.0.0.1", BUFLEN);
	inet_pton(AF_INET, buf, &addr.sin_addr);

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) != 0)
		err = errno;
	if (connect(sockfd, (struct sockaddr*) &addr, sizeof(addr)) < 0)
		err = errno;
	else {
		while(1)
		{
		int n;
		char buf[BUFLEN];
		printf("recv\n");
		memset(buf, 0, BUFLEN);
		strcpy(buf, "gaoqing.ts");
		send(sockfd, buf, strlen(buf), 0);
	    memset(buf, 0, BUFLEN);
		int fd = open("test.ts",O_APPEND|O_WRONLY);
		int total = 0;
		while ((n = recv(sockfd, buf, BUFLEN, 0)) > 0)
        {
			write(fd, buf, n);
			total += n;
			printf("recv:%d,%d\n",n,total / (1024 * 1024));
			memset(buf, 0, BUFLEN);
		}
		if (n < 0)
			perror("recv");
		close(fd);
		printf("while end\n");
		}
	}
	close(sockfd);
	printf("end%s\n",strerror(err));
	exit(1);
}
