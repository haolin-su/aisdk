# 搜索当前目录下的所有.cpp文件，已及递归目录下的所有.cpp文件
file(GLOB_RECURSE CORE_SRC_FILES "*.cc")

add_library(aisdk_core SHARED ${CORE_SRC_FILES})

target_link_libraries(aisdk_core PUBLIC aisdk_common aisdk_registry)

install(TARGETS aisdk_core DESTINATION lib)
