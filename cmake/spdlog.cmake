set(SPDLOG_VERSION "1.14.1" CACHE STRING "spdlog version")

# 设置spdlog源码路径
set(SPDLOG_SRC_PATH ${CMAKE_SOURCE_DIR}/third_party/spdlog)

# 判断spdlog是否存在，不存在则下载源码
# https://github.com/gabime/spdlog/archive/refs/tags/v1.14.1.tar.gz
if(NOT EXISTS ${SPDLOG_SRC_PATH})
    message(STATUS "Downloading spdlog ${SPDLOG_VERSION}...")
    file(DOWNLOAD https://github.com/gabime/spdlog/archive/refs/tags/v${SPDLOG_VERSION}.tar.gz ${CMAKE_SOURCE_DIR}/third_party/spdlog-${SPDLOG_VERSION}.tar.gz SHOW_PROGRESS)
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${CMAKE_SOURCE_DIR}/third_party/spdlog-${SPDLOG_VERSION}.tar.gz WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/third_party)
    execute_process(COMMAND ${CMAKE_COMMAND} -E rename ${CMAKE_SOURCE_DIR}/third_party/spdlog-${SPDLOG_VERSION} ${SPDLOG_SRC_PATH})
    file(REMOVE ${CMAKE_SOURCE_DIR}/third_party/spdlog-${SPDLOG_VERSION}.tar.gz)
endif()


# 创建spdlog编译目录
set(SPDLOG_BUILD_PATH ${CMAKE_BINARY_DIR}/spdlog)
file(MAKE_DIRECTORY ${SPDLOG_BUILD_PATH})

# 创建spdlog安装目录
set(SPDLOG_INSTALL_PATH ${CMAKE_SOURCE_DIR}/third_party/spdlog-install-${BUILD_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR_ARCH})
file(MAKE_DIRECTORY ${SPDLOG_INSTALL_PATH})

if (NOT EXISTS ${SPDLOG_INSTALL_PATH}/lib/libspdlog.so AND NOT EXISTS ${SPDLOG_INSTALL_PATH}/lib/libspdlog.a)
    # 编译spdlog
    if (BUILD_SYSTEM_NAME STREQUAL "android")
        message(STATUS "ANDROID_NDK: ${ANDROID_NDK}")
        message(STATUS "ANDROID_ABI: ${ANDROID_ABI}")
        message(STATUS "ANDROID_NATIVE_API_LEVEL: ${ANDROID_NATIVE_API_LEVEL}")
        set(CMAKE_TOOLCHAIN_PATH ${ANDROID_NDK}/build/cmake/android.toolchain.cmake)
        execute_process(COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_PREFIX=${SPDLOG_INSTALL_PATH} 
                                                    -DCMAKE_BUILD_TYPE=Release 
                                                    -DSPDLOG_BUILD_EXAMPLE=OFF 
                                                    -DSPDLOG_BUILD_TESTS=OFF 
                                                    -DSPDLOG_BUILD_BENCH=OFF 
                                                    -DSPDLOG_INSTALL=ON 
                                                    -DSPDLOG_WCHAR_SUPPORT=ON 
                                                    -DSPDLOG_BUILD_SHARED=ON
                                                    -DSPDLOG_BUILD_STATIC=ON 
                                                    -DANDROID_NDK=$NDK_PATH  
                                                    -DCMAKE_CXX_FLAGS=-std=c++11 
                                                    -ANDROID_ABI=arm64-v8a 
                                                    -DANDROID_STL=c++_static 
                                                    -DANDROID_NATIVE_API_LEVEL=${ANDROID_NATIVE_API_LEVEL}
                                                    -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} 
                                                    -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} 
                                                    -G ${CMAKE_GENERATOR} ${SPDLOG_SRC_PATH} WORKING_DIRECTORY ${SPDLOG_BUILD_PATH})
    else()
        execute_process(COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_PREFIX=${SPDLOG_INSTALL_PATH} 
                                                    -DCMAKE_BUILD_TYPE=Release 
                                                    -DSPDLOG_BUILD_EXAMPLE=OFF 
                                                    -DSPDLOG_BUILD_TESTS=OFF 
                                                    -DSPDLOG_BUILD_BENCH=OFF 
                                                    -DSPDLOG_INSTALL=ON 
                                                    -DSPDLOG_WCHAR_SUPPORT=ON 
                                                    -DSPDLOG_BUILD_SHARED=ON
                                                    -DSPDLOG_BUILD_STATIC=ON 
                                                    -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} 
                                                    -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} 
                                                    -G ${CMAKE_GENERATOR} ${SPDLOG_SRC_PATH} WORKING_DIRECTORY ${SPDLOG_BUILD_PATH})

    endif()
    execute_process(COMMAND ${CMAKE_COMMAND} --build ${SPDLOG_BUILD_PATH} --config Release --target install)
endif()

# cpm.cmake
# 配置spdlog
# include_directories(${SPDLOG_INSTALL_PATH}/include/spdlog)
include_directories(${SPDLOG_INSTALL_PATH}/include/)
link_directories(${SPDLOG_INSTALL_PATH}/lib)
set(SPDLOG_INCLUDE_DIRS ${SPDLOG_INSTALL_PATH}/include/spdlog)
set(SPDLOG_LIBRARIES ${SPDLOG_INSTALL_PATH}/lib/libspdlog.so ${SPDLOG_INSTALL_PATH}/lib/libspdlog.a)

# 打印spdlog信息
message(STATUS "spdlog version: ${SPDLOG_VERSION}")
message(STATUS "spdlog include dirs: ${SPDLOG_INCLUDE_DIRS}")
message(STATUS "spdlog libraries: ${SPDLOG_LIBRARIES}")

install(DIRECTORY ${SPDLOG_INSTALL_PATH}/include/spdlog DESTINATION third_party/spdlog/include/spdlog)
install(DIRECTORY ${SPDLOG_INSTALL_PATH}/lib/ DESTINATION third_party/spdlog/lib)

