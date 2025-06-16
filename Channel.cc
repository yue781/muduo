#include <sys/epoll.h>

#include "Channel.h"
#include "EventLoop.h"
#include "Logger.h"

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = EPOLLIN | EPOLLPRI;
const int Channel::kWriteEvent = EPOLLOUT;

Channel::Channel(EventLoop *loop, int fd)
    : loop_(loop), fd_(fd), events_(0), revents_(0), index_(-1), tied_(false)
{
}

Channel::~Channel()
{
}

// channel的tie方法调用时机：一个TcpConnection新连接创建的时候
void Channel::tie(const std::shared_ptr<void> &obj)
{
    tie_ = obj;
    tied_ = true;
}

/*
当改变channel表示的fd的events事件后，update负责在poller里面更改fd相应的事件epoll_ctl
EventLoop => ChannelList + Poller
*/
void Channel::update()
{
    // 通过channel所属的EventLoop，调用poller的相应方法，注册fd的events事件
    loop_->updateChannel(this);
}

// 在channel所属的EventLoop中，把当前的channel删掉
void Channel::remove()
{
    loop_->removeChannel(this);
}

// fd得到poller通知以后，处理事件
void Channel::handleEvent(Timestamp receiveTime)
{
    if (tied_)  // 当 Channel 绑定了对象
    {
        std::shared_ptr<void> guard = tie_.lock();  // 尝试提升为 shared_ptr
        if (guard)  // 对象仍存活
        {
            handleEventWithGuard(receiveTime);
        }
        // 若 guard 为 null，说明对象已销毁，忽略事件
    }
    else    // 当 Channel 不需要绑定对象（如单纯的 fd 监听）
    {
        handleEventWithGuard(receiveTime);
    }
}

// 根据poller通知的channel发生的具体事件，由channel负责调用具体的回调操作
void Channel::handleEventWithGuard(Timestamp receiveTime)
{
    LOG_INFO("channel handleEvent revents:%d\n", revents_);

    // 对端关闭连接或套接字不可用，并排除正常读关闭
    if ((revents_ & EPOLLHUP) && !(revents_ & EPOLLIN))
    {
        if (closeCallback_) // 在调用回调函数之前进行有效性检查可以避免空指针崩溃‌
        {
            closeCallback_();
        }
    }

    // fd发生错误
    if (revents_ & EPOLLERR)
    {
        if (errorCallback_)
        {
            errorCallback_();
        }
    }

    // 普通数据可读 或 紧急数据可读
    if (revents_ & (EPOLLIN | EPOLLPRI))
    {
        if (readCallback_)
        {
            readCallback_(receiveTime);
        }
    }

    // fd可写
    if (revents_ & EPOLLOUT)
    {
        if (writeCallback_)
        {
            writeCallback_();
        }
    }
}