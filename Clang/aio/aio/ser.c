#include"aio_rwfun.h"
#include"aio_fundef.h"
static long waitasync(int afd, int timeo) {
	struct pollfd pfd;

	pfd.fd = afd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	if (poll(&pfd, 1, timeo) < 0) {
		perror("poll");
		return -1;
	}
	if ((pfd.revents & POLLIN) == 0) {
		fprintf(stderr, "no results completed\n");
		return 0;
	}

	return 1;
}


int main(int ac, char **av) {
	int afd, fd;
	char const *testfn = "test.txt";

	BUILD_BUG_IF(sizeof(struct iocb) != 64);

	fprintf(stdout, "creating an eventfd ...\n");
	if ((afd = eventfd(0)) == -1) {
		perror("eventfd");
		return 2;
	}
	fprintf(stdout, "done! eventfd = %d\n", afd);
	if ((fd = open(testfn, O_RDWR | O_CREAT, 0644)) == -1) {
		perror(testfn);
		return 4;
	}
	ftruncate(fd, TESTFILE_SIZE);
	int n = TESTFILE_SIZE / IORTX_SIZE; 
    struct afdata *piocb = io_prep_afddata(fd,afd,n);
	char **wbuf = malloc(n * sizeof(char *));
	int i;
	for(i = 0;i < n;i ++)
	{
		wbuf[i] = malloc(IORTX_SIZE);
		sprintf(wbuf[i],"this is %d string\n",i);
		printf("%s\n",wbuf[i]);
	}
	//io_submit_write(piocb,fd,n,IORTX_SIZE,afd,wbuf);
	io_submit_read(piocb,fd,n,IORTX_SIZE,afd);
	waitasync(afd,-1);
	int nbuf;
	char **buf = io_obt_filebuf(piocb,afd,n,IORTX_SIZE,&nbuf);
	//int i;
	for(i = 0;i < nbuf; i ++)
	{
		printf("%d:\n%s\n",i,buf[i]);
	}
	io_destroy_afdata(piocb,n);
	close(fd);
	close(afd);

	return 0;
}
