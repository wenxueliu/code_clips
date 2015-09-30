/*
	（1）服务器端, 最好设置 tcp keepalive, 在服务器与客户端之间, 维持一个心跳连接,
         防止一些假死的客户端占用服务器端资源。【上面的代码中，并没有体现出来】
    （2）由于服务器端的fd, 是连续回收使用的, 所以当close(fd) 后, 需要将该 fd 的已经注册的读写 handler, 从EventHanderSet中删除掉。
    （3）在 epoll 的 LT 模式下, 在写之前, 需要注册 RegistReadEvent(), 之后，该 fd 可以一直写. 当写完后, 需要调用UnRegisterReadEvent()。
    （4）通常的接收和发送, 是需要接收/发送缓存的, 上面的代码中, 并没有体现出来.
*/
#include "tcpeventhandler.h"

/* *********************
 * 处理客户端发过来的数据
 * ********************/
class TcpDataDecoder_echo_test: public SocketDataDecoderBase {
public:
    // 真正的处理数据的函数
    virtual int OnProcess(FDEventHanderBase* pSocket, const char* buf,
     unsigned int buf_len)
    {
        // 这个地方，将客户端的数据回写
        if (0 == pSocket->RegisterWriteEvent()) {
            write(pSocket->GetFD(), buf, buf_len);
            pSocket->UnRegisterWriteEvent();
        }

        return 0;
    }
};


void TcpServer_test() {
    SocketDataDecoderBase* pDecoder = new TcpDataDecoder_echo_test;
    TcpServerBase b("127.0.0.1", 56893, pDecoder);
    b.Init();
    b.Run();
    b.Exit();
    delete pDecoder;
}
