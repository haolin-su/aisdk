# qnn根目录，获取环境变量qualcomm_QNN_ROOT
set(QNN_ROOT_DIR $ENV{QUALCOMMON_SDK_SNPE_ROOT})

# 获取QNN的版本信息，通过环境变量qualcomm_QNN_VERSION
set(QNN_VERSION $ENV{qualcomm_QNN_VERSION})

# 定义QNN_INCLUDE_DIRS
set(QNN_INCLUDE_DIRS ${QNN_ROOT_DIR}/include/QNN)
message(STATUS "QNN_INCLUDE_DIRS: ${QNN_INCLUDE_DIRS}")

# 定义QNN_LIBRARIES
set(QNN_LIBRARIES_DIRS ${QNN_ROOT_DIR}/lib/)

# 查看QNN目录是否存储，存在则定义QNN_FOUND
if(NOT EXISTS ${QNN_INCLUDE_DIRS} OR NOT EXISTS ${QNN_LIBRARIES_DIRS})
    set(QNN_FOUND FALSE)
else()
    set(QNN_FOUND TRUE)
endif()

if (QNN_FOUND)
    # 打印QNN信息
    message(STATUS "Found QNN: ${QNN_ROOT_DIR}")

    # 打印QNN版本信息
    message(STATUS "Found QNN version: ${QNN_VERSION}")

    include_directories(${QNN_INCLUDE_DIRS})

    # 根据交叉编译的类型决定使用哪个动态库路径
    message(STATUS "CMAKE_SYSTEM_PROCESSOR_ARCH: ${CMAKE_SYSTEM_PROCESSOR_ARCH}")
    message(STATUS "CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")
    message(STATUS "CMAKE_CXX_VERSION: ${CMAKE_CXX_VERSION}")
    message(STATUS "CMAKE_DSP_VERSION: ${CMAKE_DSP_VERSION}")
    if (CMAKE_SYSTEM_PROCESSOR_ARCH STREQUAL "aarch64")
        if (CMAKE_SYSTEM_NAME STREQUAL "oe")
            if (CMAKE_CXX_VERSION VERSION_GREATER_EQUAL 8.2)
                set(QNN_LIBRARIES_DIRS ${QNN_LIBRARIES_DIRS}/aarch64-oe-linux-gcc8.2)
            elseif (CMAKE_CXX_VERSION VERSION_GREATER_EQUAL 9.3)
                set(QNN_LIBRARIES_DIRS ${QNN_LIBRARIES_DIRS}/aarch64-oe-linux-gcc9.3)
            elseif (CMAKE_CXX_VERSION VERSION_GREATER_EQUAL 11.2)
                set(QNN_LIBRARIES_DIRS ${QNN_LIBRARIES_DIRS}/aarch64-oe-linux-gcc11.2)
            else()
                message(FATAL_ERROR "Unsupported compiler: ${CMAKE_SYSTEM_PROCESSOR_ARCH}-${CMAKE_SYSTEM_NAME}-${CMAKE_CXX_VERSION}")
            endif()
        elseif (CMAKE_SYSTEM_NAME STREQUAL "ubuntu")
            if (CMAKE_CXX_VERSION VERSION_GREATER_EQUAL 7.5)
                set(QNN_LIBRARIES_COMMON_DIRS ${QNN_LIBRARIES_DIRS}/aarch64-oe-linux-gcc7.5)
            elseif (CMAKE_CXX_VERSION VERSION_GREATER_EQUAL 9.4)
                set(QNN_LIBRARIES_COMMON_DIRS ${QNN_LIBRARIES_DIRS}/aarch64-oe-linuxapt -gcc9.4)
            else()
                message(FATAL_ERROR "Unsupported compiler: ${CMAKE_SYSTEM_PROCESSOR_ARCH}@${CMAKE_SYSTEM_NAME}-${CMAKE_CXX_VERSION}")
            endif()
        elseif (CMAKE_SYSTEM_NAME STREQUAL "windows")
            set(QNN_LIBRARIES_COMMON_DIRS ${QNN_LIBRARIES_DIRS}/aarch64-windows-msvc)
        elseif (CMAKE_SYSTEM_NAME STREQUAL "android")
            set(QNN_LIBRARIES_COMMON_DIRS ${QNN_LIBRARIES_DIRS}/aarch64-android)
        else()
            message(FATAL_ERROR "Unsupported compiler: ${CMAKE_SYSTEM_PROCESSOR_ARCH}-${CMAKE_SYSTEM_NAME}")
        endif()
    elseif (CMAKE_SYSTEM_PROCESSOR_ARCH STREQUAL "x86_64")
        if (CMAKE_SYSTEM STREQUAL "linux")
            set(QNN_LIBRARIES_COMMON_DIRS ${QNN_LIBRARIES_DIRS}/x86_64-linux-clang)
            set(QNN_LIBRARIES ${QNN_LIBRARIES} libGenie.so libQnnGenAiTransformerComposerQuantizer.so  
                libQnnGpuProfilingReader.so libQnnHtpQemu.so libQnnLpaiPrepare_v4.so  
                libQnnSystem.so  libHtpPrepare.so  libQnnGenAiTransformerCpuOpPkg.so  libQnnHtpNetRunExtensions.so  
                libQnnHtp.so  libQnnJsonProfilingReader.so  libQnnLpai.so  libQnnChrometraceProfilingReader.so  
                libQnnCpu.so  libQnnGenAiTransformerModel.so  libQnnHtpOptraceProfilingReader.so  libQnnHtpProfilingReader.so  
                libQnnLpaiNetRunExtensions.so  libQnnSaver.so)
        # elseif (CMAKE_SYSTEM_NAME STREQUAL "windows")
        #     set(QNN_LIBRARIES_DIRS ${QNN_LIBRARIES_DIRS}/x86_64-windows-msvc)
        else()
            message(FATAL_ERROR "Unsupported compiler: ${CMAKE_SYSTEM_PROCESSOR_ARCH}-${CMAKE_SYSTEM}")
        endif()
    else()
        message(FATAL_ERROR "Unsupported architecture: ${CMAKE_SYSTEM_PROCESSOR_ARCH}")
    endif()
    link_directories(${QNN_LIBRARIES_COMMON_DIRS})
    # "-lhta_hexagon_runtime_qnn 
    #                  -lhta_hexagon_runtime_qnn 
    #                  -lQnnDspV66CalculatorStub 
    #                  -lQnnDspV66Stub -lQnnHta 
    #                  -lQnnHtpV69Stub -lQnnTFLiteDelegate 
    #                  -lSnpeHtpV73Stub -lGenie 
    #                  -lQnnGenAiTransformerCpuOpPkg 
    #                  -lQnnGenAiTransformerModel 
    #                  -lQnnGenAiTransformer -lQnnGpuNetRunExtensions 
    #                  -lQnnGpuProfilingReader -lQnnGpu 
    #                  -lQnnHtaNetRunExtensions -lQnnHtpNetRunExtensions 
    #                  -lQnnHtpPrepare -lQnnHtpProfilingReader -lQnnHtp 
    #                  -lQnnHtpV73CalculatorStub -lQnnHtpV73Stub -lQnnHtpV75CalculatorStub"
    set(QNN_LIBRARIES ${QNN_LIBRARIES})
    # DSP的动态库放在hexagon-vxxx目录下，要根据版本来选择
    set(QNN_LIBRARIES_DSP_DIRS "")
    if (CMAKE_SYSTEM_PROCESSOR)
        if (CMAKE_DSP_VERSION VERSION_EQUAL 66)
            set(QNN_LIBRARIES_DSP_DIRS ${QNN_LIBRARIES_DIRS}/hexagon-v66)
        elseif (CMAKE_DSP_VERSION VERSION_EQUAL 68)
            set(QNN_LIBRARIES_DSP_DIRS ${QNN_LIBRARIES_DIRS}/hexagon-v68)
        elseif (CMAKE_DSP_VERSION VERSION_EQUAL 69)
            set(QNN_LIBRARIES_DSP_DIRS ${QNN_LIBRARIES_DIRS}/hexagon-v69)
        elseif (CMAKE_DSP_VERSION VERSION_EQUAL 73)
            set(QNN_LIBRARIES_DSP_DIRS ${QNN_LIBRARIES_DIRS}/hexagon-v73)
        elseif (CMAKE_DSP_VERSION VERSION_EQUAL 75)
            set(QNN_LIBRARIES_DSP_DIRS ${QNN_LIBRARIES_DIRS}/hexagon-v75)
        else()
            message(FATAL_ERROR "Unsupported DSP version: ${CMAKE_DSP_VERSION}")
        endif()

        link_directories(${QNN_LIBRARIES_DSP_DIRS}/unsigned/)
        # set(QNN_LIBRARIES ${QNN_LIBRARIES} libCalculator_skel.so  libQnnDspV${CMAKE_DSP_VERSION}Skel.so  libQnnDspV${CMAKE_DSP_VERSION}.so  libQnnSaver.so  libQnnSystem.so  libSnpeDspV${CMAKE_DSP_VERSION}Skel.so)    
    endif()

    # 设置设备库的前缀到DEVICE_LIBS，上层会调用
    set(DEVICE_LIBS ${QNN_LIBRARIES})
    message(STATUS "QNN_LIBRARIES: ${QNN_LIBRARIES}")
    set(REGISTRY_LIBRARIES "${REGISTER_LIBS};${QNN_LIBRARIES}")
    add_compile_definitions(WITH_QUALCOMM_QNN)
    # 添加qnn源码
    file(GLOB REGISTER_SRC_FILES ${CMAKE_SOURCE_DIR}/src/registry/devices/qualcomm/qnn/*.cpp)

    # 安装QNN_LIBRARIES_COMMON_DIRS、QNN_LIBRARIES_DSP_DIRS、QNN_INCLUDE_DIRS到安装目录
    # install(DIRECTORY ${QNN_LIBRARIES_COMMON_DIRS} ${QNN_LIBRARIES_DSP_DIRS} DESTINATION ${CMAKE_INSTALL_PREFIX}/third_party/qnn/lib)
    if (QNN_LIBRARIES_DSP_DIRS STREQUAL "")
        file(GLOB QNN_LIBRARIES_FILES "${QNN_LIBRARIES_COMMON_DIRS}/*")
    else()
        file(GLOB QNN_LIBRARIES_FILES "${QNN_LIBRARIES_COMMON_DIRS}/*" "${QNN_LIBRARIES_DSP_DIRS}/*")
    endif()
    
    message(STATUS "QNN_LIBRARIES_FILES: ${QNN_LIBRARIES_FILES}")
    install(FILES ${QNN_LIBRARIES_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/third_party/qnn/lib)
    # install(DIRECTORY ${QNN_INCLUDE_DIRS} DESTINATION ${CMAKE_INSTALL_PREFIX}/third_party/qnn/include)
    # install(FILES $ENV{ANDROID_NDK_ROOT}/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/aarch64-linux-android/libc++_shared.so DESTINATION ${CMAKE_INSTALL_PREFIX}/third_party/lib)

    # install(FILES ${CMAKE_SOURCE_DIR}/release/scripts/env_qualcomm_qnn.sh DESTINATION ${CMAKE_INSTALL_PREFIX}/scripts)
endif()