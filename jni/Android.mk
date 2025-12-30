LOCAL_PATH := $(call my-dir)

# ===============================================
# libnng_jni.so - JNIラッパー（Android Java向け）
# ===============================================
#
# このファイルは Android Java アプリ向けのJNIラッパーをビルドします。
# libnng.so（純粋なNNG共有ライブラリ）と組み合わせて使用します。
#
# 使用方法:
#   ndk-build NDK_PROJECT_PATH=. NDK_APPLICATION_MK=./jni/Application.mk \
#             APP_BUILD_SCRIPT=./jni/Android_jni.mk
#
# 成果物:
#   libs/<arch>/libnng_jni.so
#
# 依存:
#   libnng-shared-android-all/<arch>/libnng.so (GitHub Actionsで生成)
# ===============================================

# NNG共有ライブラリのインポート（純粋なNNG、JNIなし）
include $(CLEAR_VARS)
LOCAL_MODULE := nng
LOCAL_SRC_FILES := ../../nng-shared-library/libnng-shared-android-all/$(TARGET_ARCH_ABI)/libnng.so
LOCAL_EXPORT_C_INCLUDES := ../../base-repo/include
include $(PREBUILT_SHARED_LIBRARY)

# JNIラッパーライブラリの作成
include $(CLEAR_VARS)
LOCAL_MODULE := nng_jni
LOCAL_SRC_FILES := com_nng_android_NngWrapper.cpp
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../base-repo/include
LOCAL_SHARED_LIBRARIES := nng
LOCAL_LDLIBS := -llog -landroid
include $(BUILD_SHARED_LIBRARY)

