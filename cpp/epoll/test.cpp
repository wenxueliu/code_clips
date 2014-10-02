

    #include "reactor.h"
    #include <stdio.h>
    #include <string.h>
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <fcntl.h>

    class TimerEventHandler_test: public TimerEventHanderBase
    {
    public:
        TimerEventHandler_test(Reactor * pReactor) :
            TimerEventHanderBase(pReactor)
        {

        }
        void OnTimeOut() {
            timeval t;
            memset(&t, 0, sizeof(t));
            gettimeofday(&t, NULL);
            printf("timeOut: [%u:%u] \n", (unsigned int)t.tv_sec, (unsigned int)t.tv_usec);
        }
    };

    void reactor_test_1() {
        Reactor * pReactor = new LTReactor;
        pReactor->Init();
        TimerEventHanderBase* pHandler1 = new TimerEventHandler_test(pReactor);
        pHandler1->RegisterTimer(2000, false);
        TimerEventHanderBase* pHandler2 = new TimerEventHandler_test(pReactor);
        pHandler2->RegisterTimer(4000, false);
        TimerEventHanderBase* pHandler3 = new TimerEventHandler_test(pReactor);
        pHandler3->RegisterTimer(8000, false);
        TimerEventHanderBase* pHandler4 = new TimerEventHandler_test(pReactor);
        pHandler4->RegisterTimer(10000, false);
        pReactor->Run();
    }
    ////////////////////////////////////////////////////////////////////////////
    class FileFDEventHander: public FDEventHanderBase
    {
    public:
        FileFDEventHander(int fd, Reactor* pReactor) :
            FDEventHanderBase(fd, pReactor)
        {

        }

        // 读被触发时，调用该函数
        virtual void OnFDRead() {
            char buf[1024] = {0};
            read(m_fd, buf, sizeof(buf));
            printf("FDReadResult: [%s] \n", buf);
            UnRegisterReadEvent();
        }
        // 写被触发时，调用该函数
        virtual void OnFDWrite() {
            char buf[] = "this is a test file !";
            write(m_fd, buf, strlen(buf));

            UnRegisterWriteEvent();
        }
        // 关闭fd
        virtual void Close() {
            close(m_fd);
        }
    };

    void reactor_test_2()
    {
        int fd = -1;
        const char* path = "/home/ll/work/min_heap/test.txt";
        if(0 != mkfifo(path, 0777)) // 创建一个pipe.
        {
            fd = open(path, O_RDWR);
        }

        Reactor * pReactor = new LTReactor;
        pReactor->Init();

        FDEventHanderBase* pHandler = new FileFDEventHander(fd, pReactor);
        // 注册写事件，在命令行 cat pipe_file_name
    //    pHandler->RegisterWriteEvent();
    //    sleep(5);
    //    printf("sleep 5s over \n");
        // 注册读事件，在命令行 echo "dfsdf" > pipe_file_name
        pHandler->RegisterReadEvent();
        sleep(5);
        printf("sleep 5s over \n");
        pReactor->Run();
    }


