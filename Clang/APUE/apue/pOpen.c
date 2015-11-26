#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define MAXLINE 300
#define	PAGER	"${PAGER:-less}" /* environment variable, or default */


int test_popen2(char *path)
{
	char	line[MAXLINE]; FILE	*fpin, *fpout;

	if (NULL == path)
    {
		printf("usage: a.out <pathname>");
        return -1;
    }
	if ((fpin = fopen(path, "r")) == NULL)
    {
        char err_msg[20];
        int n;
        printf("can't open %s", path);
        return -1;
    }

	if ((fpout = popen(PAGER, "w")) == NULL)
    {
		printf("popen error");
        return -1;
    }

	/* copy argv[1] to pager */
	while (fgets(line, MAXLINE, fpin) != NULL) {
		if (fputs(line, fpout) == EOF)
        {
			printf("fputs error to pipe");
            return -1;
        }
	}
	if (ferror(fpin))
    {
		printf("fgets error");
        return -1;
    }
	if (pclose(fpout) == -1)
    {
		printf("pclose error");
        return -1;
    }
}

/*
 * Pointer to array allocated at run-time.
 */
static pid_t	*childpid = NULL;

/*
 * From our open_max(), {Prog openmax}.
 */
static int		maxfd;

FILE * popen(const char *cmdstring, const char *type)
{
	int		i;
	int		pfd[2];
	pid_t	pid;
	FILE	*fp;

	/* only allow "r" or "w" */
	if ((type[0] != 'r' && type[0] != 'w') || type[1] != 0) {
		errno = EINVAL;
		return(NULL);
	}

	if (childpid == NULL) {		/* first time through */
		/* allocate zeroed out array for child pids */
		//maxfd = open_max();
        maxfd = 10;
		if ((childpid = calloc(maxfd, sizeof(pid_t))) == NULL){
			return(NULL);
        }
	}

	if (pipe(pfd) < 0){
		return(NULL);	/* errno set by pipe() */
    }

	if (pfd[0] >= maxfd || pfd[1] >= maxfd) {
		close(pfd[0]);
		close(pfd[1]);
		errno = EMFILE;
		return(NULL);
	}

	if ((pid = fork()) < 0) {
		return(NULL);	/* errno set by fork() */
	}
    else if (pid == 0) {							/* child */
		if (*type == 'r') {
			close(pfd[0]);
			if (pfd[1] != STDOUT_FILENO) {
				dup2(pfd[1], STDOUT_FILENO);
				close(pfd[1]);
			}
		} else {
			close(pfd[1]);
			if (pfd[0] != STDIN_FILENO) {
				dup2(pfd[0], STDIN_FILENO);
				close(pfd[0]);
			}
		}

		/* close all descriptors in childpid[] */
		for (i = 0; i < maxfd; i++){
			if (childpid[i] > 0){
				close(i);
            }
        }

		execl("/bin/sh", "sh", "-c", cmdstring, (char *)0);
		_exit(127);
	}

	/* parent continues... */
	if (*type == 'r') {
		close(pfd[1]);
		if ((fp = fdopen(pfd[0], type)) == NULL)
			return(NULL);
	}
    else {
		close(pfd[0]);
		if ((fp = fdopen(pfd[1], type)) == NULL)
			return(NULL);
	}

	childpid[fileno(fp)] = pid;	/* remember child pid for this fd */
	return(fp);
}

int pclose(FILE *fp)
{
	int		fd, stat;
	pid_t	pid;

	if (childpid == NULL) {
		errno = EINVAL;
		return(-1);		/* popen() has never been called */
	}

	fd = fileno(fp);
	if (fd >= maxfd) {
		errno = EINVAL;
		return(-1);		/* invalid file descriptor */
	}
	if ((pid = childpid[fd]) == 0) {
		errno = EINVAL;
		return(-1);		/* fp wasn't opened by popen() */
	}

	childpid[fd] = 0;
	if (fclose(fp) == EOF){
		return(-1);
    }

	while (waitpid(pid, &stat, 0) < 0){
		if (errno != EINTR){
			return(-1);	/* error other than EINTR from waitpid() */
        }
    }

	return(stat);	/* return child's termination status */
}

#include <ctype.h>

int myuclc()
{
    int c;
    while ((c = getchar()) != EOF){
        if (isupper(c)){
            c = tolower(c);
        }
        if (putchar(c) == EOF){
            printf("output error");
            return -1;
        }
        if (c == '\n'){
            fflush(stdout);
        }
    }
}


void test_popen()
{
	char	line[MAXLINE];
	FILE	*fpin;

	if ((fpin = popen("./myuclc", "r")) == NULL)
    {
		perror("popen error");
    }

	for ( ; ; )
    {
		fputs("prompt> ", stdout);
		fflush(stdout);
		if (fgets(line, MAXLINE, fpin) == NULL)	/* read from pipe */
        {
			break;
        }

        printf("%s",line);

        if ((strcmp(line,"exit\n") == 0) || (strcmp(line,"quit\n") == 0)){
            exit(0);
        }
		if (fputs(line, stdout) == EOF)
        {
			perror("fputs error to pipe");
        }
	}
	if (pclose(fpin) == -1)
    {
		perror("pclose error");
    }
	putchar('\n');
}



int main(int argc,char *argv)
{
    test_popen();
    //test_popen2("./pipe.c");
    //myuclc();

    exit(0);
}
