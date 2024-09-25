file(GLOB_RECURSE COMMON_SRC_FILES "*.cpp")

add_library(aisdk_common SHARED ${COMMON_SRC_FILES})

target_link_libraries(aisdk_common PUBLIC ${OpenCV_LIBS})

install(TARGETS aisdk_common DESTINATION lib)
