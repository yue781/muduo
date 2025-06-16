#include <iostream>

#include "Logger.h"
#include "Timestamp.h"

/*
获取日志唯一的实例对象（单例模式）

    唯一性：通过局部静态变量 static Logger logger 保证只构造一次。

    全局访问：通过静态成员函数 instance() 提供访问入口。

    禁止拷贝：Logger 继承自 nocopyable（或手动删除拷贝构造/赋值），防止意外复制。
*/
Logger &Logger::instance()
{
    static Logger logger;
    return logger;
}
// 设置日志级别
void Logger::setLogLevel(int level)
{
    logLevel_ = level;
}
// 打印日志
void Logger::log(std::string msg)
{
    switch (logLevel_)
    {
    case INFO:
        std::cout<<"[INFO]";
        break;
    case ERROR:
        std::cout<<"[ERROR]";
        break;
    case FATAL:
        std::cout<<"[FATAL]";
        break;
    case DEBUG:
        std::cout<<"[DEBUG]";
        break;  
    default:
        break;
    }

    // 打印时间和msg
    std::cout << Timestamp::now().toString() << " : " << msg << std::endl;
}