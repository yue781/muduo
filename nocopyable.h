#pragma once

/*
nocopyable被继承以后的派生类对象
可以正常构造和析构
但是无法进行拷贝构造和赋值操作
*/
class nocopyable
{
public:
    nocopyable(const nocopyable &) = delete;
    void operator=(const nocopyable &) = delete;

protected:
    nocopyable() = default;
    ~nocopyable() = default;
};