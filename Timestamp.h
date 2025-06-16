#pragma once

#include <iostream>
#include <string>

class Timestamp
{
private:
    int64_t microSecondsSinceEpoch_;
public:
    Timestamp();
    explicit Timestamp(int64_t microSecondsSinceEpoch); // 使用explicit修饰构造函数时，它将禁止类对象之间的隐式转换，以及禁止隐式调用拷贝构造函数
    static Timestamp now();
    std::string toString() const;
};

