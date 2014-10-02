

    #include "unixsocketeventhandler.h"


    class UnixSocketDataDecoder_echo_test: public SocketDataDecoderBase
    {
    public:
        virtual int OnProcess(FDEventHanderBase* pSocket, const char* buf,
             unsigned int buf_len)
        {
            std::cout << "RecvString: " << buf << std::endl;
            return ((UnixSocketHandler*)pSocket)->SendBuf(buf, buf_len);
        }
    };

    void UnixSocketEventHandler_test()
    {
        const char* path = "/home/ll/work/min_heap/unix_path.sock";
        UnixSocketDataDecoder_echo_test* pDecoder = new UnixSocketDataDecoder_echo_test;
        UnixSocketServerBase server(path, pDecoder);
        server.Init();
        server.Run();
        server.Exit();

        delete pDecoder;
        pDecoder = NULL;
    }

    // 客户端测试程序
    int f_unix_socket_client()
    {
        const char* path = "/home/ll/work/min_heap/unix_path.sock";
        int sockFd = socket(AF_UNIX, SOCK_STREAM, 0);
        if ( sockFd < 0 )
        {
            std::cout << "socket Error:" << strerror(errno) << std::endl;
            return -1;
        }

        sockaddr_un server;
        server.sun_family = AF_UNIX;
        strcpy(server.sun_path, path);
        // 直接去connect就好了，不需要再bind，否则会再生成一个socket文件。
        int iRet = connect(sockFd, (struct sockaddr*) &server, sizeof(server));
        if ( 0 != iRet )
        {
            std::cout << "connect Error:" << strerror(errno) << std::endl;
            close(sockFd);
            return 0;
        }
        while(true)
        {
            char inBuf[1024] = {0};
            std::cin >> inBuf;
            write(sockFd, inBuf, strlen(inBuf));

            char outBuf[1024] = {0};
            read(sockFd, outBuf, sizeof(outBuf));
            std::cout << "Echo: " << outBuf << std::endl;
        }

        return 0;
    }



