#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <string.h>
#include <ctype.h>

#define MAXLINE 300
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

    exit(0);
}
