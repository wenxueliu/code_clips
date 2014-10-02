

    #include "tcpeventhandler.h"

    /********************************
     * 处理tcpServer事件的类
     ********************************/
    // create listen socket. return 0, success.
    int TcpServerEventHandler::OnListen() {
        m_fd = ::socket(AF_INET, SOCK_STREAM, 0);
        if (m_fd == -1)
        {
            std::cout << "create socket error." << strerror(errno) << std::endl;
            return -1;
        }
        sockaddr_in addr;
        memset(&addr, 0x00, sizeof(sockaddr_in));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(m_port);
        socklen_t len = sizeof(sockaddr_in);

        if (1 != inet_pton(AF_INET, m_ip, &addr.sin_addr))
        {
            std::cout << "inet_pton error." << strerror(errno) << std::endl;
            close(m_fd);
            return -1;
        }
        // 允许在同一端口上启动同一服务器的多个实例
        int opVal = 1;
        if (0 != setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &opVal, sizeof(opVal)))
        {
            std::cout << "socketopt error." << strerror(errno) << std::endl;
            close(m_fd);
            return -1;
        }
        // 设置非阻塞模式
        SetNonBlock(m_fd, true);
        // bind
        if (-1 == bind(m_fd, (struct sockaddr*) &addr, len))
        {
            std::cout << "bind error." << strerror(errno) << std::endl;
            close(m_fd);
            return -1;
        }
        // listen
        if (-1 == listen(m_fd, 200))
        {
            std::cout << "listen error." << strerror(errno) << std::endl;
            close(m_fd);
            return -1;
        }

        // 将该fd加入到 epoll中，监听客户端的连接
        if (0 != RegisterReadEvent())
        {
            std::cout << "RegisterReadEvent error." << std::endl;
            close(m_fd);
            return -1;
        }

        return 0;
    }

    int TcpServerEventHandler::OnAccept(int fd) {
        TcpSocketHandler* pHandler = new TcpSocketHandler(m_pDecoder, fd,
         this->m_pReactor);
        // 注册一个读事件，从TCP的客户端读取数据.
        if (0 != pHandler->RegisterReadEvent())
        {
            pHandler->Close();
            delete pHandler;
            pHandler = NULL;
        }
        // 需要delete pHandler
        return 0;
    }
    // 当epoll中有新的连接过来的时候，就会调用该函数
    void TcpServerEventHandler::OnFDRead() {
        sockaddr_in clientAddr;
        socklen_t len = sizeof(clientAddr);
        // 客户端的fd，这个地方，其实可以建立一个(fd -- 客户端)对应关系
        int sockFD = accept(m_fd, (struct sockaddr*) &clientAddr, &len);
        if (sockFD < 0)
        {
            std::cout << "accept error." << std::endl;
            return;
        }
        else
        {
            int port = ntohs(clientAddr.sin_port);
            char addr[256] =
            { 0 };
            inet_ntop(AF_INET, &clientAddr.sin_addr.s_addr, addr,
             (socklen_t) sizeof(addr));
            printf("accept: [%s:%d] \n", addr, port);
        }

        OnAccept(sockFD);
    }
    void TcpServerEventHandler::OnFDWrite() {

    }
    void TcpServerEventHandler::Close() {
        UnRegisterReadEvent();
        UnRegisterWriteEvent();
        close(m_fd);
    }

    /* ***********************
     * 处理TCP客户端发过来的数据
     * ************************/
    // 读被触发时，调用该函数，处理客户端发过来的数据
    void TcpSocketHandler::OnFDRead() {
        // 在这个函数中，可以读取客户端发过来的数据，并对数据进行分析，TCP的数据，
        // 可能会出现半包的情况，所以一般会有一个缓存来保存没有处理完毕的数据
        char buf[1024] =
        { 0 };
        int iRet = read(m_fd, buf, sizeof(buf));
        if (0 == iRet)
        {
            Close();// 客户端关闭socket.
            return;
        }
        // 回调处理函数，对数据进行处理
        m_dataDecoder->OnProcess(this, buf, iRet);
    }
    // 写被触发时，调用该函数
    void TcpSocketHandler::OnFDWrite() {

    }
    // 关闭fd
    void TcpSocketHandler::Close() {
        UnRegisterReadEvent();
        UnRegisterWriteEvent();
        close(m_fd);
        delete this;
    }
    // 可以通过该函数，向客户端发送数据
    int TcpSocketHandler::SendBuf(const char* buf, int buf_len) {
        RegisterWriteEvent();
        int iRet = write(m_fd, buf, buf_len);
        UnRegisterWriteEvent();
        return iRet;
    }


