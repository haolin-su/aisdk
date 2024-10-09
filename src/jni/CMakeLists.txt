file(GLOB_RECURSE REGISTRY_SRC_FILES "*.cc")

add_library(aisdk_jni SHARED ${REGISTRY_SRC_FILES})

set(OPENCV_LIBRARIES "opencv_core;opencv_imgcodecs;opencv_imgproc;")
target_link_libraries(aisdk_jni PUBLIC aisdk_api aisdk_core aisdk_registry aisdk_common ${OPENCV_LIBRARIES})
install(TARGETS aisdk_jni DESTINATION lib)