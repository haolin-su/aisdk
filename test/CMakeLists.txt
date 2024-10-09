
message("Device libs: ${DEVICE_LIBS}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/registry)
set(SRC_UTIL registry/command_parser/command_parser.cc)

# add_executable(test_snpe_runtime registry/test_snpe_runtime.cc)
# target_link_libraries(test_snpe_runtime ${DEVICE_LIBS})
# install(TARGETS test_snpe_runtime RUNTIME DESTINATION test)

add_executable(test_qnn_runtime registry/test_qnn_runtime.cc)
target_link_libraries(test_qnn_runtime ${DEVICE_LIBS})
install(TARGETS test_qnn_runtime RUNTIME DESTINATION test)

add_executable(test_hello registry/test_hello.cc)
# target_link_libraries(test_hello ${DEVICE_LIBS})
install(TARGETS test_hello RUNTIME DESTINATION test)

add_executable(test_qnn_runtime_multimodel registry/qnn/test_qnn_runtime_multimodel.cc registry/qnn/qnn_util.cc ${SRC_UTIL})
target_link_libraries(test_qnn_runtime_multimodel ${DEVICE_LIBS})
install(TARGETS test_qnn_runtime_multimodel RUNTIME DESTINATION test)

include_directories(${CMAKE_SOURCE_DIR}/src)
add_executable(test_yolo registry/test_yolo.cc)
target_link_libraries(test_yolo aisdk_api aisdk_core aisdk_registry aisdk_common opencv_core opencv_imgproc opencv_imgcodecs opencv_dnn)
install(TARGETS test_yolo RUNTIME DESTINATION test)