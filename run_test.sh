export SNPE_TARGET_ARCH=aarch64-android
export SNPE_TARGET_DSPARCH=hexagon-v68
export SNPE_LOCAL_ROOT=/opt/qcom/aistack/qairt/2.26.2.240911
export ANDROID_NDK_LOCAL_ROOT=/opt/android-ndk-r26c
export FIBO_AISDK_LOACL_ROOT=/home/fibo/work/aisdk
export FIBO_AISDK_INSTALL_ROOT=/home/fibo/work/aisdk/install/qualcommon_aarch64_android

# ## 只需要推送一次库即可
# adb shell "mkdir -p /data/local/tmp/fibo_aisdk/$SNPE_TARGET_ARCH/bin"
# ## 高通的库
# adb shell "mkdir -p /data/local/tmp/fibo_aisdk/$SNPE_TARGET_ARCH/lib"
# ## hexagon的库
# adb shell "mkdir -p /data/local/tmp/fibo_aisdk/$SNPE_TARGET_DSPARCH/lib"
# ## aisdk的库，目前库的链接有问题，需要底下的workaround
# adb shell "mkdir -p /data/local/tmp/fibo_aisdk/$SNPE_TARGET_ARCH/aisdk_lib"
# ## aisdk的第三方库
# adb shell "mkdir -p /data/local/tmp/fibo_aisdk/$SNPE_TARGET_ARCH/aisdk_third_party_lib"
# ## aisdk的test
# adb shell "mkdir -p /data/local/tmp/fibo_aisdk/test"
# ## aisdk的sample
# adb shell "mkdir -p /data/local/tmp/fibo_aisdk/sample"

# ## snpe_net-run是为了测试snpe的运行环境是否正常
# adb push $SNPE_LOCAL_ROOT/bin/$SNPE_TARGET_ARCH/snpe-net-run \
#       /data/local/tmp/fibo_aisdk/$SNPE_TARGET_ARCH/bin
# ## 推送高通的QNN和SNPE库
# adb push $SNPE_LOCAL_ROOT/lib/$SNPE_TARGET_ARCH/*.so \
#       /data/local/tmp/fibo_aisdk/$SNPE_TARGET_ARCH/lib
# ## 推送安卓的libc++_shared.so库
# adb push ${ANDROID_NDK_LOCAL_ROOT}/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/aarch64-linux-android/libc++_shared.so \
#     /data/local/tmp/fibo_aisdk/$SNPE_TARGET_ARCH/lib
# ## 推送hexagon的库
# adb push $SNPE_LOCAL_ROOT/lib/$SNPE_TARGET_DSPARCH/unsigned/*.so \
#       /data/local/tmp/fibo_aisdk/$SNPE_TARGET_DSPARCH/lib
# ## 目前的FIBO AISDK的库链接有问题，或者是这边环境的设置没有弄好
# adb push ${FIBO_AISDK_INSTALL_ROOT}/lib/* \
#     /data/local/tmp/fibo_aisdk/$SNPE_TARGET_ARCH/aisdk_lib
# ## 推送fibo_aisdk的第三方库
# adb push ${FIBO_AISDK_LOACL_ROOT}/third_party/opencv-4.8.0-install-android-aarch64/lib/*.so \
#     /data/local/tmp/fibo_aisdk/$SNPE_TARGET_ARCH/aisdk_third_party_lib
# adb push ${FIBO_AISDK_LOACL_ROOT}/third_party/spdlog-install-android-aarch64/lib/*.so \
#     /data/local/tmp/fibo_aisdk/$SNPE_TARGET_ARCH/aisdk_third_party_lib

# ## 创建ANDROID上的测试运行环境文件夹结构(workaround)
# adb shell "mkdir -p /data/local/tmp/fibo_aisdk/src/api"
# adb shell "mkdir -p /data/local/tmp/fibo_aisdk/src/common" 
# adb shell "mkdir -p /data/local/tmp/fibo_aisdk/src/core" 
# adb shell "mkdir -p /data/local/tmp/fibo_aisdk/src/registry" 

adb push ${FIBO_AISDK_INSTALL_ROOT}/lib/libaisdk_api.so /data/local/tmp/fibo_aisdk/src/api/libaisdk_api.so
adb push ${FIBO_AISDK_INSTALL_ROOT}/lib/libaisdk_common.so /data/local/tmp/fibo_aisdk/src/common/libaisdk_common.so
adb push ${FIBO_AISDK_INSTALL_ROOT}/lib/libaisdk_core.so /data/local/tmp/fibo_aisdk/src/core/libaisdk_core.so
adb push ${FIBO_AISDK_INSTALL_ROOT}/lib/libaisdk_registry.so /data/local/tmp/fibo_aisdk/src/registry/libaisdk_registry.so

adb shell "cp -r /data/local/tmp/fibo_aisdk/src/common /data/local/tmp/fibo_aisdk/" 
adb shell "cp -r /data/local/tmp/fibo_aisdk/src/registry /data/local/tmp/fibo_aisdk/"

## 推送测试程序
adb push ${FIBO_AISDK_INSTALL_ROOT}/test/test_yolo_det /data/local/tmp/fibo_aisdk/test/
adb push ${FIBO_AISDK_LOACL_ROOT}/test/registry/yolov8_det.json /data/local/tmp/fibo_aisdk/test/
# adb push ${FIBO_AISDK_INSTALL_ROOT}/test/test_qnn_runtime /data/local/tmp/fibo_aisdk/test/

adb push ${FIBO_AISDK_INSTALL_ROOT}/lib/libaisdk_common.so /data/local/tmp/fibo_aisdk/common/libaisdk_common.so
adb push ${FIBO_AISDK_INSTALL_ROOT}/lib/libaisdk_registry.so /data/local/tmp/fibo_aisdk/registry/libaisdk_registry.so
adb push ${FIBO_AISDK_INSTALL_ROOT}/test/test_yolo /data/local/tmp/fibo_aisdk/
#adb push ${FIBO_AISDK_LOACL_ROOT}/test/registry/input_tensor.raw /data/local/tmp/fibo_aisdk/
#adb push ${FIBO_AISDK_LOACL_ROOT}/test/registry/outputs_0.raw /data/local/tmp/fibo_aisdk/
#adb push ${FIBO_AISDK_LOACL_ROOT}/test/registry/outputs_1.raw /data/local/tmp/fibo_aisdk/
# adb push ${FIBO_AISDK_LOACL_ROOT}/test/registry/seg_input_1.raw /data/local/tmp/fibo_aisdk/
# adb push ${FIBO_AISDK_LOACL_ROOT}/test/registry/seg_input_2.raw /data/local/tmp/fibo_aisdk/
# ## 以下是安卓上要做的事
# export SNPE_TARGET_ARCH=aarch64-android
# export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/data/local/tmp/fibo_aisdk/$SNPE_TARGET_ARCH/lib:/data/local/tmp/fibo_aisdk/$SNPE_TARGET_DSPARCH/lib:/data/local/tmp/fibo_aisdk/$SNPE_TARGET_ARCH/aisdk_lib:/data/local/tmp/fibo_aisdk/$SNPE_TARGET_ARCH/aisdk_third_party_lib
# export PATH=$PATH:/data/local/tmp/fibo_aisdk/$SNPE_TARGET_ARCH/bin
# cd /data/local/tmp/fibo_aisdk/test
# ./test_yolo_det
