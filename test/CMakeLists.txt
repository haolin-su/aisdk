
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


include_directories(${CMAKE_SOURCE_DIR}/src)
set(APP_YOLO_SOURCES
    "registry/test_yolo.cc"
    "registry/yolo_snpe/Util.cpp"
    "registry/yolo_snpe/SetBuilderOptions.cpp"
    "registry/yolo_snpe/SetBuilderOptions.hpp"
    "registry/yolo_snpe/SaveOutputTensor.cpp"
    "registry/yolo_snpe/GetOpt.cpp"
    "registry/yolo_snpe/GetOpt.hpp"
    "registry/yolo_snpe/SaveOutputTensor.hpp"
    "registry/yolo_snpe/Util.hpp"
    "registry/yolo_snpe/NV21Load.cpp"
    "registry/yolo_snpe/NV21Load.hpp"
    "registry/yolo_snpe/LoadInputTensor.cpp"
    "registry/yolo_snpe/LoadInputTensor.hpp"
    "registry/yolo_snpe/CheckRuntime.cpp"
    "registry/yolo_snpe/CheckRuntime.hpp"
    "registry/yolo_snpe/LoadContainer.cpp"
    "registry/yolo_snpe/LoadContainer.hpp"
    "registry/yolo_snpe/PreprocessInput.cpp"
    "registry/yolo_snpe/PreprocessInput.hpp"
    "registry/yolo_snpe/CreateUserBuffer.cpp"
    "registry/yolo_snpe/CreateUserBuffer.hpp"
    "registry/yolo_snpe/LoadUDOPackage.cpp"
    "registry/yolo_snpe/LoadUDOPackage.hpp"
    "registry/yolo_snpe/CreateGLBuffer.cpp"
    "registry/yolo_snpe/CreateGLBuffer.hpp"
)
include_directories(/opt/qcom/aistack/qairt/2.26.2.240911/include/SNPE)

add_executable(test_yolo ${APP_YOLO_SOURCES})
target_link_libraries(test_yolo ${DEVICE_LIBS} aisdk_api aisdk_core aisdk_registry aisdk_common opencv_core opencv_imgproc opencv_imgcodecs opencv_dnn /opt/qcom/aistack/qairt/2.26.2.240911/lib/aarch64-android/libSNPE.so)
install(TARGETS test_yolo RUNTIME DESTINATION test)

add_executable(test_qnn_runtime_multimodel registry/qnn/test_qnn_runtime_multimodel.cc registry/qnn/qnn_util.cc ${SRC_UTIL})
target_link_libraries(test_qnn_runtime_multimodel ${DEVICE_LIBS})
install(TARGETS test_qnn_runtime_multimodel RUNTIME DESTINATION test)
