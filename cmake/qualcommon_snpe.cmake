# snpe根目录，获取环境变量qualcomm_SNPE_ROOT
set(SNPE_ROOT_DIR $ENV{QUALCOMMON_SDK_SNPE_ROOT})

# 获取SNPE的版本信息，通过环境变量qualcomm_SNPE_VERSION
set(SNPE_VERSION $ENV{qualcomm_SNPE_VERSION})

# 定义SNPE_INCLUDE_DIRS
set(SNPE_INCLUDE_DIRS ${SNPE_ROOT_DIR}/include/SNPE)

# 定义SNPE_LIBRARIES
set(SNPE_LIBRARIES_DIRS ${SNPE_ROOT_DIR}/lib/)
message("#######################SNPE LIBRARIES: ${SNPE_LIBRARIES_DIRS}, ${SNPE_ROOT_DIR}")

# 查看SNPE目录是否存储，存在则定义SNPE_FOUND
if(NOT EXISTS ${SNPE_INCLUDE_DIRS} OR NOT EXISTS ${SNPE_LIBRARIES_DIRS})
    set(SNPE_FOUND FALSE)
else()
    set(SNPE_FOUND TRUE)
endif()

message("SNPE_FOUND:${SNPE_FOUND}")

if (SNPE_FOUND)
    # 打印SNPE信息
    message(STATUS "Found SNPE: ${SNPE_ROOT_DIR}")

    # 打印SNPE版本信息
    message(STATUS "Found SNPE version: ${SNPE_VERSION}")

    include_directories(${SNPE_INCLUDE_DIRS})
    include_directories(${SNPE_ROOT_DIR}/include)

    # 根据交叉编译的类型决定使用哪个动态库路径
    message(STATUS "CMAKE_SYSTEM_PROCESSOR_ARCH: ${CMAKE_SYSTEM_PROCESSOR_ARCH}")
    message(STATUS "BUILD_SYSTEM_NAME: ${BUILD_SYSTEM_NAME}")
    message(STATUS "CMAKE_CXX_VERSION: ${CMAKE_CXX_VERSION}")
    message(STATUS "CMAKE_DSP_VERSION: ${CMAKE_DSP_VERSION}")

    set(SNPE_LIBRARIES_FILES "")

    if (CMAKE_SYSTEM_PROCESSOR_ARCH STREQUAL "aarch64")
        if (BUILD_SYSTEM_NAME STREQUAL "oe")
            if (CMAKE_CXX_VERSION VERSION_GREATER_EQUAL 8.2)
                set(SNPE_LIBRARIES_DIRS ${SNPE_LIBRARIES_DIRS}/aarch64-oe-linux-gcc8.2)
            elseif (CMAKE_CXX_VERSION VERSION_GREATER_EQUAL 9.3)
                set(SNPE_LIBRARIES_DIRS ${SNPE_LIBRARIES_DIRS}/aarch64-oe-linux-gcc9.3)
            elseif (CMAKE_CXX_VERSION VERSION_GREATER_EQUAL 11.2)
                set(SNPE_LIBRARIES_DIRS ${SNPE_LIBRARIES_DIRS}/aarch64-oe-linux-gcc11.2)
            else()
                message(FATAL_ERROR "Unsupported compiler: ${CMAKE_SYSTEM_PROCESSOR_ARCH}-${BUILD_SYSTEM_NAME}-${CMAKE_CXX_VERSION}")
            endif()
        elseif (BUILD_SYSTEM_NAME STREQUAL "ubuntu")
            if (CMAKE_CXX_VERSION VERSION_GREATER_EQUAL 7.5)
                set(SNPE_LIBRARIES_COMMON_DIRS ${SNPE_LIBRARIES_DIRS}/aarch64-oe-linux-gcc7.5)
            elseif (CMAKE_CXX_VERSION VERSION_GREATER_EQUAL 9.4)
                set(SNPE_LIBRARIES_COMMON_DIRS ${SNPE_LIBRARIES_DIRS}/aarch64-oe-linux-gcc9.4)
            else()
                message(FATAL_ERROR "Unsupported compiler: ${CMAKE_SYSTEM_PROCESSOR_ARCH}-${BUILD_SYSTEM_NAME}-${CMAKE_CXX_VERSION}")
            endif()
        elseif (BUILD_SYSTEM_NAME STREQUAL "windows")
            set(SNPE_LIBRARIES_COMMON_DIRS ${SNPE_LIBRARIES_DIRS}/aarch64-windows-msvc)
        elseif (BUILD_SYSTEM_NAME STREQUAL "android")
            set(SNPE_LIBRARIES_COMMON_DIRS ${SNPE_LIBRARIES_DIRS}/aarch64-android)
            set(SNPE_LIBRARIES ${SNPE_LIBRARIES} libGenie.so
                libSnpeHta.so     libSnpeHtpV73CalculatorStub.so  libhta_hexagon_runtime_snpe.so
                libPlatformValidatorShared.so        libSnpeHtpPrepare.so            libSnpeHtpV73Stub.so
                libSnpeHtpV68CalculatorStub.so  libSnpeHtpV75CalculatorStub.so
                libSnpeHtpV68Stub.so            libSnpeHtpV75Stub.so libSNPE.so libSnpeHtpV69CalculatorStub.so  libcalculator.so         libhta_hexagon_runtime_qnn.so)
            foreach(file ${SNPE_LIBRARIES})
                # message(STATUS "file: ${file}")
                # 补充路径前缀
                set(file_path "${SNPE_LIBRARIES_COMMON_DIRS}/${file}")
                message(STATUS "file_path: ${file_path}")
                set(SNPE_LIBRARIES_FILES "${SNPE_LIBRARIES_FILES} ${file_path}")
            endforeach()
        else()
            message(FATAL_ERROR "Unsupported compiler: ${CMAKE_SYSTEM_PROCESSOR_ARCH}-${BUILD_SYSTEM_NAME}")
        endif()
    elseif (CMAKE_SYSTEM_PROCESSOR_ARCH STREQUAL "x86_64")
        if (BUILD_SYSTEM_NAME STREQUAL "linux")
            set(SNPE_LIBRARIES_COMMON_DIRS ${SNPE_LIBRARIES_DIRS}/x86_64-linux-clang)
            # set(SNPE_LIBRARIES ${SNPE_LIBRARIES} libGenie.so libQnnGenAiTransformerCpuOpPkg.so libQnnHtpProfilingReader.so libQnnModelDlc.so libHtpPrepare.so libQnnGenAiTransformerModel.so libQnnHtpQemu.so libQnnSaver.so libQnnChrometraceProfilingReader.so libQnnGpuProfilingReader.so libQnnJsonProfilingReader.so libQnnSystem.so libQnnCpu.so libQnnHtp.so libQnnLpai.so libSNPE.so libQnnGenAiTransformer.so libQnnHtpNetRunExtensions.so libQnnLpaiNetRunExtensions.so libQnnGenAiTransformerComposerQuantizer.so libQnnHtpOptraceProfilingReader.so libQnnLpaiPrepare_v4.so)
            set(SNPE_LIBRARIES ${SNPE_LIBRARIES} libGenie.so  libHtpPrepare.so libSNPE.so )
            foreach(file ${SNPE_LIBRARIES})
                # message(STATUS "file: ${file}")
                # 补充路径前缀
                set(file_path "${SNPE_LIBRARIES_COMMON_DIRS}/${file}")
                message(STATUS "file_path: ${file_path}")
                set(SNPE_LIBRARIES_FILES "${SNPE_LIBRARIES_FILES} ${file_path}")
            endforeach()
        # elseif (BUILD_SYSTEM_NAME STREQUAL "windows")
        #     set(SNPE_LIBRARIES_DIRS ${SNPE_LIBRARIES_DIRS}/x86_64-windows-msvc)
        else()
            message(FATAL_ERROR "Unsupported compiler: ${CMAKE_SYSTEM_PROCESSOR_ARCH}-${CMAKE_SYSTEM}")
        endif()
    else()
        message(FATAL_ERROR "Unsupported architecture: ${CMAKE_SYSTEM_PROCESSOR_ARCH}")
    endif()
    link_directories(${SNPE_LIBRARIES_COMMON_DIRS})
    # "-lhta_hexagon_runtime_qnn 
    #                  -lhta_hexagon_runtime_snpe 
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
    set(SNPE_LIBRARIES ${SNPE_LIBRARIES})
    # DSP的动态库放在hexagon-vxxx目录下，要根据版本来选择
    set(SNPE_LIBRARIES_DSP_DIRS "")
    if (CMAKE_SYSTEM_PROCESSOR)
        if (CMAKE_DSP_VERSION VERSION_EQUAL 66)
            set(SNPE_LIBRARIES_DSP_DIRS ${SNPE_LIBRARIES_DIRS}/hexagon-v66)
        elseif (CMAKE_DSP_VERSION VERSION_EQUAL 68)
            set(SNPE_LIBRARIES_DSP_DIRS ${SNPE_LIBRARIES_DIRS}/hexagon-v68)
        elseif (CMAKE_DSP_VERSION VERSION_EQUAL 69)
            set(SNPE_LIBRARIES_DSP_DIRS ${SNPE_LIBRARIES_DIRS}/hexagon-v69)
        elseif (CMAKE_DSP_VERSION VERSION_EQUAL 73)
            set(SNPE_LIBRARIES_DSP_DIRS ${SNPE_LIBRARIES_DIRS}/hexagon-v73)
        elseif (CMAKE_DSP_VERSION VERSION_EQUAL 75)
            set(SNPE_LIBRARIES_DSP_DIRS ${SNPE_LIBRARIES_DIRS}/hexagon-v75)
        else()
            message(FATAL_ERROR "Unsupported DSP version: ${CMAKE_DSP_VERSION}")
        endif()

        link_directories(${SNPE_LIBRARIES_DSP_DIRS}/unsigned/)
        # set(SNPE_LIBRARIES ${SNPE_LIBRARIES} libCalculator_skel.so  libQnnDspV${CMAKE_DSP_VERSION}Skel.so  libQnnDspV${CMAKE_DSP_VERSION}.so  libQnnSaver.so  libQnnSystem.so  libSnpeDspV${CMAKE_DSP_VERSION}Skel.so)    
    endif()

    # 设置设备库的前缀到DEVICE_LIBS，上层会调用
    set(DEVICE_LIBS ${SNPE_LIBRARIES})
    message(STATUS "SNPE_LIBRARIES: ${SNPE_LIBRARIES}")
    set(REGISTRY_LIBRARIES "${REGISTER_LIBS};${SNPE_LIBRARIES}")
    add_compile_definitions(WITH_QUALCOMM_SNPE)
    # 添加snpe源码
    file(GLOB REGISTER_SRC_FILES ${CMAKE_SOURCE_DIR}/src/registry/devices/qualcomm/snpe/*.cc)

    # 安装SNPE_LIBRARIES_COMMON_DIRS、SNPE_LIBRARIES_DSP_DIRS、SNPE_INCLUDE_DIRS到安装目录
    # install(DIRECTORY ${SNPE_LIBRARIES_COMMON_DIRS} ${SNPE_LIBRARIES_DSP_DIRS} DESTINATION ${CMAKE_INSTALL_PREFIX}/third_party/snpe/lib)
    # if (SNPE_LIBRARIES_DSP_DIRS STREQUAL "")
    #     file(GLOB SNPE_LIBRARIES_FILES "${SNPE_LIBRARIES_COMMON_DIRS}/*")
    # else()
    #     file(GLOB SNPE_LIBRARIES_FILES "${SNPE_LIBRARIES_COMMON_DIRS}/*" "${SNPE_LIBRARIES_DSP_DIRS}/*")
    # endif()

    # message("SNPE_LIBRARIES_COMMON_DIRS: ${SNPE_LIBRARIES_COMMON_DIRS}")
    
    # foreach(file ${SNPE_LIBRARIES})
    #     message(STATUS "file: ${file}")
    #     string(REPLACE "${SNPE_LIBRARIES_COMMON_DIRS}" "" file_path ${file})
    #     message(STATUS "file_path: ${file_path}")
    #     set(SNPE_LIBRARIES_FILES "${SNPE_LIBRARIES_FILES} ${file_path}")
    # endforeach()

    # SNPE_LIBRARIES_FILES 需要改为包含路径前缀来install
    # message(STATUS "SNPE_LIBRARIES_FILES: ${SNPE_LIBRARIES_FILES}")
    # foreach(file ${SNPE_LIBRARIES_FILES})
    #     message(STATUS "file: ${file}")
    #     install(FILES ${file} DESTINATION ${CMAKE_INSTALL_PREFIX}/third_party/snpe/lib)
    # endforeach()
    # install(FILES ${SNPE_LIBRARIES_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/third_party/snpe/lib)
    install(DIRECTORY ${SNPE_INCLUDE_DIRS} DESTINATION ${CMAKE_INSTALL_PREFIX}/third_party/snpe/include)
    # install(FILES $ENV{ANDROID_NDK_ROOT}/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/aarch64-linux-android/libc++_shared.so DESTINATION ${CMAKE_INSTALL_PREFIX}/third_party/lib)
    install(FILES /opt/android-ndk-r26c/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/aarch64-linux-android/libc++_shared.so DESTINATION ${CMAKE_INSTALL_PREFIX}/third_party/lib)

    # install(FILES ${CMAKE_SOURCE_DIR}/release/scripts/env_qualcomm_snpe.sh DESTINATION ${CMAKE_INSTALL_PREFIX}/scripts)
endif()
