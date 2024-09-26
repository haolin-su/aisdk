# include_directories(${CMAKE_SOURCE_DIR}/third_party/re2/include)

# 搜索当前目录下的所有.cpp文件，已及递归目录下的所有.cpp文件
file(GLOB_RECURSE REGISTRY_SRC_FILES "*.cc")

add_library(aisdk_registry SHARED ${REGISTRY_SRC_FILES})

set(OPENCV_LIBRARIES "opencv_core;opencv_imgcodecs;opencv_imgproc;")
target_link_libraries(aisdk_registry PUBLIC aisdk_common ${OPENCV_LIBRARIES} ${REGISTRY_LIBRARIES})
install(TARGETS aisdk_registry DESTINATION lib)