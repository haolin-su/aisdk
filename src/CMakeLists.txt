include_directories(${CMAKE_SOURCE_DIR}/src)

add_subdirectory(common)
add_subdirectory(registry)
add_subdirectory(core)
add_subdirectory(api)
add_subdirectory(app)

# 如果是android，则添加jni目录
if (BUILD_SYSTEM_NAME STREQUAL "android")
    #add_subdirectory(jni)
endif()
