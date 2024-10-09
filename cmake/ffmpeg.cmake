# # 配置ffmpeg

# set(FFMPEG_VERSION "4.4.5" CACHE STRING "ffmpeg version")

# # 设置ffmpeg源码路径
# set(FFMPEG_SRC_PATH ${CMAKE_SOURCE_DIR}/third_party/ffmpeg-${FFMPEG_VERSION})
# set(X264_SRC_PATH ${CMAKE_SOURCE_DIR}/third_party/x264)

# # 设置x264源码路径
# set(X264_SRC_PATH ${CMAKE_SOURCE_DIR}/third_party/x264)
# set(X264_INSTALL_PATH ${CMAKE_SOURCE_DIR}/third_party/ffmpeg-install-${BUILD_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR_ARCH})
# if (NOT EXISTS ${X264_SRC_PATH})
#     # 下载x264
#     file(DOWNLOAD https://code.videolan.org/videolan/x264/-/archive/master/x264-master.tar.gz WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/third_party/x264.tar.gz SHOW_PROGRESS)
#     execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${CMAKE_SOURCE_DIR}/third_party/x264.tar.gz WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/third_party)

# endif()

# # 创建ffmpeg编译目录
# set(FFMPEG_BUILD_PATH ${CMAKE_BINARY_DIR}/ffmpeg)
# file(MAKE_DIRECTORY ${FFMPEG_BUILD_PATH})

# # 创建ffmpeg安装目录
# set(FFMPEG_INSTALL_PATH ${CMAKE_SOURCE_DIR}/third_party/ffmpeg-install-${BUILD_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR_ARCH})
# file(MAKE_DIRECTORY ${FFMPEG_INSTALL_PATH})

# # 判断ffmpeg是否存在，不存在则下载源码https://github.com/FFmpeg/FFmpeg/archive/refs/tags/n4.4.5.tar.gz
# # https://github.com/FFmpeg/FFmpeg/archive/refs/tags/n${FFMPEG_VERSION}.tar.gz
# if(NOT EXISTS ${FFMPEG_SRC_PATH})
#     message(STATUS "Downloading ffmpeg ${FFMPEG_VERSION}...")
#     file(DOWNLOAD https://github.com/FFmpeg/FFmpeg/archive/refs/tags/n${FFMPEG_VERSION}.tar.gz ${CMAKE_SOURCE_DIR}/third_party/ffmpeg-${FFMPEG_VERSION}.tar.gz SHOW_PROGRESS)
#     # 如果下载失败，则报错
#     if (NOT EXISTS ${CMAKE_SOURCE_DIR}/third_party/ffmpeg-${FFMPEG_VERSION}.tar.gz)
#         message(FATAL_ERROR "Failed to download ffmpeg ${FFMPEG_VERSION}")
#     endif()
#     execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${CMAKE_SOURCE_DIR}/third_party/ffmpeg-${FFMPEG_VERSION}.tar.gz WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/third_party)
#     execute_process(COMMAND ${CMAKE_COMMAND} -E rename ${CMAKE_SOURCE_DIR}/third_party/ffmpeg-${FFMPEG_VERSION} ${FFMPEG_SRC_PATH})
#     file(REMOVE ${CMAKE_SOURCE_DIR}/third_party/ffmpeg-${FFMPEG_VERSION}.tar.gz)
# endif()

# if (NOT EXISTS ${FFMPEG_INSTALL_PATH}/lib/libffmpeg_core.so)
#     message(STATUS "Building ffmpeg ${FFMPEG_VERSION}...")  
#     # 如果是交叉编译:
#     if (CMAKE_SYSTEM_PROCESSOR_ARCH STREQUAL "aarch64")
#         if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
#             set(CROSS_PREFIX "aarch64-linux-gnu-")
#         elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
#             set(CROSS_PREFIX "aarch64-linux-gnu-")
#         else()
#             message(FATAL_ERROR "Unsupported compiler: ${CMAKE_CXX_COMPILER_ID}")
#         endif()

#         if (BUILD_SYSTEM_NAME STREQUAL "Android")
#             # 配置和编译x264
#             if (NOT EXISTS ${X264_INSTALL_PATH})
#                 execute_process(COMMAND ${FFMPEG_SRC_PATH}/configure 
#                                                             --cross-prefix=${CROSS_PREFIX}
#                                                             --target-os=android 
#                                                             --arch=aarch64
#                                                             --enable-cross-compile 
#                                                             --enable-static 
#                                                             --disable-opencl 
#                                                             --disable-cli 
#             endif()

#             # 配置ffmpeg
#             execute_process(COMMAND ${FFMPEG_SRC_PATH}/configure --prefix=${FFMPEG_INSTALL_PATH} 
#                                                         --cross-prefix=${CROSS_PREFIX}
#                                                         --target-os=android 
#                                                         --arch=aarch64
#                                                         --enable-cross-compile 
#                                                         --disable-shared 
#                                                         --enable-static 
#                                                         --enable-gpl 
#                                                         --enable-libx264
#                                                         --enable-nonfree 
#                                                         --enable-small 
#                                                         --enable-neon 
#                                                         --enable-hwaccels 
#                                                         --enable-avdevice 
#                                                         --enable-postproc 
#                                                         --enable-shared 
#                                                         --enable-jni 
#                                                         --enable-mediacodec
#                                                         --enable-decoder=h264_mediacodec
#                                                         --enable-encoder=h264_mediacodec
#                                                         --enable-muxer=mp4
#                                                         --enable-demuxer=mp4
#                                                         --enable-protocol=file
#                                                         --enable-protocol=http
#                                                         --enable-protocol=https
#                                                         --enable-protocol=tcp
#                                                         --enable-protocol=udp
#                                                         --enable-protocol=rtp
#                                                         --enable-protocol=rtsp
#                                                         --enable-protocol=srtp)

#                                                         ./configure \
#     # --prefix=$PREFIX \
#     # --disable-static \
#     # --disable-doc \
#     # --disable-ffmpeg \
#     # --disable-ffplay \
#     # --disable-ffprobe \
#     # --disable-doc \
#     # --disable-symver \
#     # --enable-nonfree \
#     # --enable-gpl \
#     # --enable-small \
#     # --enable-neon \
#     # --enable-hwaccels \
#     # --enable-avdevice \
#     # --enable-postproc \
#     # --enable-shared \
#     # --enable-jni \
#     # --enable-mediacodec \
#     # --enable-decoder=h264_mediacodec \
#     # --cross-prefix=$CROSS_PREFIX \
#     # --target-os=android \
#     # --arch=$ARCH \
#     # --cpu=$CPU \
#     # --nm=$NM \
#     # --strip=$STRIP \
#     # --cc=$CC \
#     # --cxx=$CXX \
#     # --enable-cross-compile \
#     # --sysroot=$SYSROOT \
#     # --extra-cflags="-Os -fpic $OPTIMIZE_CFLAGS" \
#     # --extra-ldflags="$ADDI_LDFLAGS" \
#     # $ADDITIONAL_CONFIGURE_FLAG

# #     ARCH=aarch64-linux-android-
# # VERSION=arm64
# # CPU=armv8-a
# # CROSS_PREFIX=$TOOLCHAIN/bin/${ARCH}
# # CC=$TOOLCHAIN/bin/aarch64-linux-android$API-clang
# # CXX=$TOOLCHAIN/bin/aarch64-linux-android$API-clang++
# # NM=$TOOLCHAIN/bin/${ARCH}nm
# # STRIP=$TOOLCHAIN/bin/${ARCH}strip
# # PREFIX=$(pwd)/android/$CPU
# # OPTIMIZE_CFLAGS="-march=$CPU"
# # build_android

                                                        
#         else()
#             # 配置和编译x264
#             if (NOT EXISTS ${X264_INSTALL_PATH})
#                 execute_process(COMMAND ${FFMPEG_SRC_PATH}/configure 
#                                                             --cross-prefix=${CROSS_PREFIX}
#                                                             --target-os=linux 
#                                                             --arch=aarch64
#                                                             --enable-cross-compile 
#                                                             --enable-static 
#                                                             --disable-opencl 
#                                                             --disable-cli 
#                                                             --enable-pic 
#                                                             --disable-asm 
#                                                             --prefix=${X264_INSTALL_PATH})
#                 execute_process(COMMAND ${CMAKE_COMMAND} --build ${FFMPEG_BUILD_PATH} --config Release --target install)
#             endif()

#             # 配置ffmpeg
#             execute_process(COMMAND ${FFMPEG_SRC_PATH}/configure --prefix=${FFMPEG_INSTALL_PATH} 
#                                                         --cross-prefix=${CROSS_PREFIX}
#                                                         --target-os=linux 
#                                                         --arch=aarch64
#                                                         --enable-cross-compile 
#                                                         --disable-shared 
#                                                         --enable-static 
#                                                         --enable-gpl 
#                                                         --enable-libx264 
#                                                         --enable-nonfree )   

#         endif()
                                                    
#     elseif (CMAKE_SYSTEM_PROCESSOR_ARCH STREQUAL "x86_64")
#         message(STATUS "Building ffmpeg ${FFMPEG_VERSION} for x86_64...")

#         # 配置和编译x264    
#         if (NOT EXISTS ${X264_INSTALL_PATH})
#             execute_process(COMMAND ${FFMPEG_SRC_PATH}/configure 
#                                                         --enable-static 
#                                                         --disable-opencl 
#                                                         --disable-cli 
#                                                         --enable-pic 
#                                                         --disable-asm 
#                                                         --prefix=${X264_INSTALL_PATH})
#             execute_process(COMMAND ${CMAKE_COMMAND} --build ${FFMPEG_BUILD_PATH} --config Release --target install)
#         endif()

#         # 配置ffmpeg
#         execute_process(COMMAND ${FFMPEG_SRC_PATH}/configure --prefix=${FFMPEG_INSTALL_PATH} 
#                                                     --enable-shared 
#                                                     --enable-static 
#                                                     --enable-gpl 
#                                                     --enable-libx264
#                                                     --enable-nonfree 
#                                                     --disable-x86asm
#                                                     --enable-ffmpeg
#                                                     --extra-cflags=-I${X264_INSTALL_PATH}/include
#                                                     --extra-ldflags=-L${X264_INSTALL_PATH}/lib)   
#     else()
#         message(FATAL_ERROR "Unsupported architecture: ${CMAKE_SYSTEM_PROCESSOR_ARCH}")
#     endif()
                                                
#     execute_process(COMMAND ${CMAKE_COMMAND} --build ${FFMPEG_BUILD_PATH} --config Release --target install)

# endif()

# # 配置ffmpeg
# include_directories(${FFMPEG_INSTALL_PATH}/include/ffmpeg)
# link_directories(${FFMPEG_INSTALL_PATH}/lib)
# set(FFMPEG_INCLUDE_DIRS ${FFMPEG_INSTALL_PATH}/include/ffmpeg)
# set(FFMPEG_LIBRARIES ${FFMPEG_INSTALL_PATH}/lib/libffmpeg_core.so)

# # 打印ffmpeg信息
# message(STATUS "ffmpeg version: ${FFMPEG_VERSION}")
# message(STATUS "ffmpeg include dirs: ${FFMPEG_INCLUDE_DIRS}")
# message(STATUS "ffmpeg libraries: ${FFMPEG_LIBRARIES}")

# install(DIRECTORY ${FFMPEG_INSTALL_PATH}/include/ffmpeg DESTINATION third_party/ffmpeg/include/ffmpeg)

