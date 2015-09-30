#pragma once
#include "reactor.h"
#include <sys/types.h> /* See NOTES */
#include <sys/socket.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

class SocketDataDecoderBase {
public:
    virtual int OnProcess(FDEventHanderBase* pSocket,
            const char* buf,
            unsigned int buf_len) = 0;
};

/********************************
 * 处理tcpServer事件的类
 ********************************/
class TcpServerEventHandler: public FDEventHanderBase {
public:
    TcpServerEventHandler(const char* ip,
            int port,
            SocketDataDecoderBase* decoder,
            int fd,
            Reactor* pReactor) : FDEventHanderBase(fd, pReactor), m_pDecoder(decoder) {
        memset(m_ip, 0x00, sizeof(m_ip));
        strncpy(m_ip, ip, strlen(ip));
        m_port = port;
    }

    virtual ~TcpServerEventHandler() {
    }

    // create listen socket. return 0, success.
    int OnListen();
    int OnAccept(int fd);
    // 当epoll中有新的连接过来的时候，就会调用该函数
    virtual void OnFDRead();
    virtual void OnFDWrite();
    virtual void Close();

protected:
    char m_ip[16]; //ipv4
    int m_port;
    SocketDataDecoderBase* m_pDecoder;
};

/* **************************
 * 处理TCP客户端发过来的数据
 * 这个地方，可以开辟2块缓冲区（读缓冲区和写缓冲区）
 * (1)读: 当socket可读时，epoll会被触发，回调OnFDRead(), 将读取到的数据进行处理，或者放到读缓冲区
 * (2)写: 当有数据需要通过socket发出去时, 先将数据放到写缓冲区中, 注册 RegisterWriteEvent 之后, OnFDWrite 就会被回调.
 * 若 LT 模式, 当数据没有写完时，OnFDWrite 一直会被回调, 直到写完后, 用户调用UnRegisteWriteEvent.
 * ************************/
class TcpSocketHandler: public FDEventHanderBase {
public:
    TcpSocketHandler(SocketDataDecoderBase* decoder,
            int fd,
            Reactor* pReactor) :
        FDEventHanderBase(fd, pReactor),
        m_dataDecoder(decoder) {

    }
    virtual ~TcpSocketHandler() {
    }
    // 读被触发时，调用该函数，处理客户端发过来的数据
    virtual void OnFDRead();
    // 写被触发时，调用该函数
    virtual void OnFDWrite();
    // 关闭fd
    virtual void Close();
    // 可以通过该函数，向客户端发送数据
    virtual int SendBuf(const char* buf, int buf_len);
private:
    SocketDataDecoderBase* m_dataDecoder;
    //Buffer* pReadBuf;
    //Buffer* pWriteBuf;
};

/* *********************************
 *          TcpServerBase类
 * *********************************/
class TcpServerBase {
public:
    TcpServerBase(const char* ip, int port, SocketDataDecoderBase* pDecoder)
    {
        memset(m_ip, 0x00, sizeof(m_ip));
        strncpy(m_ip, ip, strlen(ip));
        m_port = port;
        m_pReactor = NULL;
        m_pDecoder = pDecoder;
    }
    virtual int Init(){
        m_pReactor = new LTReactor;
        return m_pReactor->Init();
    }
    virtual int Run(){
        TcpServerEventHandler h(m_ip, m_port, m_pDecoder, 0, m_pReactor);
        if (0 == h.OnListen()) {
            m_pReactor->Run();
        }

        return 0;
    }
    virtual int Exit(){
        delete m_pReactor;
        return 0;
    }
private:
    char m_ip[16];//ipv4
    int m_port;
    Reactor* m_pReactor;
    SocketDataDecoderBase* m_pDecoder;
};


