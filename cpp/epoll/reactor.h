#pragma once

#include <time.h>
#include <map>
#include <sys/time.h>
#include <fcntl.h>
#include "min_heap.h"

class FDEventHanderBase;
class FDEventHanderSet;
class TimerEventHanderBase;

class NonCopyable
{
protected:
    NonCopyable() {
    }
    virtual ~NonCopyable() {
    }
private:
    NonCopyable(const NonCopyable &); //禁止拷贝
    const NonCopyable & operator=(const NonCopyable &); //禁止赋值
};

class Reactor: public NonCopyable
{
public:
    Reactor() {
    }

    virtual ~Reactor() {
    }

    virtual bool Init() = 0;
    virtual bool Run() = 0;
    virtual bool Stop() = 0;
    virtual int RegisterReadEvent(FDEventHanderBase* pBase) = 0;
    virtual int RegisterWriteEvent(FDEventHanderBase* pBase) = 0;
    virtual int UnRegisterReadEvent(FDEventHanderBase* pBase) = 0;
    virtual int UnRegisterWriteEvent(FDEventHanderBase* pBase) = 0;
    virtual int RegisterTimer(TimerEventHanderBase* pBase) = 0;
    virtual int UnRegisterTimer(TimerEventHanderBase* pBase) = 0;
};
/* **********************************
 *             处理FD类的基类
 * 当该fd被触发时，调用处理函数来进行处理
 * ***********************************/
class FDEventHanderBase: public NonCopyable
{
public:
    FDEventHanderBase(int fd, Reactor* pReactor) :
        m_fd(fd), m_pReactor(pReactor)
    {
    }
    virtual ~FDEventHanderBase() {
    }
    // 读被触发时，调用该函数
    virtual void OnFDRead() = 0;
    // 写被触发时，调用该函数
    virtual void OnFDWrite() = 0;
    // 关闭fd
    virtual void Close() = 0;
    // 注册读事件
    int RegisterReadEvent();
    // 注册写事件
    int RegisterWriteEvent();
    // 注销读事件
    int UnRegisterReadEvent();
    // 注销写事件
    int UnRegisterWriteEvent();
    int GetFD() const;
    void SetFD(int fd);
    // 设置非阻塞，返回0，success.
    int SetNonBlock(int fd, bool bNonBlock = true);
protected:
    int m_fd;//该handler所对应的fd.
    Reactor* m_pReactor;
};
/* **********************************
 *        处理timer类的基类
 *    当timer被触发时，调用该类的函数
 * **********************************/
class TimerEventHanderBase: public NonCopyable
{
public:
    TimerEventHanderBase(Reactor* reactor) :
        m_pReactor(reactor), m_bRestart(false)
    {
        timerclear(&m_interval);
        timerclear(&m_endtime);
    }
    virtual ~TimerEventHanderBase() {
    }
    virtual void OnTimeOut() = 0;
    // 注册定时器，@msec，多长时间响一次,单位:毫秒
    // 由于epoll精度的限制，这个地方，精度只能达到 毫秒，但不准确
    int RegisterTimer(unsigned int msec /*ms*/, bool restart = false);
    int UnRegisterTimer();
    int RegisterTimerAgain();
    // 获取结束时间
    const timeval& GetEndTime() const {
        return m_endtime;
    }
    // 该定时器是否需要重启
    bool IsRestart() const {
        return m_bRestart;
    }
protected:
    Reactor* m_pReactor;
    timeval m_interval;// 时间间隔
    timeval m_endtime; // 定时器到期时间
    bool m_bRestart;// 是否自动重启
};

/* **********************************
 *             处理类的集合
 *     有个对应关系 -- fd -- 处理函数 -- read/write.
 * *********************************/
class EventHanderSet
{
public:
    // 增加fd事件, @type : EPOLLIN, EPOLLOUT，
    void AddFDEventHandler(FDEventHanderBase* pHandler, int type);
    // @type : EPOLLIN, EPOLLOUT
    void DelFDEventHandler(int fd, int type);
    // 根据fd，找到 FDEventHanderBase，及type.
    FDEventHanderBase* GetFDEventHandler(int fd, int& type);
public:
    void AddTimerEventHandler(TimerEventHanderBase* pHandler);
    void DelTimerEventHandler(TimerEventHanderBase* pHandler);
    void ScanTimer();
private:
    struct FDHandler
    {
        int type; //EPOLLIN, EPOLLOUT
        FDEventHanderBase* pHandler;
    };
    std::map<int, FDHandler> m_fdmap; // fd -- type -- handler

    // 用于保存timer，在同一时刻，可能有多个handler.
    MinHeap<std::pair<long int, long int> > m_timerMinHeap;
    std::multimap<std::pair<long int, long int>, TimerEventHanderBase*> m_timerMultiMap;
};

//////////// LT epoll /////////////////
class LTReactor: public Reactor
{
public:
    LTReactor();
    virtual ~LTReactor();

    bool Init();
    bool Run();
    bool Stop();
    //注册读事件
    int RegisterReadEvent(FDEventHanderBase* pBase);
    //注册写事件
    int RegisterWriteEvent(FDEventHanderBase* pBase);

    int UnRegisterReadEvent(FDEventHanderBase* pBase);
    int UnRegisterWriteEvent(FDEventHanderBase* pBase);
    int UnRegisterAllEvent(FDEventHanderBase* pBase);
    // 注册定时器
    int RegisterTimer(TimerEventHanderBase* pBase);
    int UnRegisterTimer(TimerEventHanderBase* pBase);
private:
    void ScanTimer();
private:
    int m_epfd; //epoll's fd
    bool m_bRunning;
    timeval m_stoptime; //停止时间
    EventHanderSet m_handlerSet;
};
