# Set the default target platform and ABI
if(NOT ANDROID_ABI)
    set(ANDROID_ABI "armeabi-v7a")
endif()

# Set the default API level if not already set
if(NOT ANDROID_NATIVE_API_LEVEL)
    set(ANDROID_NATIVE_API_LEVEL 21)
endif()

# Set the NDK path
set(ANDROID_NDK $ENV{ANDROID_NDK_ROOT})

# Set the toolchain and other variables
set(CMAKE_SYSTEM_NAME android)
set(CMAKE_SYSTEM_VERSION ${ANDROID_NATIVE_API_LEVEL})
# set(CMAKE_SYSTEM_PROCESSOR ${ANDROID_ABI})

# Set the compiler executable paths
set(ANDROID_NDK_TOOLCHAIN_PATH ${ANDROID_NDK}/toolchains/llvm/prebuilt/linux-x86_64/bin)
set(CMAKE_C_COMPILER ${ANDROID_NDK_TOOLCHAIN_PATH}/aarch64-linux-android${ANDROID_NATIVE_API_LEVEL}-clang)
set(CMAKE_CXX_COMPILER ${ANDROID_NDK_TOOLCHAIN_PATH}/aarch64-linux-android${ANDROID_NATIVE_API_LEVEL}-clang++)
set(CMAKE_AR ${ANDROID_NDK}/toolchains/llvm/prebuilt/linux-x86_64/bin/llvm-ar)

# Set the target ABI flags
set(CMAKE_ANDROID_ARCH_ABI ${ANDROID_ABI})
set(CMAKE_ANDROID_NDK_TOOLCHAIN_VERSION clang)
set(CMAKE_ANDROID_STL_TYPE c++_shared)

# Set the CMake toolchain file
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_LIST_FILE}")
