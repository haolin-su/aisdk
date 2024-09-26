# 搜索当前目录下的所有.cpp文件，已及递归目录下的所有.cpp文件
file(GLOB_RECURSE API_SRC_FILES "*.cc")

# 编译为动态库
add_library(aisdk_api SHARED ${API_SRC_FILES})
# 链接aisdk_common aisdk_registry aisdk_core
target_link_libraries(aisdk_api PUBLIC aisdk_core aisdk_registry aisdk_common)

# 安装库文件
install(TARGETS aisdk_api DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
