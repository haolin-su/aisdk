vscode远程调试工具

# 前置准备：
安装虚拟机，安装ubuntu22系统  
\\192.168.1.33\sz_r&d\AI研究院\AI软件开发部\环境搭建\ubuntu-22.04.4-desktop-amd64.iso  
\\192.168.1.33\sz_r&d\AI研究院\AI软件开发部\环境搭建\VMware17虚拟机安装文件.zip  

安装docker，自行谷歌搜索；

已经配置好高通SDK环境的docker镜像如下：  
\\192.168.1.33\sz_r&d\AI研究院\AI软件开发部\高通工具与SDK\fibosdk_qualcommon_snpe.tar
```shell
sudo docker load -i fibosdk_qualcommon_snpe.tar

docker images
# REPOSITORY                             TAG       IMAGE ID       CREATED        SIZE
# fibosdk/qualcommon/snpe                v0.1      075bbe7e9e96   4 weeks ago    10.9GB

```

安装adb工具：
```shell
sudo apt-get udpate -y && sudo apt-get install adb
```


# 启动docker容器
```shell
# 创建docker容器建议使用--privileged参数，否则无法使用adb命令
docker run -it --privileged --name aisdk_snpe_qnn --network host -v /root/work/aisdk/:/aisdk fibosdk/qualcommon/snpe:v0.1 bash
# -v 自行决定映射目录，/root/work/aisdk/:/aisdk 映射到主机的/aisdk目录，方便后续操作
```

在容器内安装其他工具：
```shell
sudo apt-get update -y && sudo apt-get install -y git cmake build-essential ccache vim gdb-multiarch
```


# 安装vscode远程调试环境
配置vscode远程调试环境：
1. 安装Remote Development插件
2. 打开vscode，点击左下角的齿轮图标，选择settings，搜索Remote，点击Remote-SSH: Connect to Host...，输入虚拟机的IP地址，点击Connect，输入用户名和密码
3. 进入远程桌面后，不需要打开目录，直接ctrl+shift+p，输入>dev Containers:Attach to Running Container，选择aisdk_snpe_qnn，点击Attach
4. 进入远程的容器后，再打开对应的目录，这里打开aisdk和aitools两个窗口

# gdbserver和gdb-multiarch安装
板端使用USB线连接主机，主机的虚拟机界面右下角点击链接，同时在虚拟机系统需要adb kill-server，这样保证adb可以直通docker容器中；

安卓不自带gdbserver，需要自己下载并推送到板端：
wget https://raw.githubusercontent.com/topjohnwu/FrankeNDK/master/prebuilt/android-arm64/gdbserver/gdbserver -P /tmp

adb shell mkdir /data/local/tmp/bin
adb push /tmp/gdbserver /data/local/tmp/bin/gdbserver
adb shell chmod 755 /data/local/tmp/bin/gdbserver

# 正式运行
## 在vscode中配置调试环境  

在aisdk窗口工程中配置launch.json
```json
{
    // Use IntelliSense to learn about possible attributes.
    // Hover to view descriptions of existing attributes.
    // For more information, visit: https://go.microsoft.com/fwlink/?linkid=830387
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Python: Current File",
            "type": "python",
            "request": "launch",
            "program": "${file}",
            "console": "integratedTerminal",
            "args": ["--config", "/data/benchmark/config.json", "--work-dir", "/volume1/gddi-data/lgy/gddeploy/benchmark/"]
        },
        {
            "name": "(gdb) Launch",
            "type": "cppdbg",
            "request": "launch",
            
            // "program": "${command:cmake.launchTargetPath}",
            "program": "${workspaceFolder}/install/qualcommon_aarch64_android/test/test_qnn_runtime_multimodel",
            // "args": [],
            "stopAtEntry": false,
            "cwd": "${fileDirname}",
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb",
            "setupCommands": [
                {
                    "description": "Enable pretty-printing for gdb",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ],
            // "miDebuggerPath": "/usr/bin/gdb"
            "miDebuggerPath": "/usr/bin/gdb-multiarch",
            "miDebuggerServerAddress": ":12345", //
        }
    ]
}
```


在aisdk窗口工程中配置tasks.json
```json
{
	"version": "2.0.0",
	"tasks": [
		{
			"type": "cmake",
			"label": "CMake: configure",
			"command": "configure",
			"runOptions": {},
			"problemMatcher": [],
			"detail": "CMake template configure task",
			"group": "build"
		},
		{
			"type": "cmake",
			"label": "CMake: build",
			"command": "build",
			"targets": [
				"all"
			],
			"group": "build",
			"problemMatcher": [],
			"dependsOn": [
				"CMake: configure"
			],
			"detail": "CMake template build task"
		},
		{
			"type": "cmake",
			"label": "CMake: install",
			"command": "install",
			"problemMatcher": [],
			"dependsOn": [
				"CMake: build"
			],
			"detail": "CMake template install task",
			"group": "build"
		},
	]
}
```

在aisdk窗口工程中配置settings.json
```json
{
    "files.associations": {
        "*.ipp": "cpp",
        "functional": "cpp",
        "cctype": "cpp",
        "clocale": "cpp",
        "cmath": "cpp",
        "csignal": "cpp",
        "cstdarg": "cpp",
        "cstddef": "cpp",
        "cstdio": "cpp",
        "cstdlib": "cpp",
        "cstring": "cpp",
        "ctime": "cpp",
        "cwchar": "cpp",
        "cwctype": "cpp",
        "any": "cpp",
        "array": "cpp",
        "atomic": "cpp",
        "strstream": "cpp",
        "*.tcc": "cpp",
        "bitset": "cpp",
        "chrono": "cpp",
        "codecvt": "cpp",
        "complex": "cpp",
        "condition_variable": "cpp",
        "cstdint": "cpp",
        "deque": "cpp",
        "forward_list": "cpp",
        "list": "cpp",
        "unordered_map": "cpp",
        "unordered_set": "cpp",
        "vector": "cpp",
        "exception": "cpp",
        "algorithm": "cpp",
        "filesystem": "cpp",
        "iterator": "cpp",
        "map": "cpp",
        "memory": "cpp",
        "memory_resource": "cpp",
        "numeric": "cpp",
        "optional": "cpp",
        "random": "cpp",
        "ratio": "cpp",
        "set": "cpp",
        "string": "cpp",
        "string_view": "cpp",
        "system_error": "cpp",
        "tuple": "cpp",
        "type_traits": "cpp",
        "utility": "cpp",
        "fstream": "cpp",
        "future": "cpp",
        "initializer_list": "cpp",
        "iomanip": "cpp",
        "iosfwd": "cpp",
        "iostream": "cpp",
        "istream": "cpp",
        "limits": "cpp",
        "mutex": "cpp",
        "new": "cpp",
        "ostream": "cpp",
        "sstream": "cpp",
        "stdexcept": "cpp",
        "streambuf": "cpp",
        "thread": "cpp",
        "cfenv": "cpp",
        "cinttypes": "cpp",
        "typeindex": "cpp",
        "typeinfo": "cpp",
        "valarray": "cpp",
        "variant": "cpp",
        "source_location": "cpp",
        "queue": "cpp",
        "bit": "cpp",
        "__nullptr": "cpp",
        "__hash_table": "cpp",
        "__split_buffer": "cpp",
        "__tree": "cpp",
        "stack": "cpp",
        "compare": "cpp",
        "concepts": "cpp",
        "numbers": "cpp",
        "ios": "cpp",
        "__bit_reference": "cpp",
        "__config": "cpp",
        "__debug": "cpp",
        "__errc": "cpp",
        "__locale": "cpp",
        "__mutex_base": "cpp",
        "__node_handle": "cpp",
        "__threading_support": "cpp",
        "__verbose_abort": "cpp",
        "charconv": "cpp",
        "locale": "cpp",
        "__memory": "cpp",
        "regex": "cpp",
        "scoped_allocator": "cpp",
        "span": "cpp",
        "semaphore": "cpp",
        "stop_token": "cpp"
    },
    "python.pythonPath": "/bin/python3",
    "cmake.configureArgs": [
        "-G",
        "Ninja",
        "-DCMAKE_CXX_COMPILER_LAUNCHER=ccache",
        "-DBUILD_TEST=ON",

        "-DCMAKE_DSP_VERSION=68",

        // "-DCMAKE_CXX_VERSION=11.2",
        // "-DCMAKE_SYSTEM_PROCESSOR_ARCH=x86_64", //x86/aarch64
        // "-DCMAKE_SYSTEM_NAME=linux", //linux/android/oe/ubuntu
        // "-DCMAKE_INSTALL_PREFIX=/aisdk/install/qualcommon_x86_64_linux"

        // 使用安卓的交叉编译,指定toolchain/android_ndk.cmake
        "-DCMAKE_SYSTEM_PROCESSOR_ARCH=aarch64",
        "-DCMAKE_SYSTEM_NAME=android",
        "-DANDROID_ABI=arm64-v8a",
        "-DANDROID_NATIVE_API_LEVEL=21",
        "-DCMAKE_TOOLCHAIN_FILE=/aisdk/toolchains/android_ndk.cmake",
        "-DCMAKE_INSTALL_PREFIX=/aisdk/install/qualcommon_aarch64_android"
    ],
    "cmake.buildToolArgs": [],
    "cmake.buildDirectory": "${workspaceFolder}/build",
    "cmake.cmakePath": "cmake",
    "cmake.sourceDirectory": "${workspaceFolder}",
    "fileheader.customMade": {
        "param": "",
    },
    "C_Cpp.errorSquiggles": "disabled",
    
}
```

在aitools窗口工程中配置launch.json
```json
{
    // Use IntelliSense to learn about possible attributes.
    // Hover to view descriptions of existing attributes.
    // For more information, visit: https://go.microsoft.com/fwlink/?linkid=830387
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Python Debugger: Current File",
            "type": "debugpy",
            "request": "launch",
            // "program": "${file}",
            "program": "./main.py",
            "args": [
                // main.py
                "--log-to-console",
                "--log-level", "DEBUG",
                // "--task_config", "./config/task/tasks_hello.json"
                "--task_config", "./config/task/tasks_only_infer_gdb.json"
            ],
            "console": "integratedTerminal"
        }
    ]
}
```

## 运行调试

以上工作全部准备好后，即可运行调试。

在aitools窗口中按F5即可将aisdk中的代码push到板端，并且最后开启gdbserver;

在aisdk窗口中按F5，即可运行程序，接下来就可以随意进入gdb调试模式。