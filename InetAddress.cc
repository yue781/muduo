#include <strings.h>
#include <string.h>

#include "InetAddress.h"

InetAddress::InetAddress(uint16_t port, std::string ip)
{
    bzero(&addr_, sizeof addr_);                   // 清空结构体
    addr_.sin_family = AF_INET;                    // 设置地址族为 IPv4
    addr_.sin_port = htons(port);                  // 端口号转为网络字节序
    addr_.sin_addr.s_addr = inet_addr(ip.c_str()); // IP地址转为网络字节序
}

std::string InetAddress::toIp() const
{
    char buf[64] = {0};
    ::inet_ntop(AF_INET, &addr_.sin_addr, buf, sizeof buf); // 将网络字节序的二进制IP转换为点分十进制字符串
    return buf; // 返回点分十进制IP（如 "127.0.0.1"）
}

std::string InetAddress::toIpPort() const // 生成 IP:Port 格式的字符串
{
    char buf[64] = {0};
    ::inet_ntop(AF_INET, &addr_.sin_addr, buf, sizeof buf); // 先转换IP
    size_t end = strlen(buf); // 获取IP字符串的末尾位置
    uint16_t port = ntohs(addr_.sin_port); // 端口号转为主机字节序
    sprintf(buf + end, ":%u", port); // 追加端口号（如 ":8080"）
    return buf;
}

uint16_t InetAddress::toPort() const
{
    return ntohs(addr_.sin_port); // 返回主机字节序的端口号
}

// #include <iostream>
// int main()
// {
//     InetAddress addr(8080);
//     std::cout << addr.toIpPort() << std::endl;
//     return 0;
// }