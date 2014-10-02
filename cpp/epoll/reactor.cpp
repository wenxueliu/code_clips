#include "reactor.h"
#include <sys/epoll.h>
#include <errno.h>
#include <iostream>
#include <string.h>
#include <stdio.h>

/////////////////////////////////////////////////////////
#define MAX_EPOLL_SIZE 1024
#define MAX_EPOLL_EVENTS_SIZE 128

LTReactor::LTReactor() :
    m_epfd(-1), m_bRunning(false)
{
}

LTReactor::~LTReactor() {
}

bool LTReactor::Init() {
    //TODO more exactual error check
    m_epfd = epoll_create(MAX_EPOLL_SIZE);
    if (m_epfd == -1)
    {
        std::cout << "epoll_create error: " << strerror(errno) << std::endl;
        return false;
    }

    m_bRunning = true;
    return true;
}

bool LTReactor::Run() {
    struct epoll_event events[MAX_EPOLL_EVENTS_SIZE];
    int nfds = 0; //返回被触发的事件的个数
    while (true)
    {
        // 超时时间，若m_bRunning，立即返回；否则100ms，即0.1s. 实际上，epoll_wait，只能够精确到毫秒(1/1000)
        nfds = epoll_wait(m_epfd, events, sizeof(events) / sizeof(events[0]),
         m_bRunning ? 100 : 1);
        // 要停止epoll.
        if (m_bRunning == false)
        {
            timeval now;
            gettimeofday(&now, NULL); //获取当前时间
            if (timercmp(&now, &m_stoptime, >))
            {
                break;
            }
        }
        // epoll 出错
        if (nfds == -1)
        {
            std::cout << "epoll_wait error: " << strerror(errno) << std::endl;
            continue;
        }
        // 处理被触发的事件
        for (int i = 0; i < nfds; ++i)
        {
            int type = 0;
            int fd = events[i].data.fd;
            if (fd < 0) //fd出错
            {
                std::cout << "the FD is " << fd << std::endl;
                continue;
            }
            FDEventHanderBase* pBase = m_handlerSet.GetFDEventHandler(fd, type);
            if (NULL == pBase)
            {
                std::cout << "pBase is NULL, fd is" << fd << std::endl;
                continue;
            }

            if (events[i].events & EPOLLIN)//read events
            {
                pBase->OnFDRead();
            }
            if (events[i].events & EPOLLOUT) //write events
            {
                pBase->OnFDWrite();
            }
            if (events[i].events & EPOLLERR) // error events
            {
                pBase->Close();
            }
        }
        // 处理定时器事件
        ScanTimer();
        // 处理idle事件
    } // end of while

    std::cout << "stop epoll" << std::endl;
    close(m_epfd); //close epoll fd
    m_epfd = -1;

    return true;
}

bool LTReactor::Stop() {
    m_bRunning = false;

    timeval val, now;
    val.tv_usec = 50 * 1000; //50ms后停止
    gettimeofday(&now, NULL);
    timeradd(&val, &now, &m_stoptime);
    return true;
}
// 注册读事件,
// @return: 0, success. else failed.
int LTReactor::RegisterReadEvent(FDEventHanderBase* pBase) {
    int type = 0;
    FDEventHanderBase* pHander = m_handlerSet.GetFDEventHandler(pBase->GetFD(),
     type);

    epoll_event event;
    event.data.fd = pBase->GetFD();
    event.events = type | EPOLLIN; // 注册读事件

    int iRet = epoll_ctl(m_epfd, NULL == pHander ? EPOLL_CTL_ADD
     : EPOLL_CTL_MOD, pBase->GetFD(), &event);
    if (iRet == -1)
    {
        std::cout << "epoll_ctl error: " << strerror(errno) << std::endl;
        return -1;
    }

    m_handlerSet.AddFDEventHandler(pBase, EPOLLIN);
    return 0;
}
// 注册写事件
// @return: 0, success. else failed.
int LTReactor::RegisterWriteEvent(FDEventHanderBase* pBase) {
    int type = 0;
    FDEventHanderBase* pHandler = m_handlerSet.GetFDEventHandler(
     pBase->GetFD(), type);

    epoll_event event;
    event.data.fd = pBase->GetFD();
    event.events = type | EPOLLOUT; //注册写事件

    int iRet = epoll_ctl(m_epfd, NULL == pHandler ? EPOLL_CTL_ADD
     : EPOLL_CTL_MOD, pBase->GetFD(), &event);
    if (iRet == -1)
    {
        std::cout << "epoll_ctl error: " << strerror(errno) << std::endl;
        return -1;
    }

    m_handlerSet.AddFDEventHandler(pBase, EPOLLOUT);
    return 0;
}
// 注销读事件
// @return: 0, success. else failed.
int LTReactor::UnRegisterReadEvent(FDEventHanderBase* pBase) {
    int type = 0;
    FDEventHanderBase* pHandler = m_handlerSet.GetFDEventHandler(
     pBase->GetFD(), type);
    epoll_event event;
    event.data.fd = pBase->GetFD();
    event.events = (type & ~EPOLLIN); // 取消读事件

    int iRet = epoll_ctl(m_epfd, NULL == pHandler ? EPOLL_CTL_DEL
     : EPOLL_CTL_MOD, pBase->GetFD(), &event);
    if (iRet == -1)
    {
        std::cout << "epoll_ctl error: " << strerror(errno) << std::endl;
        return -1;
    }

    m_handlerSet.DelFDEventHandler(pBase->GetFD(), EPOLLIN);
    return 0;
}

// 注销写事件
// @return: 0, success. else failed.
int LTReactor::UnRegisterWriteEvent(FDEventHanderBase* pBase) {
    int type = 0;
    FDEventHanderBase* pHandler = m_handlerSet.GetFDEventHandler(
     pBase->GetFD(), type);
    epoll_event event;
    event.data.fd = pBase->GetFD();
    event.events = (type & ~EPOLLOUT); // 取消写事件

    int iRet = epoll_ctl(m_epfd, NULL == pHandler ? EPOLL_CTL_DEL
     : EPOLL_CTL_MOD, pBase->GetFD(), &event);
    if (iRet == -1)
    {
        std::cout << "epoll_ctl error: " << strerror(errno) << std::endl;
        return -1;
    }

    m_handlerSet.DelFDEventHandler(pBase->GetFD(), EPOLLOUT);
    return 0;
}
// 注销读写事件
int LTReactor::UnRegisterAllEvent(FDEventHanderBase* pBase) {
    epoll_event event;
    event.data.fd = pBase->GetFD();
    event.events = EPOLLOUT | EPOLLIN; // 取消读写事件
    int iRet = epoll_ctl(m_epfd, EPOLL_CTL_DEL, pBase->GetFD(), &event);
    if (iRet == -1)
    {
        std::cout << "epoll_ctl error: " << strerror(errno) << std::endl;
        return -1;
    }

    m_handlerSet.DelFDEventHandler(pBase->GetFD(), EPOLLOUT | EPOLLIN);
    return 0;
}
// 注册定时器
int LTReactor::RegisterTimer(TimerEventHanderBase* pBase) {
    m_handlerSet.AddTimerEventHandler(pBase);
    return 0;
}
// 注销定时器
int LTReactor::UnRegisterTimer(TimerEventHanderBase* pBase) {
    m_handlerSet.DelTimerEventHandler(pBase);
    return 0;
}
// 扫描，看是否有定时器的时间到了
void LTReactor::ScanTimer() {
    m_handlerSet.ScanTimer();
}

/* **********************************
 *             处理FD类的基类
 * 当该fd被触发时，调用处理函数来进行处理
 * ***********************************/
// 注册读事件
int FDEventHanderBase::RegisterReadEvent() {
    return m_pReactor->RegisterReadEvent(this);
}
// 注册写事件
int FDEventHanderBase::RegisterWriteEvent() {
    return m_pReactor->RegisterWriteEvent(this);
}
// 注销读事件
int FDEventHanderBase::UnRegisterReadEvent() {
    return m_pReactor->UnRegisterReadEvent(this);
}
// 注销写事件
int FDEventHanderBase::UnRegisterWriteEvent() {
    return m_pReactor->UnRegisterWriteEvent(this);
}

int FDEventHanderBase::GetFD() const {
    return m_fd;
}
void FDEventHanderBase::SetFD(int fd) {
    m_fd = fd;
}
// 设置非阻塞，返回0，success.
int FDEventHanderBase::SetNonBlock(int fd, bool bNonBlock/* = true*/) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
    {
        return -1;
    }
    if (bNonBlock)
        flags |= O_NONBLOCK;
    else
        flags &= ~O_NONBLOCK;

    if (fcntl(fd, F_SETFL, flags) == -1)
    {
        return -1;
    }

    return 0;
}
/* **********************************
 *        处理timer类的基类
 *    当timer被触发时，调用该类的函数
 * **********************************/
// 注册定时器，@msec，多长时间响一次,单位:毫秒
// 由于epoll精度的限制，这个地方，精度只能达到 毫秒，但不是准确
int TimerEventHanderBase::RegisterTimer(unsigned int msec /*ms*/, bool restart)
{
    m_bRestart = restart;

    m_interval.tv_sec = msec / 1000;
    m_interval.tv_usec = msec % 1000;
    timeval now;
    gettimeofday(&now, NULL);
    timeradd(&now, &m_interval, &m_endtime);
    return m_pReactor->RegisterTimer(this);
}
int TimerEventHanderBase::UnRegisterTimer() {
    return m_pReactor->UnRegisterTimer(this);
}
int TimerEventHanderBase::RegisterTimerAgain() {
    if (timerisset(&m_endtime) && timerisset(&m_interval))
    {
        timeval now;
        gettimeofday(&now, NULL);
        timeradd(&now, &m_interval, &m_endtime);
        return m_pReactor->RegisterTimer(this);
    }
    return -1;
}
/* **********************************
 *             处理类的集合
 *         有个对应关系 -- fd -- 处理函数 -- read/write.
 * *********************************/
// 增加fd事件, @type : EPOLLIN, EPOLLOUT，
void EventHanderSet::AddFDEventHandler(FDEventHanderBase* pHandler, int type) {
    const int fd = pHandler->GetFD();
    std::map<int, FDHandler>::iterator it = m_fdmap.find(fd);
    if (it != m_fdmap.end())
    {
        it->second.type |= type;
        it->second.pHandler = pHandler;
    }
    else
    {
        FDHandler th;
        th.type = type;
        th.pHandler = pHandler;
        m_fdmap.insert(std::make_pair(fd, th));
    }
}
// @type : EPOLLIN, EPOLLOUT
void EventHanderSet::DelFDEventHandler(int fd, int type) {
    std::map<int, FDHandler>::iterator it = m_fdmap.find(fd);
    if (it != m_fdmap.end())
    {
        it->second.type &= ~type;//去掉该属性
        if (0 == it->second.type) //若所有的属性都去掉了，从map中删除该handler.
        {
            m_fdmap.erase(it);
        }
    }
}
// 根据fd，找到 FDEventHanderBase，及type.
FDEventHanderBase* EventHanderSet::GetFDEventHandler(int fd, int& type) {
    std::map<int, FDHandler>::iterator it = m_fdmap.find(fd);
    if (it != m_fdmap.end())
    {
        type = it->second.type;
        return it->second.pHandler;
    }

    return NULL;
}

void EventHanderSet::AddTimerEventHandler(TimerEventHanderBase* pHandler) {
    timeval endtime = pHandler->GetEndTime();
    std::pair<long int, long int> t(endtime.tv_sec, endtime.tv_usec);
    printf("addTimer: [%u:%u] \n", (unsigned int) t.first,
     (unsigned int) t.second);
    m_timerMinHeap.Insert(t);
    m_timerMultiMap.insert(std::make_pair(t, pHandler));
}
void EventHanderSet::DelTimerEventHandler(TimerEventHanderBase* pHandler) {
    timeval endtime = pHandler->GetEndTime();
    std::pair<long int, long int> t(endtime.tv_sec, endtime.tv_usec);
    std::multimap<std::pair<long int, long int>, TimerEventHanderBase*>::iterator
     beg = m_timerMultiMap.lower_bound(t);
    std::multimap<std::pair<long int, long int>, TimerEventHanderBase*>::iterator
     end = m_timerMultiMap.upper_bound(t);
    for (; beg != end; ++beg)
    {
        if (beg->second == pHandler)
        {
            m_timerMultiMap.erase(beg);
            break;
        }
    }
}
void EventHanderSet::ScanTimer() {

    while (true)
    {
        //        printf("m_timerMinHeap: m_max_size[%d] m_cur_size[%d] \n",
        //         m_timerMinHeap.GetMaxSize(), m_timerMinHeap.GetCurSize());
        if (!m_timerMinHeap.IsEmpty())
        {
            std::pair<long int, long int> minTime = m_timerMinHeap.GetMin(); //获取最小时间
            timeval minTimeVal;
            minTimeVal.tv_sec = minTime.first;
            minTimeVal.tv_usec = minTime.second;

            timeval now =
            { 0, 0 };
            gettimeofday(&now, NULL);
            if (timercmp(&now, &minTimeVal, >=)) //当前时间>=触发时间
            {
                m_timerMinHeap.RemoveMin(); //去掉最小堆里面的时间
                std::multimap<std::pair<long int, long int>,
                 TimerEventHanderBase*>::iterator beg =
                 m_timerMultiMap.lower_bound(minTime);

                for (; beg != m_timerMultiMap.upper_bound(minTime);)
                {
                    TimerEventHanderBase* pHandler = beg->second;
                    pHandler->OnTimeOut();
                    if (pHandler->IsRestart()) //需要重启
                    {
                        AddTimerEventHandler(pHandler);
                    }
                    m_timerMultiMap.erase(beg++);//去掉multimap中的句柄
                }
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
}
