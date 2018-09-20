# 一些问题

### Q1: 在 macOS 上编译 UNIX 环境高级编程的实例代码时，发现 'apue.h' file not found

1. 去 官网 下载对应版本的源代码 

2. 解压之后在 apue.h 文件中 #endif 之前添加如下语句 #include "error.c"，并将 include 文件夹下的 apue.h 和 lib 文件夹下的 error.c 复制到 /usr/include 目录下 

3. 如果 /usr/include 目录不存在，或者出现 Operation not permitted 权限不足情况，请移步解决macOS /usr/include 无法写入或者不存在的问题 

4. 以上完成之后，即可编译并运行实例代码
