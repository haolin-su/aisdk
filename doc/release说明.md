# release说明

## 版本号说明

- 主版本号：当有重大功能更新或者较大改动时，增加主版本号。如：1.0.0 -> 2.0.0
- 次版本号：当有新功能更新或者较小改动时，增加次版本号。如：1.0.0 -> 1.1.0
- 修订号：当有bug修复或者较小改动时，增加修订号。如：1.0.0 -> 1.0.1

## 发布说明

- 发布日期：发布日期为发布版本的日期。
- 版本说明：版本说明为本次发布版本的主要功能和改动说明。
- 下载地址：下载地址为发布版本的下载地址。
- 发布文档：发布文档为发布版本的详细说明文档。

## 发布SDK包内容
### 文件目录

```
aisdk/
├── bin/
│   ├── aisdk
├── cmake/
├── lib/
│   ├── libfibosdk.so
├── include/
│   ├── aisdk.hpp
├── doc/
│   ├── release说明.md
├── examples/
│   ├── fibosdk_example.cpp
├── docker/
│   ├── Dockerfile
├── scripts/
│   ├── build.sh
│   ├── install.sh
├── python/
├── java/
├── third_party/
│   ├── boost/
├── tools/
├── README.md
└── CHANGELOG.md
```


### 发布包内容

- bin/aisdk：编译好的fibosdk可执行文件。
- cmake/：CMake模块文件，比如包含编译链配置，编译fibosdk所需的依赖库。
- lib/libfibosdk.so：编译好的fibosdk动态库。
- include/aisdk.hpp：fibosdk的头文件。
- doc/release说明.md：发布说明文档。
- examples/fibosdk_example.cpp：fibosdk的示例代码。
- docker/Dockerfile：Dockerfile文件，用于构建docker镜像。
- scripts/build.sh：编译脚本，用于编译fibosdk。
- scripts/install.sh：安装脚本，用于安装fibosdk。
- python/：fibosdk的python接口。
- java/：fibosdk的java接口。
- third_party/boost/：依赖库boost。
- tools/：工具脚本，比如用于生成密钥对，签名等。