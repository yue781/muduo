#include <stdlib.h>

#include "Poller.h"
#include "EPollPoller.h"

// 通过公共文件解除了Poller和EpollPoller的强耦合，避免抽象基类依赖具体派生类
Poller *Poller::newDefaultPoller(EventLoop *loop)
{
    if (::getenv("MUDUO_USE_POLL")) // :: 表示调用全局命名空间的C标准库函数getenv
    {
        return nullptr; // 生成poll的实例
    }
    else
    {
        return new EPollPoller(loop); // 生成epoll的实例
    }
}