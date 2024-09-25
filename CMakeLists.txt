cmake_minimum_required (VERSION 3.20)
project(aisdk)
set(CMAKE_VERBOSE_MAKEFILE ON)

# if (CMAKE_BUILD_TYPE STREQUAL "")
#     message(STATUS "CMAKE_BUILD_TYPE not defined, 'Release' will be used")
#     set(CMAKE_BUILD_TYPE "Release")
# endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)

# if(CMAKE_COMPILER_IS_GNUCXX)
    # set(CMAKE_CXX_FLAGS "-std=c++1 ${CMAKE_CXX_FLAGS}")
    # set(CMAKE_CXX_STANDARD 17)
# endif(CMAKE_COMPILER_IS_GNUCXX)
    # set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -v")

    set(CMAKE_CXX_FLAGS   "${CMAKE_CXX_FLAGS} -std=c++11")             # c++17
    set(CMAKE_CXX_FLAGS   "${CMAKE_CXX_FLAGS} -fPIE")             # c++17
    # set(CMAKE_CXX_FLAGS   "Wl,-Bsymbolic")                     # 调试信息
    # # set(CMAKE_CXX_FLAGS_DEBUG   "-g")                     # 调试信息
    set(CMAKE_CXX_FLAGS   "${CMAKE_CXX_FLAGS} -Wall")                  # 开启所有警告
    set(CMAKE_CXX_FLAGS   "${CMAKE_CXX_FLAGS}  -Wno-unused-variable -fno-operator-names -Wno-comment -Wno-unused-but-set-variable -Wno-unused-function")                # 开启额外警告
    # if (CMAKE_BUILD_TYPE STREQUAL "Debug") # -Wno-error=deprecated-declarations -Werror -Werror=reorder
    #     set(CMAKE_CXX_FLAGS   "${CMAKE_CXX_FLAGS} -fopenmp -g -DTIC_EXPORT=0" )             # 调试包不优化
    # else()
    #     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -O3 -DTIC_EXPORT=1" )   # release包优化
    # endif()
# set(CMAKE_CXX_FLAGS   "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")
# set(CMAKE_CXX_FLAGS   "${CMAKE_CXX_FLAGS} -fopenmp -O1 -DTIC_EXPORT=0 -Werror=return-type" )          

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
# set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS}")

#从文件中读取版本信息,将内容读入 PROGRAM_VERSION 变量中，该变量会去掉尾部的空格。当设置了变量 PROGRAM_VERSION ，就可以使用它来配置 version.hpp.in ，生成 generated/version.hpp 
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/VERSION")
    file(READ "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" PROJECT_VERSION)
    string(STRIP "${PROJECT_VERSION}" PROJECT_VERSION)

    # 逐行读取文件内容
    file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" PROJECT_VERSION)
    message(STATUS "!!Project version: ${PROJECT_VERSION}")
    # 第一行赋值给PROJECT_APP_VERSION, 第二行赋值给PROJECT_API_VERSION， 第三行赋值给PROJECT_CORE_VERSION， 第四行赋值给PROJECT_COMMON_VERSION， 第五行赋值给PROJECT_REGISTER_VERSION
    list(GET PROJECT_VERSION 0 PROJECT_APP_VERSION)
    list(GET PROJECT_VERSION 1 PROJECT_API_VERSION)
    list(GET PROJECT_VERSION 2 PROJECT_CORE_VERSION)
    list(GET PROJECT_VERSION 3 PROJECT_COMMON_VERSION)
    list(GET PROJECT_VERSION 4 PROJECT_REGISTER_VERSION)

    message(STATUS "Project app version: ${PROJECT_APP_VERSION}")
    message(STATUS "Project api version: ${PROJECT_API_VERSION}")
    message(STATUS "Project core version: ${PROJECT_CORE_VERSION}")
    message(STATUS "Project common version: ${PROJECT_COMMON_VERSION}")
    message(STATUS "Project register version: ${PROJECT_REGISTER_VERSION}")
else()
    message(FATAL_ERROR "File ${CMAKE_CURRENT_SOURCE_DIR}/VERSION not found")
endif()
message(STATUS "Program version: ${PROJECT_VERSION}")

# generate file version.hpp based on version.hpp.in
configure_file(
    VERSION
    ${CMAKE_CURRENT_SOURCE_DIR}/include/version.h
    @ONLY
)

# 设置编译选项
option(CMAKE_SYSTEM_PROCESSOR_ARCH "System processor architecture" "aarch64")
option(CMAKE_SYSTEM_NAME "System name" "Linux")
option(CMAKE_CXX_VERSION "C++ version" "14")
option(CMAKE_DSP_VERSION "DSP version" "68")
option(BUILD_STATIC_LIBS "Build static libraries" OFF)
option(BUILD_TEST "Build test" ON)
option(BUILD_SAMPLE "Build sample" ON)
option(BUILD_ALL_LIB "Build all library" ON)

# set(CMAKE_CXX_STANDARD ${CMAKE_CXX_VERSION})

set(BUILD_SYSTEM_NAME "linux" CACHE STRING "Build system name")
message(STATUS "BUILD_TARGET_CHIP: ${BUILD_TARGET_CHIP} BUILD_SYSTEM_NAME:${BUILD_SYSTEM_NAME}")

option(BUILD_ALL_LIB "Build all library" ON)
option(BUILD_PYTHON "Build python" OFF)



include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# add_compile_definitions(CNIS_RECORD_PERF) 

# 根据编译链的不同，设置不同的编译选项
# 如果是交叉编译链，设置TARGET_ARCH变量为"aaarch64"；如果是x86编译链，设置TARGET_ARCH变量为"x86"
message(${CMAKE_CXX_COMPILER})
if (CMAKE_CXX_COMPILER MATCHES "aarch64")

    message("Build on aarch64")
    set(TARGET_ARCH "aarch64")
elseif(CMAKE_CXX_COMPILER MATCHES "clang")
    message("Build on clang")
    set(TARGET_ARCH "aarch64")
else() #if (CMAKE_CXX_COMPILER MATCHES "x86")
    message("Build on x86")
    set(TARGET_ARCH "x86")
endif()


file(GLOB CMakeFiles "cmake/*.cmake")
foreach (cmake_module_file IN LISTS CMakeFiles)
    # get_filename_component(CMakeName ${cmake_module_file} NAME_WLE)
    get_filename_component(CMakePath ${cmake_module_file} DIRECTORY)
    message(STATUS "LoadCMake: ${cmake_module_file}")
    include(${cmake_module_file})
endforeach (cmake_module_file)

# NLP会包含一些第三份依赖库
# third-party libraries
# set(ABSL_ENABLE_INSTALL ON)
# set(ABSL_PROPAGATE_CXX_STD ON)
# add_subdirectory(${CMAKE_SOURCE_DIR}/third_party/abseil-cpp)
# add_subdirectory(${CMAKE_SOURCE_DIR}/third_party/re2)

add_subdirectory(src)

if (BUILD_TEST)
    message("Build test folder")
    add_subdirectory(test)
endif()

if (BUILD_SAMPLE)
    message("Build sample folder")
    add_subdirectory(sample)
endif()

if (BUILD_PYTHON)
    message("Build python folder")
    add_subdirectory(python)
endif()

add_subdirectory(release)

# if (NOT DEFINED CMAKE_INSTALL_PREFIX)
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install" CACHE PATH "installation directory")
# endif ()
message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")

# 安装头文件
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_PREFIX}/include)