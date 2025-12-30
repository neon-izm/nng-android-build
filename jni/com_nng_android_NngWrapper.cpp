/**
 * NNG JNI Wrapper for Android
 *
 * This file implements the JNI bindings for the NNG messaging library.
 * It is designed to work with the auto-generated header from javac -h.
 *
 * Build: ndk-build with Android.mk
 * Dependencies: libnng.so
 */

#include "com_nng_android_NngWrapper.h"

#include <nng/nng.h>
#include <nng/protocol/pair0/pair.h>
#include <nng/protocol/pair1/pair.h>
#include <nng/protocol/reqrep0/req.h>
#include <nng/protocol/reqrep0/rep.h>
#include <nng/protocol/pubsub0/pub.h>
#include <nng/protocol/pubsub0/sub.h>
#include <nng/protocol/pipeline0/push.h>
#include <nng/protocol/pipeline0/pull.h>
#include <nng/protocol/bus0/bus.h>
#include <android/log.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>

#define LOG_TAG "NNG-JNI"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// ============================================================
// Helper: Convert socket ID to nng_socket structure
// ============================================================
static inline nng_socket id_to_socket(jlong id) {
    nng_socket s;
    s.id = (uint32_t)id;
    return s;
}

// ============================================================
// Socket Operations
// ============================================================

JNIEXPORT jlong JNICALL Java_com_nng_android_NngWrapper_nngPair0Open
  (JNIEnv* env, jobject obj) {
    nng_socket socket;
    int rv = nng_pair0_open(&socket);
    if (rv != 0) {
        LOGE("nng_pair0_open failed: %s", nng_strerror(rv));
        return 0;
    }
    LOGI("Pair0 socket opened: %u", socket.id);
    return (jlong)socket.id;
}

JNIEXPORT jlong JNICALL Java_com_nng_android_NngWrapper_nngPair1Open
  (JNIEnv* env, jobject obj) {
    nng_socket socket;
    int rv = nng_pair1_open(&socket);
    if (rv != 0) {
        LOGE("nng_pair1_open failed: %s", nng_strerror(rv));
        return 0;
    }
    LOGI("Pair1 socket opened: %u", socket.id);
    return (jlong)socket.id;
}

JNIEXPORT jlong JNICALL Java_com_nng_android_NngWrapper_nngReq0Open
  (JNIEnv* env, jobject obj) {
    nng_socket socket;
    int rv = nng_req0_open(&socket);
    if (rv != 0) {
        LOGE("nng_req0_open failed: %s", nng_strerror(rv));
        return 0;
    }
    LOGI("Req0 socket opened: %u", socket.id);
    return (jlong)socket.id;
}

JNIEXPORT jlong JNICALL Java_com_nng_android_NngWrapper_nngRep0Open
  (JNIEnv* env, jobject obj) {
    nng_socket socket;
    int rv = nng_rep0_open(&socket);
    if (rv != 0) {
        LOGE("nng_rep0_open failed: %s", nng_strerror(rv));
        return 0;
    }
    LOGI("Rep0 socket opened: %u", socket.id);
    return (jlong)socket.id;
}

JNIEXPORT jlong JNICALL Java_com_nng_android_NngWrapper_nngPub0Open
  (JNIEnv* env, jobject obj) {
    nng_socket socket;
    int rv = nng_pub0_open(&socket);
    if (rv != 0) {
        LOGE("nng_pub0_open failed: %s", nng_strerror(rv));
        return 0;
    }
    LOGI("Pub0 socket opened: %u", socket.id);
    return (jlong)socket.id;
}

JNIEXPORT jlong JNICALL Java_com_nng_android_NngWrapper_nngSub0Open
  (JNIEnv* env, jobject obj) {
    nng_socket socket;
    int rv = nng_sub0_open(&socket);
    if (rv != 0) {
        LOGE("nng_sub0_open failed: %s", nng_strerror(rv));
        return 0;
    }
    LOGI("Sub0 socket opened: %u", socket.id);
    return (jlong)socket.id;
}

JNIEXPORT jlong JNICALL Java_com_nng_android_NngWrapper_nngPush0Open
  (JNIEnv* env, jobject obj) {
    nng_socket socket;
    int rv = nng_push0_open(&socket);
    if (rv != 0) {
        LOGE("nng_push0_open failed: %s", nng_strerror(rv));
        return 0;
    }
    LOGI("Push0 socket opened: %u", socket.id);
    return (jlong)socket.id;
}

JNIEXPORT jlong JNICALL Java_com_nng_android_NngWrapper_nngPull0Open
  (JNIEnv* env, jobject obj) {
    nng_socket socket;
    int rv = nng_pull0_open(&socket);
    if (rv != 0) {
        LOGE("nng_pull0_open failed: %s", nng_strerror(rv));
        return 0;
    }
    LOGI("Pull0 socket opened: %u", socket.id);
    return (jlong)socket.id;
}

JNIEXPORT jlong JNICALL Java_com_nng_android_NngWrapper_nngBus0Open
  (JNIEnv* env, jobject obj) {
    nng_socket socket;
    int rv = nng_bus0_open(&socket);
    if (rv != 0) {
        LOGE("nng_bus0_open failed: %s", nng_strerror(rv));
        return 0;
    }
    LOGI("Bus0 socket opened: %u", socket.id);
    return (jlong)socket.id;
}

JNIEXPORT jint JNICALL Java_com_nng_android_NngWrapper_nngClose
  (JNIEnv* env, jobject obj, jlong handle) {
    nng_socket socket = id_to_socket(handle);
    int rv = nng_close(socket);
    if (rv == 0) {
        LOGI("Socket closed: %u", socket.id);
    } else {
        LOGE("nng_close failed: %s", nng_strerror(rv));
    }
    return rv;
}

// ============================================================
// Communication
// ============================================================

JNIEXPORT jint JNICALL Java_com_nng_android_NngWrapper_nngListen
  (JNIEnv* env, jobject obj, jlong handle, jstring url) {
    nng_socket socket = id_to_socket(handle);
    const char* url_str = env->GetStringUTFChars(url, nullptr);

    int rv = nng_listen(socket, url_str, NULL, 0);
    if (rv == 0) {
        LOGI("Listening on: %s", url_str);
    } else {
        LOGE("nng_listen failed on %s: %s", url_str, nng_strerror(rv));
    }

    env->ReleaseStringUTFChars(url, url_str);
    return rv;
}

JNIEXPORT jint JNICALL Java_com_nng_android_NngWrapper_nngDial
  (JNIEnv* env, jobject obj, jlong handle, jstring url) {
    nng_socket socket = id_to_socket(handle);
    const char* url_str = env->GetStringUTFChars(url, nullptr);

    int rv = nng_dial(socket, url_str, NULL, 0);
    if (rv == 0) {
        LOGI("Dialed: %s", url_str);
    } else {
        LOGE("nng_dial failed on %s: %s", url_str, nng_strerror(rv));
    }

    env->ReleaseStringUTFChars(url, url_str);
    return rv;
}

JNIEXPORT jint JNICALL Java_com_nng_android_NngWrapper_nngSend
  (JNIEnv* env, jobject obj, jlong handle, jbyteArray data, jint size, jint flags) {
    nng_socket socket = id_to_socket(handle);

    // Android推奨: GetByteArrayRegionを使用してメモリ効率を向上
    jbyte* buffer = (jbyte*)malloc(size);
    if (buffer == nullptr) {
        LOGE("nng_send: Failed to allocate buffer");
        return NNG_ENOMEM;
    }

    env->GetByteArrayRegion(data, 0, size, buffer);
    int rv = nng_send(socket, buffer, (size_t)size, flags);
    free(buffer);

    if (rv == 0) {
        LOGI("Sent %d bytes (flags=%d)", size, flags);
    } else if (rv != NNG_EAGAIN) {
        LOGE("nng_send failed: %s", nng_strerror(rv));
    }

    return rv;
}

JNIEXPORT jint JNICALL Java_com_nng_android_NngWrapper_nngRecv
  (JNIEnv* env, jobject obj, jlong handle, jbyteArray buffer, jint bufferSize, jint flags) {
    nng_socket socket = id_to_socket(handle);

    // Android推奨: 一時バッファを使用してSetByteArrayRegionで効率的にコピー
    jbyte* tempBuffer = (jbyte*)malloc(bufferSize);
    if (tempBuffer == nullptr) {
        LOGE("nng_recv: Failed to allocate temp buffer");
        return -NNG_ENOMEM;
    }

    size_t received = (size_t)bufferSize;
    int rv = nng_recv(socket, tempBuffer, &received, flags);

    if (rv == 0) {
        // Android推奨: SetByteArrayRegionを使用してJava配列に効率的にコピー
        env->SetByteArrayRegion(buffer, 0, (jsize)received, tempBuffer);
        free(tempBuffer);
        LOGI("Received %zu bytes (flags=%d)", received, flags);
        return (jint)received;
    } else {
        free(tempBuffer);
        if (rv != NNG_EAGAIN && rv != NNG_ETIMEDOUT) {
            LOGE("nng_recv failed: %s", nng_strerror(rv));
        }
        return -rv;  // Return negative error code
    }
}

// ============================================================
// Options
// ============================================================

JNIEXPORT jint JNICALL Java_com_nng_android_NngWrapper_nngSetRecvTimeout
  (JNIEnv* env, jobject obj, jlong handle, jint ms) {
    nng_socket socket = id_to_socket(handle);
    int rv = nng_socket_set_ms(socket, NNG_OPT_RECVTIMEO, (nng_duration)ms);
    if (rv != 0) {
        LOGE("nng_socket_set_ms(RECVTIMEO) failed: %s", nng_strerror(rv));
    }
    return rv;
}

JNIEXPORT jint JNICALL Java_com_nng_android_NngWrapper_nngSetSendTimeout
  (JNIEnv* env, jobject obj, jlong handle, jint ms) {
    nng_socket socket = id_to_socket(handle);
    int rv = nng_socket_set_ms(socket, NNG_OPT_SENDTIMEO, (nng_duration)ms);
    if (rv != 0) {
        LOGE("nng_socket_set_ms(SENDTIMEO) failed: %s", nng_strerror(rv));
    }
    return rv;
}

JNIEXPORT jint JNICALL Java_com_nng_android_NngWrapper_nngGetRecvTimeout
  (JNIEnv* env, jobject obj, jlong handle) {
    nng_socket socket = id_to_socket(handle);
    nng_duration ms;
    int rv = nng_socket_get_ms(socket, NNG_OPT_RECVTIMEO, &ms);
    if (rv != 0) {
        LOGE("nng_socket_get_ms(RECVTIMEO) failed: %s", nng_strerror(rv));
        return -rv;
    }
    return (jint)ms;
}

JNIEXPORT jint JNICALL Java_com_nng_android_NngWrapper_nngGetSendTimeout
  (JNIEnv* env, jobject obj, jlong handle) {
    nng_socket socket = id_to_socket(handle);
    nng_duration ms;
    int rv = nng_socket_get_ms(socket, NNG_OPT_SENDTIMEO, &ms);
    if (rv != 0) {
        LOGE("nng_socket_get_ms(SENDTIMEO) failed: %s", nng_strerror(rv));
        return -rv;
    }
    return (jint)ms;
}

// ============================================================
// Subscriber Options
// ============================================================

JNIEXPORT jint JNICALL Java_com_nng_android_NngWrapper_nngSubSubscribe
  (JNIEnv* env, jobject obj, jlong handle, jbyteArray topic) {
    nng_socket socket = id_to_socket(handle);

    jsize len = env->GetArrayLength(topic);
    jbyte* bytes = env->GetByteArrayElements(topic, nullptr);

    int rv = nng_socket_set(socket, NNG_OPT_SUB_SUBSCRIBE, bytes, (size_t)len);
    env->ReleaseByteArrayElements(topic, bytes, JNI_ABORT);

    if (rv != 0) {
        LOGE("nng_socket_set(SUB_SUBSCRIBE) failed: %s", nng_strerror(rv));
    } else {
        LOGI("Subscribed to topic (len=%d)", len);
    }
    return rv;
}

JNIEXPORT jint JNICALL Java_com_nng_android_NngWrapper_nngSubUnsubscribe
  (JNIEnv* env, jobject obj, jlong handle, jbyteArray topic) {
    nng_socket socket = id_to_socket(handle);

    jsize len = env->GetArrayLength(topic);
    jbyte* bytes = env->GetByteArrayElements(topic, nullptr);

    int rv = nng_socket_set(socket, NNG_OPT_SUB_UNSUBSCRIBE, bytes, (size_t)len);
    env->ReleaseByteArrayElements(topic, bytes, JNI_ABORT);

    if (rv != 0) {
        LOGE("nng_socket_set(SUB_UNSUBSCRIBE) failed: %s", nng_strerror(rv));
    } else {
        LOGI("Unsubscribed from topic (len=%d)", len);
    }
    return rv;
}

// ============================================================
// Utilities
// ============================================================

JNIEXPORT jint JNICALL Java_com_nng_android_NngWrapper_nngSocketId
  (JNIEnv* env, jobject obj, jlong handle) {
    nng_socket socket = id_to_socket(handle);
    return nng_socket_id(socket);
}

JNIEXPORT jstring JNICALL Java_com_nng_android_NngWrapper_nngStrerror
  (JNIEnv* env, jobject obj, jint error) {
    const char* error_str = nng_strerror(error);
    return env->NewStringUTF(error_str);
}

JNIEXPORT jstring JNICALL Java_com_nng_android_NngWrapper_nngVersion
  (JNIEnv* env, jobject obj) {
    char version[32];
    snprintf(version, sizeof(version), "%d.%d.%d",
             NNG_MAJOR_VERSION, NNG_MINOR_VERSION, NNG_PATCH_VERSION);
    return env->NewStringUTF(version);
}

