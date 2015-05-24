/*
 *gcc -g -o test init.c test.c
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

void init_daemon(void)
{
    int pid;
    int i;
    if(pid=fork())
        exit(0);//是父进程，结束父进程
    else if(pid< 0)
        exit(1);//fork失败，退出
    //是第一子进程，后台继续执行
    setsid();//第一子进程成为新的会话组长和进程组长
    //并与控制终端分离
    if(pid=fork())
        exit(0);//是第一子进程，结束第一子进程
    else if(pid< 0)
        exit(1);//fork失败，退出
    //是第二子进程，继续
    //第二子进程不再是会话组长
    for(i=0; i< NOFILE;++i)//关闭打开的文件描述符
        close(i);
    chdir("/tmp");//改变工作目录到/tmp
    umask(0);//重设文件创建掩模
    return;
}

void main()
{
    FILE *fp;
    time_t t;
    init_daemon();//初始化为Daemon

    while(1)//每隔一分钟向test.log报告运行状态 
    {
        sleep(60);//睡眠一分钟
        if((fp=fopen("test.log","a")) >=0)
        {
            t=time(0);
            fprintf(fp,"Im here at %s/n",asctime(localtime(&t)) ); 
            fclose(fp);
        }
    }
}
