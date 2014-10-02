

    #include "unixsocketeventhandler.h"

    // create unix socket. return 0, success.
    int UnixSocketServerEventHandler::OnListen() {
        m_fd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (m_fd == -1)
        {
            std::cout << "create socket error." << strerror(errno) << std::endl;
            return -1;
        }

        sockaddr_un addr;
        memset(&addr, 0, sizeof(addr));
        addr.sun_family = AF_UNIX;
        strcpy(addr.sun_path, m_path);

        // 设置非阻塞模式
        SetNonBlock(m_fd, true);
        // 在bind的时候，会生成socket文件。
        socklen_t len = offsetof(struct sockaddr_un, sun_path)+strlen(addr.sun_path);
        if (bind(m_fd, (struct sockaddr *) &addr, len) != 0)
        {
            std::cout << "unix socket bind error,path = " << m_path << std::endl;
            return -1;
        }

        if (listen(m_fd, 500) != 0)
        {
            std::cout << "unix listen error" << std::endl;
            return -1;
        }
        // 更改socket文件的访问权限.
        if (0 != chmod(m_path, 0777))
        {
            std::cout << "chmod[777] " << m_path << "error " << strerror(errno)
             << std::endl;
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

    // 读被触发时，调用该函数
    void UnixSocketServerEventHandler::OnFDRead() {
        sockaddr_un clientAddr;
        memset(&clientAddr, 0x00, sizeof(sockaddr_un));
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
            len -= offsetof(struct sockaddr_un, sun_path);
            clientAddr.sun_path[len] = 0;
            std::cout << "accpet path = " << clientAddr.sun_path << std::endl;
        }

        OnAccept(sockFD);
    }

    int UnixSocketServerEventHandler::OnAccept(int fd) {
        SetNonBlock(fd, true);
        UnixSocketHandler* pHandler = new UnixSocketHandler(m_pDecoder, fd,
         this->m_pReactor);
        if (pHandler == NULL)
        {
            return -1;
        }
        if (0 != pHandler->RegisterReadEvent())
        {
            pHandler->Close();
            delete pHandler;
            pHandler = NULL;
            return -1;
        }
        return 0;
    }

    /* ********************************
     *    处理 unix socket 客户端事件
     * ********************************/
    void UnixSocketHandler::OnFDRead() {
        char buf[1024] =
        { 0 };
        int iRet = read(m_fd, buf, sizeof(buf));
        if (0 == iRet)
        {
            Close();// 客户端关闭socket.
            return;
        }
        // 回调处理函数，对数据进行处理
        m_pDecoder->OnProcess(this, buf, iRet);
    }

    void UnixSocketHandler::Close() {
        UnRegisterReadEvent();
        UnRegisterWriteEvent();
        close(m_fd);
        delete this;
    }

    void UnixSocketHandler::OnFDWrite() {

    }

    // 可以通过该函数，向客户端发送数据
    int UnixSocketHandler::SendBuf(const char* buf, int buf_len) {
        RegisterWriteEvent();
        int iRet = write(m_fd, buf, buf_len);
        UnRegisterWriteEvent();
        return iRet;
    }


