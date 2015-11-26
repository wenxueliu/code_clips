#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAXLINE 300
#define	DEF_PAGER	"/bin/more"		/* default pager program */

int test_pipe1()
{
	int		n;
	int		fd[2];
	pid_t	pid;
	char	line[MAXLINE];
    int i = 0;

	if (pipe(fd) < 0)
    {
		printf("pipe error");
        return -1;
    }
    for(i = 0; i < 5; i++) {
        if ((pid = fork()) < 0) {
            printf("fork error");
            return -1;
        } else if (pid > 0) {		/* parent */
            close(fd[1]);
            n = read(fd[0], line, MAXLINE);
            write(STDOUT_FILENO, line, n);
        } else {					/* child */
            close(fd[0]);
            write(fd[1], "hello world\n", 12);
        }
    }
    return 0;
}

int test_pipe2(char *path)
{
	int		n;
	int		fd[2];
	pid_t	pid;
	char	*pager, *argv0;
	char	line[MAXLINE];
	FILE	*fp;

	if (NULL == path)
    {
		printf("path cannot be empty\n");
        return -1;
    }

	if ((fp = fopen(path, "r")) == NULL)
    {
		printf("can't open %s", path);
        return -1;
    }

	if (pipe(fd) < 0)
    {
		printf("pipe error");
        return -1;
    }

	if ((pid = fork()) < 0)
    {
		printf("fork error");
        return -1;
	}
    else if (pid > 0)
    {								/* parent */
		close(fd[0]);		/* close read end */

		/* parent copies argv[1] to pipe */
		while (fgets(line, MAXLINE, fp) != NULL)
        {
			n = strlen(line);
			if (write(fd[1], line, n) != n)
            {
				printf("write error to pipe");
                break;
            }
		}

		if (ferror(fp))
        {
			printf("fgets error");
            return -1;
        }
		close(fd[1]);	/* close write end of pipe for reader */

		if (waitpid(pid, NULL, 0) < 0)
        {
			printf("waitpid error");
            return -1;
        }
	}
    else
    {										/* child */
		close(fd[1]);	/* close write end */
		if (fd[0] != STDIN_FILENO) {
			if (dup2(fd[0], STDIN_FILENO) != STDIN_FILENO)
            {
				printf("dup2 error to stdin");
                return -1;
            }
			close(fd[0]);	/* don't need this after dup2 */
		}

		/* get arguments for execl() */
		if ((pager = getenv("PAGER")) == NULL)
        {
			pager = DEF_PAGER;
        }

		if ((argv0 = strrchr(pager, '/')) != NULL)
        {
			argv0++;		/* step past rightmost slash */
        }
		else
        {
			argv0 = pager;	/* no slash in pager */
        }

		if (execl(pager, argv0, (char *)0) < 0)
        {
			printf("execl error for %s", pager);
            return -1;
        }
	}
    return 0;
}

static int	pfd1[2], pfd2[2];

void TELL_WAIT(void)
{
	if (pipe(pfd1) < 0 || pipe(pfd2) < 0)
    {
		perror("pipe error");
    }
}

void TELL_PARENT(pid_t pid)
{
	if (write(pfd2[1], "c", 1) != 1)
    {
		perror("write error");
    }
}

void WAIT_PARENT(void)
{
	char c;
	if (read(pfd1[0], &c, 1) != 1)
    {
		perror("read error");
    }

	if (c != 'p')
    {
		perror("WAIT_PARENT: incorrect data");
    }
}

void TELL_CHILD(pid_t pid)
{
	if (write(pfd1[1], "p", 1) != 1)
    {
		perror("write error");
    }
}

void WAIT_CHILD(void)
{
	char c;
	if (read(pfd2[0], &c, 1) != 1)
    {
		perror("read error");
    }

	if (c != 'c')
    {
		perror("WAIT_CHILD: incorrect data");
    }
}

int main(void)
{
    char path[10] = "test";
    int ret;

    //if (ret = test_pipe1() < 0){
    //  exit(1);
    //}

    //if (ret = test_pipe2(path) < 0){
    //  exit(1);
    //}

    test_add();

	exit(0);
}
