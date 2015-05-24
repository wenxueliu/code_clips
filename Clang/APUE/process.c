#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>


int     globvar = 6;
char    buf[] = "a write to stdout\n";

void test_process()
{
    int     var;
    pid_t  p_id;
    if ((p_id = fork()) < 0)
    {
    }
    else if (p_id == 0)
    {
        globvar++;
        var++;
        sleep(2);
    }
    else
    {
        //sleep(2);
    }

    printf("ppid = %d, pid = %ld, glob = %d, var = %d",
        getppid(),(long)getpid(), globvar,var);
}

void test_process1()
{
    int     var;
    pid_t  p_id;
    if ((p_id = fork()) < 0)
    {
    }
    else if (p_id == 0)
    {
        globvar++;
        var++;
    }
    else
    {
        sleep(2);
    }

    printf("ppid = %d, pid = %ld, glob = %d, var = %d",
        getppid(),(long)getpid(), globvar,var);
}

void test_process2()
{
    int     var;
    pid_t  p_id;
    if (write(STDOUT_FILENO, buf, sizeof(buf)-1) != sizeof(buf)-1)
    {
        printf("write error");
        exit(EXIT_FAILURE);
    }
    printf("before fork\n");

    if ((p_id = fork()) < 0)
    {
    }
    else if (p_id == 0)
    {
        globvar++;
        var++;
    }
    else
    {
        sleep(2);
    }

    printf("ppid = %d, pid = %ld, glob = %d, var = %d",
        getppid(),(long)getpid(), globvar,var);
}

void test_process3()
{
    int     var;
    pid_t  p_id;
    if (write(STDOUT_FILENO, buf, sizeof(buf)-1) != sizeof(buf)-1)
    {
        printf("write error");
        exit(EXIT_FAILURE);
    }
    printf("before fork");

    if ((p_id = fork()) < 0)
    {
    }
    else if (p_id == 0)
    {
        globvar++;
        var++;
    }
    else
    {
        sleep(2);
    }

    printf("ppid = %d, pid = %ld, glob = %d, var = %d",
        getppid(),(long)getpid(), globvar,var);
}

void get_childpid()
{
    pid_t  p_id;
    if ((p_id = fork()) < 0)
    {
        printf("fork error");
        exit(EXIT_FAILURE);
    }
    else if (p_id == 0)
    {
        if ((p_id = fork()) < 0)
        {
            printf("fork error");
            exit(EXIT_FAILURE);
        }
        else if (p_id > 0)
        {
            printf("ppid = %d, first child pid = %ld", getppid(),(long)getpid() );
            exit(EXIT_SUCCESS);
        }

        sleep(2);
        printf("ppid = %d, second child pid = %ld", getppid(),(long)getpid() );
        exit(0);
    }

    if (waitpid(p_id, NULL, 0) != p_id)
    {
        printf("waitpid error\n");
    }
    exit(EXIT_SUCCESS);
}

void pid_about()
{
    pid_t  gpid = getpgrp();
    pid_t  gpid1 = getpgid(gpid);
    pid_t  ppid = getppid();
    pid_t  pid = getpid();

    printf("pid : %d\n",(int)pid);
    printf("parent pid : %d\n",(int)ppid);
    printf("group pid : %d\n",(int)gpid);
    printf("group pid : %d\n",(int)gpid1);

    if (pid == gpid)
    {
        printf("this is process group leader\n");
    }
}

void set_sessionid()
{
    pid_t spid = getsid(0);
    printf("get session id : %d\n", spid);

    pid_t pid = 0;
    if ((pid = fork()) < 0)
    {
        perror("fork error");
    }
    else if (pid > 0)
    {
    }
    else
    {
        pid_t spid = setsid();
        printf("set session id : %d\n", spid);
        pid_about();
    }
}

int main(void)
{
    //test_process();
    //test_process1();
    //get_childpid();
    pid_about();
    //set_sessionid();

    exit(EXIT_SUCCESS);
}
