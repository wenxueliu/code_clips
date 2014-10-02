

    #pragma once

    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include <sys/types.h> /* See NOTES */
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <sys/un.h>
    #include <iostream>
    #include <errno.h>
    #include <sys/stat.h>

    #include "reactor.h"
    #include "SocketDataDecoderBase.h"

    #define MAX_PATH 256

    /* ********************************
     *    处理 unix socket server事件
     * ********************************/
    class UnixSocketServerEventHandler: public FDEventHanderBase
    {
    public:
        UnixSocketServerEventHandler(const char* path, SocketDataDecoderBase* decoder, int fd, Reactor* pReactor) :
            FDEventHanderBase(fd, pReactor), m_pDecoder(decoder)
        {
            assert(strlen(path) > 0);
            strncpy(m_path, path, sizeof(m_path));
            unlink(m_path);
        }
        ~UnixSocketServerEventHandler() {

        }
        // create unix socket. return 0, success.
        int OnListen();
        int OnAccept(int fd);
        // 读被触发时，调用该函数
        virtual void OnFDRead();
        // 写被触发时，调用该函数
        virtual void OnFDWrite() {

        }
        // 关闭fd
        virtual void Close() {
            unlink(m_path);
            close(m_fd);
        }
    private:
        char m_path[MAX_PATH];
        SocketDataDecoderBase* m_pDecoder;
    };

    /* ********************************
     *    处理 unix socket 客户端事件
     * ********************************/
    class UnixSocketHandler: public FDEventHanderBase
    {
    public:
        UnixSocketHandler(SocketDataDecoderBase* decoder, int fd, Reactor* pReactor) :
            FDEventHanderBase(fd, pReactor), m_pDecoder(decoder)
        {

        }
        ~UnixSocketHandler(){}
        virtual void OnFDRead();
        virtual void OnFDWrite();
        virtual void Close();
        virtual int SendBuf(const char* buf, int buf_len);
    private:
        SocketDataDecoderBase* m_pDecoder;
    };

    /* *********************************
     *          UnixSocketServerBase类
     * *********************************/
    class UnixSocketServerBase
    {
    public:
        explicit UnixSocketServerBase(const char* path, SocketDataDecoderBase* pDecoder)
        {
            memset(m_path, 0x00, sizeof(m_path));
            strncpy(m_path, path, strlen(path));
            m_pReactor = NULL;
            m_pDecoder = pDecoder;
        }
        virtual int Init()
        {
            m_pReactor = new LTReactor;
            return m_pReactor->Init();
        }
        virtual int Run()
        {
            UnixSocketServerEventHandler h(m_path, m_pDecoder, 0, m_pReactor);
            if (0 == h.OnListen())
            {
                m_pReactor->Run();
            }

            return 0;
        }
        virtual int Exit()
        {
            delete m_pReactor;
            return 0;
        }
    private:
        UnixSocketServerBase(const UnixSocketServerBase&);
        const UnixSocketServerBase& operator=(const UnixSocketServerBase&);
    private:
        char m_path[MAX_PATH];
        Reactor* m_pReactor;
        SocketDataDecoderBase* m_pDecoder;
    };


