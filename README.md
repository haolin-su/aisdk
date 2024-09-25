# AISDK

## 介绍

AISDK是FIBO区块链项目的SDK，包含了FIBO区块链的底层API、应用层API、算法注册、编译工具链等。

## 目录结构

代码分层：
APP层
API层
Core层
Registry层

## 功能介绍
- 算法的端测推理


## 快速开始



## 代码结构

APP层：
Registry层：
包含算法注册和芯片SDK注册的相关代码。
算法注册：algorithm_registry.h/cpp

编译工具链：
toolchains目录下包含不同芯片SDK的编译工具链。


代码编译参数：
-DCMAKE_SYSTEM_PROCESSOR：编译的目标芯片架构，比如aarch64/armv8/x86_64等。
-DCMAKE_SYSTEM_NAME：编译的目标系统，比如oe/ubuntu/android/Windows等。
-DCMAKE_SYSTEM： 编译器类型，比如linux/windows。
-DCMAKE_SYSTEM_PROCESSOR_DSP：是否DSP的架构。
-DCMAKE_DSP_VERSION： 编译DSP的版本，比如66/68/69/73/75等。


交叉编译链配置文件存放在toolchain目录下，文件名为toolchain_xxx.cmake。

编译命令：
mkdir build && cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain_xxx.cmake ..
make -j8

#todo 
#ubuntu20 板子需要升级glibc 版本, 目前docker 镜像是ubuntu22 用的交叉编译工具链是 gcc-9-aarch64-linux-gnu=9.4.0-5ubuntu1cross1 , 其libc版本是2.35 ,ubuntu22 apt 能装gcc 有:
```
apt-cache madison  gcc-9-aarch64-linux-gnu
gcc-9-aarch64-linux-gnu | 9.5.0-1ubuntu1~22.04cross1 | http://mirrors.ustc.edu.cn/ubuntu jammy-updates/universe amd64 Packages
gcc-9-aarch64-linux-gnu | 9.5.0-1ubuntu1~22.04cross1 | http://security.ubuntu.com/ubuntu jammy-security/universe amd64 Packages
gcc-9-aarch64-linux-gnu | 9.4.0-5ubuntu1cross1 | http://mirrors.ustc.edu.cn/ubuntu jammy/universe amd64 Packages
gcc-9-aarch64-linux-gnu | 9.3.0-10ubuntu1cross2 | http://cz.archive.ubuntu.com/ubuntu focal/main amd64 Packages
``
#ubuntu20 板子默认是2.31 