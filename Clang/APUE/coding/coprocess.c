#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>

#define MAXLINE 300

static void	sig_pipe(int);		/* our signal handler */

int coprocess(void)
{
	int		n, fd1[2], fd2[2];
	pid_t	pid;
	char	line[MAXLINE];

	if (signal(SIGPIPE, sig_pipe) == SIG_ERR){
		printf("signal error");
        return -1;
    }

	if (pipe(fd1) < 0 || pipe(fd2) < 0){
		printf("pipe error");
        return -1;
    }

	if ((pid = fork()) < 0) {
		printf("fork error");
        return -1;
	}
    else if (pid > 0) {							/* parent */
		close(fd1[0]);
		close(fd2[1]);

		while (fgets(line, MAXLINE, stdin) != NULL) {
			n = strlen(line);
			if (write(fd1[1], line, n) != n){
				printf("write error to pipe");
                return -1;
            }
			if ((n = read(fd2[0], line, MAXLINE)) < 0){
				printf("read error from pipe");
                return -1;
            }
			if (n == 0) {
				printf("child closed pipe");
				break;
			}
			line[n] = 0;	/* null terminate */
			if (fputs(line, stdout) == EOF){
				printf("fputs error");
            }
		}

		if (ferror(stdin)){
			printf("fgets error on stdin");
            return -1;
        }
	}
    else {									/* child */
		close(fd1[1]);
		close(fd2[0]);
		if (fd1[0] != STDIN_FILENO) {
			if (dup2(fd1[0], STDIN_FILENO) != STDIN_FILENO){
				printf("dup2 error to stdin");
            }
			close(fd1[0]);
		}

		if (fd2[1] != STDOUT_FILENO) {
			if (dup2(fd2[1], STDOUT_FILENO) != STDOUT_FILENO){
				printf("dup2 error to stdout");
            }
			close(fd2[1]);
		}
		//if (execl("./add", "add", (char *)0) < 0){
		if (execl("./add2", "add2", (char *)0) < 0){
			printf("execl error");
        }
	}
	return 0;
}

static void sig_pipe(int signo)
{
	printf("SIGPIPE caught\n");
	exit(1);
}

int test_add()
{
	int		n, int1, int2;
	char	line[MAXLINE];

	while ((n = read(STDIN_FILENO, line, MAXLINE)) > 0) {
		line[n] = 0;		/* null terminate */
		if (sscanf(line, "%d%d", &int1, &int2) == 2) {
			sprintf(line, "%d\n", int1 + int2);
			n = strlen(line);
			if (write(STDOUT_FILENO, line, n) != n){
				printf("write error");
                break;
            }
		}
        else {
			if (write(STDOUT_FILENO, "invalid args\n", 13) != 13){
				printf("write error");
                break;
            }
            //else{
            //    break;
            //}
		}
	}
}

int test_add2()
{
	int		int1, int2;
	char	line[MAXLINE];

    if (setvbuf(stdin, NULL, _IOLBF, 0) != 0){
        printf("setvbuf to stdin error");
        return -1;
    }
    if (setvbuf(stdout, NULL, _IOLBF, 0) != 0){
        printf("setvbuf to stdout error");
        return -1;
    }
	while (fgets(line, MAXLINE, stdin) != NULL) {
		if (sscanf(line, "%d%d", &int1, &int2) == 2) {
			if (printf("%d\n", int1 + int2) == EOF){
				printf("printf error");
                return -1;
            }
		}
        else {
			if (printf("invalid args\n") == EOF){
				printf("printf error");
                return -1;
            }
		}
	}

}

int main(int argc, char *argv[])
{
    int ret;
    if ((ret = coprocess()) == 0)
    {
        return 1;
    }
    //test_add();
    //test_add2();
    exit(0);
}
