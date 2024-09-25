
message("Device libs: ${DEVICE_LIBS}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/registry)
set(SRC_UTIL registry/command_parser/command_parser.cpp)

# add_executable(test_snpe_runtime registry/test_snpe_runtime.cpp)
# target_link_libraries(test_snpe_runtime ${DEVICE_LIBS})
# install(TARGETS test_snpe_runtime RUNTIME DESTINATION test)

add_executable(test_qnn_runtime registry/test_qnn_runtime.cpp)
target_link_libraries(test_qnn_runtime ${DEVICE_LIBS})
install(TARGETS test_qnn_runtime RUNTIME DESTINATION test)

add_executable(test_hello registry/test_hello.cpp)
# target_link_libraries(test_hello ${DEVICE_LIBS})
install(TARGETS test_hello RUNTIME DESTINATION test)

add_executable(test_qnn_runtime_multimodel registry/qnn/test_qnn_runtime_multimodel.cpp registry/qnn/qnn_util.cpp ${SRC_UTIL})
target_link_libraries(test_qnn_runtime_multimodel ${DEVICE_LIBS})
install(TARGETS test_qnn_runtime_multimodel RUNTIME DESTINATION test)
