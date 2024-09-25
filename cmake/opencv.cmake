# 配置opencv

set(OPENCV_VERSION "4.8.0" CACHE STRING "opencv version")

# 设置opencv源码路径
set(OPENCV_SRC_PATH ${CMAKE_SOURCE_DIR}/third_party/opencv-${OPENCV_VERSION})

# 判断opencv是否存在，不存在则下载源码
# https://github.com/opencv/opencv/archive/refs/tags/4.8.0.tar.gz
if(NOT EXISTS ${OPENCV_SRC_PATH})
    message(STATUS "Downloading opencv ${OPENCV_VERSION}...")
    file(DOWNLOAD https://github.com/opencv/opencv/archive/refs/tags/4.8.0.tar.gz ${CMAKE_SOURCE_DIR}/third_party/opencv-${OPENCV_VERSION}.tar.gz SHOW_PROGRESS)
    # 如果下载失败，则报错
    if (NOT EXISTS ${CMAKE_SOURCE_DIR}/third_party/opencv-${OPENCV_VERSION}.tar.gz)
        message(FATAL_ERROR "Failed to download opencv ${OPENCV_VERSION}")
    endif()
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${CMAKE_SOURCE_DIR}/third_party/opencv-${OPENCV_VERSION}.tar.gz WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/third_party)
    execute_process(COMMAND ${CMAKE_COMMAND} -E rename ${CMAKE_SOURCE_DIR}/third_party/opencv-${OPENCV_VERSION} ${OPENCV_SRC_PATH})
    file(REMOVE ${CMAKE_SOURCE_DIR}/third_party/opencv-${OPENCV_VERSION}.tar.gz)
endif()

# 创建opencv编译目录
set(OPENCV_BUILD_PATH ${CMAKE_BINARY_DIR}/opencv)
file(MAKE_DIRECTORY ${OPENCV_BUILD_PATH})

# 创建opencv安装目录
set(OPENCV_INSTALL_PATH ${CMAKE_SOURCE_DIR}/third_party/opencv-${OPENCV_VERSION}-install-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR_ARCH})
file(MAKE_DIRECTORY ${OPENCV_INSTALL_PATH})

if (NOT EXISTS ${OPENCV_INSTALL_PATH}/lib/libopencv_core.so)
    # 如果是交叉编译,并且是android编译:
    if (CMAKE_SYSTEM_PROCESSOR_ARCH STREQUAL "aarch64")
        if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            set(CROSS_PREFIX "aarch64-linux-gnu-")
        elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
            set(CROSS_PREFIX "aarch64-linux-gnu-")
        else()
            message(FATAL_ERROR "Unsupported compiler: ${CMAKE_CXX_COMPILER_ID}")
        endif()

        if (CMAKE_SYSTEM_NAME STREQUAL "android")
            message(STATUS "ANDROID_NDK: ${ANDROID_NDK}")
            message(STATUS "ANDROID_ABI: ${ANDROID_ABI}")
            message(STATUS "ANDROID_NATIVE_API_LEVEL: ${ANDROID_NATIVE_API_LEVEL}")
            set(CMAKE_TOOLCHAIN_PATH ${ANDROID_NDK}/build/cmake/android.toolchain.cmake)
            execute_process(COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_PREFIX=${OPENCV_INSTALL_PATH} 
                                                    # -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
                                                    # -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
                                                    -DCMAKE_TOOLCHAIN_FILE=$CMAKE_TOOLCHAIN_PATH 
                                                    -DCMAKE_BUILD_TYPE=Release 
                                                    -DANDROID_NDK=$NDK_PATH  
                                                    -DCMAKE_CXX_FLAGS=-std=c++11 
                                                    -DANDROID_ABI=arm64-v8a 
                                                    -DANDROID_STL=c++_static 
                                                    -DENABLE_CXX11=ON 
                                                    -DBUILD_ANDROID_PROJECTS=OFF 
                                                    -DBUILD_ANDROID_EXAMPLES=OFF 
                                                    -DBUILD_JAVA=OFF  
                                                    -DBUILD_CUDA_STUBS=OFF 
                                                    -DBUILD_DOCS=OFF 
                                                    -DBUILD_EXAMPLES=OFF 
                                                    -DBUILD_JASPER=OFF 
                                                    -DBUILD_JPEG=OFF 
                                                    -DBUILD_OPENEXR=OFF 
                                                    -DBUILD_PACKAGE=OFF 
                                                    -DBUILD_PERF_TESTS=OFF 
                                                    -DBUILD_PNG=OFF 
                                                    -DBUILD_SHARED_LIBS=OFF 
                                                    -DBUILD_TBB=OFF  
                                                    -DBUILD_TESTS=OFF 
                                                    -DBUILD_TIFF=OFF  
                                                    -DBUILD_WITH_DEBUG_INFO=OFF  
                                                    -DBUILD_WITH_DYNAMIC_IPP=OFF  
                                                    -DBUILD_ZLIB=OFF  
                                                    -DBUILD_opencv_apps=OFF 
                                                    -DBUILD_opencv_calib3d=OFF 
                                                    -DBUILD_opencv_core=ON 
                                                    -DBUILD_opencv_features2d=OFF 
                                                    -DBUILD_opencv_flann=OFF 
                                                    -DBUILD_opencv_highgui=OFF 
                                                    -DBUILD_opencv_imgcodecs=OFF 
                                                    -DBUILD_opencv_imgproc=ON 
                                                    -DBUILD_opencv_ml=OFF 
                                                    -DBUILD_opencv_objdetect=OFF 
                                                    -DBUILD_opencv_photo=OFF 
                                                    -DBUILD_opencv_python2=OFF 
                                                    -DBUILD_opencv_python3=OFF 
                                                    -DBUILD_opencv_shape=OFF 
                                                    -DBUILD_opencv_stitching=OFF 
                                                    -DBUILD_opencv_superres=OFF 
                                                    -DBUILD_opencv_ts=OFF 
                                                    -DBUILD_opencv_video=ON 
                                                    -DBUILD_opencv_videoio=OFF 
                                                    -DBUILD_opencv_videostab=OFF 
                                                    -DBUILD_opencv_world=OFF 
                                                    -DWITH_IPP=OFF 
                                                    -DWITH_FFMPEG=OFF 
                                                    -DWITH_CUDA=OFF 
                                                    -DWITH_CUFFT=OFF 
                                                    -DWITH_EIGEN=OFF 
                                                    -DWITH_JASPER=OFF 
                                                    -DWITH_JPEG=OFF 
                                                    -DWITH_PNG=OFF 
                                                    -DWITH_PTHREADS_PF=OFF 
                                                    -DWITH_OPENEXR=OFF 
                                                    -DWITH_MATLAB=OFF 
                                                    -DWITH_TBB=OFF 
                                                    -DWITH_TIFF=OFF 
                                                    -DWITH_WEBP=OFF 
                                                    -DANDROID_NATIVE_API_LEVEL=${ANDROID_NATIVE_API_LEVEL}
                                                    ${OPENCV_SRC_PATH} 
                                                    -B ${OPENCV_BUILD_PATH})
        else()
            execute_process(COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_PREFIX=${OPENCV_INSTALL_PATH} 
                                                    -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
                                                    -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
                                                    # -DCMAKE_CXX_STANDARD=${CMAKE_CXX_VERSION}
                                                    -DCMAKE_BUILD_TYPE=Release 
                                                    -DOPENCV_BUILD_EXAMPLE=OFF 
                                                    -DOPENCV_BUILD_TESTS=OFF 
                                                    -DOPENCV_BUILD_BENCH=OFF 
                                                    -DOPENCV_INSTALL=ON 
                                                    -DOPENCV_WCHAR_SUPPORT=ON 
                                                    -DOPENCV_BUILD_opencv_core=ON 
                                                    -DOPENCV_BUILD_opencv_imgcodecs=ON 
                                                    -DOPENCV_BUILD_opencv_imgproc=OFF 
                                                    -DOPENCV_BUILD_opencv_highgui=OFF 
                                                    -DOPENCV_BUILD_opencv_videoio=OFF 
                                                    -DOPENCV_BUILD_opencv_calib3d=OFF 
                                                    -DOPENCV_BUILD_opencv_features2d=OFF 
                                                    -DOPENCV_BUILD_opencv_flann=OFF 
                                                    -DOPENCV_BUILD_opencv_ml=OFF 
                                                    -DOPENCV_BUILD_opencv_objdetect=OFF 
                                                    -DOPENCV_BUILD_opencv_photo=OFF 
                                                    -DOPENCV_BUILD_opencv_stitching=OFF 
                                                    -DOPENCV_BUILD_opencv_video=OFF 
                                                    -DOPENCV_BUILD_opencv_world=OFF 
                                                    -DOPENCV_BUILD_ML=OFF
                                                    -DOPENCV_BUILD_STITCHING=OFF
                                                    -DOPENCV_BUILD_CALIB3D=OFF
                                                    -DOPENCV_BUILD_DNN=OFF
                                                    -DOPENCV_BUILD_FEATURES2D=OFF
                                                    -DOPENCV_BUILD_FLANN=OFF
                                                    -DOPENCV_BUILD_HIGHGUI=OFF
                                                    -DOPENCV_BUILD_OBJDETECT=OFF
                                                    -DOPENCV_BUILD_EXAMPLES=OFF 
                                                    -DOPENCV_BUILD_TESTS=OFF 
                                                    -DOPENCV_BUILD_PERF_TESTS=OFF 
                                                    -DOPENCV_BUILD_DOCS=OFF 
                                                    -DINSTALL_C_EXAMPLES=OFF 
                                                    -DINSTALL_PYTHON_EXAMPLES=OFF 
                                                    -DWITH_1394=OFF 
                                                    -DWITH_CUDA=OFF 
                                                    -DWITH_CUFFT=OFF 
                                                    -DWITH_TBB=OFF 
                                                    -DWITH_VTK=OFF 
                                                    -DWITH_IPP=OFF 
                                                    -DWITH_OPENCL=OFF 
                                                    -DWITH_OPENCL_SVM=OFF 
                                                    -DWITH_OPENCLAMDFFT=OFF 
                                                    -DWITH_OPENCLAMDBLAS=OFF 
                                                    -DWITH_GTK=ON 
                                                    -DWITH_QT=OFF 
                                                    -DWITH_OPENGL=OFF 
                                                    -DWITH_JASPER=OFF 
                                                    -DWITH_PNG=ON 
                                                    -DWITH_JPEG=ON 
                                                    -DWITH_WEBP=OFF 
                                                    -DWITH_TIFF=ON 
                                                    -DWITH_PNG=ON 
                                                    -DWITH_PROTOBUF=OFF 
                                                    -DWITH_FFMPEG=OFF 
                                                    -DWITH_GSTREAMER=OFF 
                                                    -DWITH_GDAL=OFF 
                                                    -DWITH_XINE=OFF 
                                                    -DWITH_IMAGEIO=OFF 
                                                    -DWITH_IPP_IW=OFF 
                                                    -DWITH_IPP=OFF 
                                                    -DWITH_IPP_A=OFF 
                                                    -DWITH_IPP_IW=OFF 
                                                    -DWITH_IPP_IW=OFF 
                                                    -DWITH_IPP_W=OFF 
                                                    -DWITH_IPP_A=OFF 
                                                    -DWITH_IPP_IW=OFF 
                                                    ${OPENCV_SRC_PATH} 
                                                    -B ${OPENCV_BUILD_PATH})
        endif()
    elseif (CMAKE_SYSTEM_PROCESSOR_ARCH STREQUAL "x86_64")
        # 编译opencv，只需要配置imread功能，其他均不需要
        execute_process(COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_PREFIX=${OPENCV_INSTALL_PATH} 
                                                    -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
                                                    -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
                                                    -DCMAKE_CXX_STANDARD=${CMAKE_CXX_VERSION}
                                                    -DCMAKE_BUILD_TYPE=Release 
                                                    -DOPENCV_BUILD_EXAMPLE=OFF 
                                                    -DOPENCV_BUILD_TESTS=OFF 
                                                    -DOPENCV_BUILD_BENCH=OFF 
                                                    -DOPENCV_INSTALL=ON 
                                                    -DOPENCV_WCHAR_SUPPORT=ON 
                                                    -DOPENCV_BUILD_opencv_core=ON 
                                                    -DOPENCV_BUILD_opencv_imgcodecs=ON 
                                                    -DOPENCV_BUILD_opencv_imgproc=OFF 
                                                    -DOPENCV_BUILD_opencv_highgui=OFF 
                                                    -DOPENCV_BUILD_opencv_videoio=OFF 
                                                    -DOPENCV_BUILD_opencv_calib3d=OFF 
                                                    -DOPENCV_BUILD_opencv_features2d=OFF 
                                                    -DOPENCV_BUILD_opencv_flann=OFF 
                                                    -DOPENCV_BUILD_opencv_ml=OFF 
                                                    -DOPENCV_BUILD_opencv_objdetect=OFF 
                                                    -DOPENCV_BUILD_opencv_photo=OFF 
                                                    -DOPENCV_BUILD_opencv_stitching=OFF 
                                                    -DOPENCV_BUILD_opencv_video=OFF 
                                                    -DOPENCV_BUILD_opencv_world=OFF 
                                                    -DOPENCV_BUILD_ML=OFF
                                                    -DOPENCV_BUILD_STITCHING=OFF
                                                    -DOPENCV_BUILD_CALIB3D=OFF
                                                    -DOPENCV_BUILD_DNN=OFF
                                                    -DOPENCV_BUILD_FEATURES2D=OFF
                                                    -DOPENCV_BUILD_FLANN=OFF
                                                    -DOPENCV_BUILD_HIGHGUI=OFF
                                                    -DOPENCV_BUILD_OBJDETECT=OFF
                                                    -DOPENCV_BUILD_EXAMPLES=OFF 
                                                    -DOPENCV_BUILD_TESTS=OFF 
                                                    -DOPENCV_BUILD_PERF_TESTS=OFF 
                                                    -DOPENCV_BUILD_DOCS=OFF 
                                                    -DINSTALL_C_EXAMPLES=OFF 
                                                    -DINSTALL_PYTHON_EXAMPLES=OFF 
                                                    -DWITH_1394=OFF 
                                                    -DWITH_CUDA=OFF 
                                                    -DWITH_CUFFT=OFF 
                                                    -DWITH_TBB=OFF 
                                                    -DWITH_VTK=OFF 
                                                    -DWITH_IPP=OFF 
                                                    -DWITH_OPENCL=OFF 
                                                    -DWITH_OPENCL_SVM=OFF 
                                                    -DWITH_OPENCLAMDFFT=OFF 
                                                    -DWITH_OPENCLAMDBLAS=OFF 
                                                    -DWITH_GTK=ON 
                                                    -DWITH_QT=OFF 
                                                    -DWITH_OPENGL=OFF 
                                                    -DWITH_JASPER=OFF 
                                                    -DWITH_PNG=ON 
                                                    -DWITH_JPEG=ON 
                                                    -DWITH_WEBP=OFF 
                                                    -DWITH_TIFF=ON 
                                                    -DWITH_PNG=ON 
                                                    -DWITH_PROTOBUF=OFF 
                                                    -DWITH_FFMPEG=OFF 
                                                    -DWITH_GSTREAMER=OFF 
                                                    -DWITH_GDAL=OFF 
                                                    -DWITH_XINE=OFF 
                                                    -DWITH_IMAGEIO=OFF 
                                                    -DWITH_IPP_IW=OFF 
                                                    -DWITH_IPP=OFF 
                                                    -DWITH_IPP_A=OFF 
                                                    -DWITH_IPP_IW=OFF 
                                                    -DWITH_IPP_IW=OFF 
                                                    -DWITH_IPP_W=OFF 
                                                    -DWITH_IPP_A=OFF 
                                                    -DWITH_IPP_IW=OFF 
                                                    ${OPENCV_SRC_PATH} 
                                                    -B ${OPENCV_BUILD_PATH})
    endif()                                           
    execute_process(COMMAND ${CMAKE_COMMAND} --build ${OPENCV_BUILD_PATH} --config Release --target install -j8)

endif()

# 配置opencv
include_directories(${OPENCV_INSTALL_PATH}/include/opencv4)
link_directories(${OPENCV_INSTALL_PATH}/lib)
set(OPENCV_INCLUDE_DIRS ${OPENCV_INSTALL_PATH}/include/opencv4)
set(OPENCV_LIBRARIES ${OPENCV_INSTALL_PATH}/lib/libopencv_core.so ${OPENCV_INSTALL_PATH}/lib/libopencv_imgcodecs.so ${OPENCV_INSTALL_PATH}/lib/libopencv_imgproc.so ${OPENCV_INSTALL_PATH}/lib/libopencv_highgui.so ${OPENCV_INSTALL_PATH}/lib/libopencv_videoio.so ${OPENCV_INSTALL_PATH}/lib/libopencv_calib3d.so ${OPENCV_INSTALL_PATH}/lib/libopencv_features2d.so ${OPENCV_INSTALL_PATH}/lib/libopencv_flann.so ${OPENCV_INSTALL_PATH}/lib/libopencv_ml.so ${OPENCV_INSTALL_PATH}/lib/libopencv_objdetect.so ${OPENCV_INSTALL_PATH}/lib/libopencv_photo.so ${OPENCV_INSTALL_PATH}/lib/libopencv_stitching.so ${OPENCV_INSTALL_PATH}/lib/libopencv_video.so ${OPENCV_INSTALL_PATH}/lib/libopencv_world.so)

# 打印opencv信息
message(STATUS "opencv version: ${OPENCV_VERSION}")
message(STATUS "opencv include dirs: ${OPENCV_INCLUDE_DIRS}")
message(STATUS "opencv libraries: ${OPENCV_LIBRARIES}")

install(DIRECTORY ${OPENCV_INSTALL_PATH}/include/opencv4 DESTINATION third_party/opencv/include/opencv4)

