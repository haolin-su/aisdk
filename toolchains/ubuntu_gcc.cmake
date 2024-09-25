set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(TOOL_CHAIN_PATH /usr)
set(CMAKE_C_COMPILER ${TOOL_CHAIN_PATH}/bin/aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER ${TOOL_CHAIN_PATH}/bin/aarch64-linux-gnu-g++)
# 如果需要，可以设置链接器
# set(CMAKE_LINKER /path/to/arm-linux-gnueabi-gcc)

#todo 指定库文件的位置
# set(CMAKE_FIND_ROOT_PATH /path/to/sysroot)
# 不要猜测库文件的存在性
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)