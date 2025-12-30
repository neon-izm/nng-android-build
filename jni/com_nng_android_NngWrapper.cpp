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
// Survey protocol headers - check if available in NNG 1.10
// #include <nng/protocol/survey0/survey.h>
// #include <nng/protocol/survey0/respond.h>
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

// ============================================================
// Survey Protocol Operations
// ============================================================

JNIEXPORT jlong JNICALL Java_com_nng_android_NngWrapper_nngSurveyor0Open
  (JNIEnv* env, jobject obj) {
#ifdef NNG_HAVE_SURVEYOR0
    nng_socket socket;
    int rv = nng_surveyor0_open(&socket);
    if (rv != 0) {
        LOGE("nng_surveyor0_open failed: %s", nng_strerror(rv));
        return -rv;
    }
    LOGI("nng_surveyor0_open successful, socket ID: %u", socket.id);
    return (jlong)socket.id;
#else
    LOGE("Survey protocol not supported in this NNG version");
    return -NNG_ENOTSUP;
#endif
}

JNIEXPORT jlong JNICALL Java_com_nng_android_NngWrapper_nngRespondent0Open
  (JNIEnv* env, jobject obj) {
#ifdef NNG_HAVE_RESPONDENT0
    nng_socket socket;
    int rv = nng_respondent0_open(&socket);
    if (rv != 0) {
        LOGE("nng_respondent0_open failed: %s", nng_strerror(rv));
        return -rv;
    }
    LOGI("nng_respondent0_open successful, socket ID: %u", socket.id);
    return (jlong)socket.id;
#else
    LOGE("Survey protocol not supported in this NNG version");
    return -NNG_ENOTSUP;
#endif
}

// ============================================================
// Asynchronous I/O Operations
// ============================================================

JNIEXPORT jlong JNICALL Java_com_nng_android_NngWrapper_nngAioAlloc
  (JNIEnv* env, jobject obj) {
    nng_aio* aio;
    int rv = nng_aio_alloc(&aio, NULL, NULL);
    if (rv != 0) {
        LOGE("nng_aio_alloc failed: %s", nng_strerror(rv));
        return -rv;
    }
    LOGI("nng_aio_alloc successful, aio: %p", aio);
    return (jlong)(uintptr_t)aio;
}

JNIEXPORT void JNICALL Java_com_nng_android_NngWrapper_nngAioFree
  (JNIEnv* env, jobject obj, jlong aio_ptr) {
    nng_aio* aio = (nng_aio*)(uintptr_t)aio_ptr;
    if (aio != NULL) {
        nng_aio_free(aio);
        LOGI("nng_aio_free completed");
    }
}

JNIEXPORT void JNICALL Java_com_nng_android_NngWrapper_nngSocketSendAio
  (JNIEnv* env, jobject obj, jlong socket_id, jlong aio_ptr) {
    nng_socket socket = id_to_socket(socket_id);
    nng_aio* aio = (nng_aio*)(uintptr_t)aio_ptr;
    
    if (aio == NULL) {
        LOGE("Invalid AIO pointer");
        return;
    }
    
    nng_socket_send(socket, aio);
    LOGI("nng_socket_send_aio initiated");
}

JNIEXPORT void JNICALL Java_com_nng_android_NngWrapper_nngSocketRecvAio
  (JNIEnv* env, jobject obj, jlong socket_id, jlong aio_ptr) {
    nng_socket socket = id_to_socket(socket_id);
    nng_aio* aio = (nng_aio*)(uintptr_t)aio_ptr;
    
    if (aio == NULL) {
        LOGE("Invalid AIO pointer");
        return;
    }
    
    nng_socket_recv(socket, aio);
    LOGI("nng_socket_recv_aio initiated");
}

JNIEXPORT jint JNICALL Java_com_nng_android_NngWrapper_nngAioResult
  (JNIEnv* env, jobject obj, jlong aio_ptr) {
    nng_aio* aio = (nng_aio*)(uintptr_t)aio_ptr;
    
    if (aio == NULL) {
        LOGE("Invalid AIO pointer");
        return NNG_EINVAL;
    }
    
    int result = nng_aio_result(aio);
    LOGI("nng_aio_result: %d", result);
    return result;
}

JNIEXPORT void JNICALL Java_com_nng_android_NngWrapper_nngAioWait
  (JNIEnv* env, jobject obj, jlong aio_ptr) {
    nng_aio* aio = (nng_aio*)(uintptr_t)aio_ptr;
    
    if (aio == NULL) {
        LOGE("Invalid AIO pointer");
        return;
    }
    
    nng_aio_wait(aio);
    LOGI("nng_aio_wait completed");
}

// ============================================================
// Message Operations
// ============================================================

JNIEXPORT jlong JNICALL Java_com_nng_android_NngWrapper_nngMsgAlloc
  (JNIEnv* env, jobject obj, jint size) {
    nng_msg* msg;
    int rv = nng_msg_alloc(&msg, (size_t)size);
    if (rv != 0) {
        LOGE("nng_msg_alloc failed: %s", nng_strerror(rv));
        return -rv;
    }
    LOGI("nng_msg_alloc successful, msg: %p, size: %d", msg, size);
    return (jlong)(uintptr_t)msg;
}

JNIEXPORT void JNICALL Java_com_nng_android_NngWrapper_nngMsgFree
  (JNIEnv* env, jobject obj, jlong msg_ptr) {
    nng_msg* msg = (nng_msg*)(uintptr_t)msg_ptr;
    if (msg != NULL) {
        nng_msg_free(msg);
        LOGI("nng_msg_free completed");
    }
}

JNIEXPORT jbyteArray JNICALL Java_com_nng_android_NngWrapper_nngMsgBody
  (JNIEnv* env, jobject obj, jlong msg_ptr) {
    nng_msg* msg = (nng_msg*)(uintptr_t)msg_ptr;
    
    if (msg == NULL) {
        LOGE("Invalid message pointer");
        return NULL;
    }
    
    void* body = nng_msg_body(msg);
    size_t len = nng_msg_len(msg);
    
    if (body == NULL || len == 0) {
        return env->NewByteArray(0);
    }
    
    jbyteArray result = env->NewByteArray((jsize)len);
    if (result != NULL) {
        env->SetByteArrayRegion(result, 0, (jsize)len, (const jbyte*)body);
    }
    
    LOGI("nng_msg_body returned %zu bytes", len);
    return result;
}

JNIEXPORT jint JNICALL Java_com_nng_android_NngWrapper_nngMsgLen
  (JNIEnv* env, jobject obj, jlong msg_ptr) {
    nng_msg* msg = (nng_msg*)(uintptr_t)msg_ptr;
    
    if (msg == NULL) {
        LOGE("Invalid message pointer");
        return 0;
    }
    
    size_t len = nng_msg_len(msg);
    LOGI("nng_msg_len: %zu", len);
    return (jint)len;
}

JNIEXPORT jint JNICALL Java_com_nng_android_NngWrapper_nngSendMsg
  (JNIEnv* env, jobject obj, jlong socket_id, jlong msg_ptr, jint flags) {
    nng_socket socket = id_to_socket(socket_id);
    nng_msg* msg = (nng_msg*)(uintptr_t)msg_ptr;
    
    if (msg == NULL) {
        LOGE("Invalid message pointer");
        return NNG_EINVAL;
    }
    
    int rv = nng_sendmsg(socket, msg, flags);
    if (rv != 0) {
        LOGE("nng_sendmsg failed: %s", nng_strerror(rv));
    } else {
        LOGI("nng_sendmsg successful");
    }
    
    return rv;
}

JNIEXPORT jlong JNICALL Java_com_nng_android_NngWrapper_nngRecvMsg
  (JNIEnv* env, jobject obj, jlong socket_id, jint flags) {
    nng_socket socket = id_to_socket(socket_id);
    nng_msg* msg;
    
    int rv = nng_recvmsg(socket, &msg, flags);
    if (rv != 0) {
        LOGE("nng_recvmsg failed: %s", nng_strerror(rv));
        return -rv;
    }
    
    LOGI("nng_recvmsg successful, msg: %p", msg);
    return (jlong)(uintptr_t)msg;
}

// ============================================================
// URL Operations
// ============================================================

JNIEXPORT jlong JNICALL Java_com_nng_android_NngWrapper_nngUrlParse
  (JNIEnv* env, jobject obj, jstring url_str) {
    if (url_str == NULL) {
        LOGE("URL string is null");
        return -NNG_EINVAL;
    }
    
    const char* url_cstr = env->GetStringUTFChars(url_str, NULL);
    if (url_cstr == NULL) {
        LOGE("Failed to get URL string");
        return -NNG_ENOMEM;
    }
    
    nng_url* url;
    int rv = nng_url_parse(&url, url_cstr);
    
    env->ReleaseStringUTFChars(url_str, url_cstr);
    
    if (rv != 0) {
        LOGE("nng_url_parse failed: %s", nng_strerror(rv));
        return -rv;
    }
    
    LOGI("nng_url_parse successful, url: %p", url);
    return (jlong)(uintptr_t)url;
}

JNIEXPORT void JNICALL Java_com_nng_android_NngWrapper_nngUrlFree
  (JNIEnv* env, jobject obj, jlong url_ptr) {
    nng_url* url = (nng_url*)(uintptr_t)url_ptr;
    if (url != NULL) {
        nng_url_free(url);
        LOGI("nng_url_free completed");
    }
}

JNIEXPORT jstring JNICALL Java_com_nng_android_NngWrapper_nngUrlScheme
  (JNIEnv* env, jobject obj, jlong url_ptr) {
    nng_url* url = (nng_url*)(uintptr_t)url_ptr;
    
    if (url == NULL || url->u_scheme == NULL) {
        return NULL;
    }
    
    return env->NewStringUTF(url->u_scheme);
}

JNIEXPORT jstring JNICALL Java_com_nng_android_NngWrapper_nngUrlHost
  (JNIEnv* env, jobject obj, jlong url_ptr) {
    nng_url* url = (nng_url*)(uintptr_t)url_ptr;
    
    if (url == NULL || url->u_hostname == NULL) {
        return NULL;
    }
    
    return env->NewStringUTF(url->u_hostname);
}

JNIEXPORT jstring JNICALL Java_com_nng_android_NngWrapper_nngUrlPort
  (JNIEnv* env, jobject obj, jlong url_ptr) {
    nng_url* url = (nng_url*)(uintptr_t)url_ptr;
    
    if (url == NULL || url->u_port == NULL) {
        return NULL;
    }
    
    return env->NewStringUTF(url->u_port);
}

// ============================================================
// Dialer/Listener Operations
// ============================================================

JNIEXPORT jlong JNICALL Java_com_nng_android_NngWrapper_nngDialerCreate
  (JNIEnv* env, jobject obj, jlong socket_id, jstring url_str) {
    nng_socket socket = id_to_socket(socket_id);
    
    if (url_str == NULL) {
        LOGE("URL string is null");
        return -NNG_EINVAL;
    }
    
    const char* url_cstr = env->GetStringUTFChars(url_str, NULL);
    if (url_cstr == NULL) {
        LOGE("Failed to get URL string");
        return -NNG_ENOMEM;
    }
    
    nng_dialer dialer;
    int rv = nng_dialer_create(&dialer, socket, url_cstr);
    
    env->ReleaseStringUTFChars(url_str, url_cstr);
    
    if (rv != 0) {
        LOGE("nng_dialer_create failed: %s", nng_strerror(rv));
        return -rv;
    }
    
    LOGI("nng_dialer_create successful, dialer ID: %u", dialer.id);
    return (jlong)dialer.id;
}

JNIEXPORT jlong JNICALL Java_com_nng_android_NngWrapper_nngListenerCreate
  (JNIEnv* env, jobject obj, jlong socket_id, jstring url_str) {
    nng_socket socket = id_to_socket(socket_id);
    
    if (url_str == NULL) {
        LOGE("URL string is null");
        return -NNG_EINVAL;
    }
    
    const char* url_cstr = env->GetStringUTFChars(url_str, NULL);
    if (url_cstr == NULL) {
        LOGE("Failed to get URL string");
        return -NNG_ENOMEM;
    }
    
    nng_listener listener;
    int rv = nng_listener_create(&listener, socket, url_cstr);
    
    env->ReleaseStringUTFChars(url_str, url_cstr);
    
    if (rv != 0) {
        LOGE("nng_listener_create failed: %s", nng_strerror(rv));
        return -rv;
    }
    
    LOGI("nng_listener_create successful, listener ID: %u", listener.id);
    return (jlong)listener.id;
}

JNIEXPORT jint JNICALL Java_com_nng_android_NngWrapper_nngDialerStart
  (JNIEnv* env, jobject obj, jlong dialer_id, jint flags) {
    nng_dialer dialer;
    dialer.id = (uint32_t)dialer_id;
    
    int rv = nng_dialer_start(dialer, flags);
    if (rv != 0) {
        LOGE("nng_dialer_start failed: %s", nng_strerror(rv));
    } else {
        LOGI("nng_dialer_start successful");
    }
    
    return rv;
}

JNIEXPORT jint JNICALL Java_com_nng_android_NngWrapper_nngListenerStart
  (JNIEnv* env, jobject obj, jlong listener_id, jint flags) {
    nng_listener listener;
    listener.id = (uint32_t)listener_id;
    
    int rv = nng_listener_start(listener, flags);
    if (rv != 0) {
        LOGE("nng_listener_start failed: %s", nng_strerror(rv));
    } else {
        LOGI("nng_listener_start successful");
    }
    
    return rv;
}

JNIEXPORT jint JNICALL Java_com_nng_android_NngWrapper_nngDialerClose
  (JNIEnv* env, jobject obj, jlong dialer_id) {
    nng_dialer dialer;
    dialer.id = (uint32_t)dialer_id;
    
    int rv = nng_dialer_close(dialer);
    if (rv != 0) {
        LOGE("nng_dialer_close failed: %s", nng_strerror(rv));
    } else {
        LOGI("nng_dialer_close successful");
    }
    
    return rv;
}

JNIEXPORT jint JNICALL Java_com_nng_android_NngWrapper_nngListenerClose
  (JNIEnv* env, jobject obj, jlong listener_id) {
    nng_listener listener;
    listener.id = (uint32_t)listener_id;
    
    int rv = nng_listener_close(listener);
    if (rv != 0) {
        LOGE("nng_listener_close failed: %s", nng_strerror(rv));
    } else {
        LOGI("nng_listener_close successful");
    }
    
    return rv;
}

// ============================================================
// Statistics Operations
// ============================================================

JNIEXPORT jlong JNICALL Java_com_nng_android_NngWrapper_nngStatsGet
  (JNIEnv* env, jobject obj) {
    nng_stat* stats;
    int rv = nng_stats_get(&stats);
    if (rv != 0) {
        LOGE("nng_stats_get failed: %s", nng_strerror(rv));
        return -rv;
    }
    
    LOGI("nng_stats_get successful, stats: %p", stats);
    return (jlong)(uintptr_t)stats;
}

JNIEXPORT void JNICALL Java_com_nng_android_NngWrapper_nngStatsFree
  (JNIEnv* env, jobject obj, jlong stats_ptr) {
    nng_stat* stats = (nng_stat*)(uintptr_t)stats_ptr;
    if (stats != NULL) {
        nng_stats_free(stats);
        LOGI("nng_stats_free completed");
    }
}

JNIEXPORT jstring JNICALL Java_com_nng_android_NngWrapper_nngStatName
  (JNIEnv* env, jobject obj, jlong stat_ptr) {
    nng_stat* stat = (nng_stat*)(uintptr_t)stat_ptr;
    
    if (stat == NULL) {
        return NULL;
    }
    
    const char* name = nng_stat_name(stat);
    if (name == NULL) {
        return NULL;
    }
    
    return env->NewStringUTF(name);
}

JNIEXPORT jlong JNICALL Java_com_nng_android_NngWrapper_nngStatValue
  (JNIEnv* env, jobject obj, jlong stat_ptr) {
    nng_stat* stat = (nng_stat*)(uintptr_t)stat_ptr;
    
    if (stat == NULL) {
        return 0;
    }
    
    uint64_t value = nng_stat_value(stat);
    return (jlong)value;
}

JNIEXPORT jlong JNICALL Java_com_nng_android_NngWrapper_nngStatNext
  (JNIEnv* env, jobject obj, jlong stat_ptr) {
    nng_stat* stat = (nng_stat*)(uintptr_t)stat_ptr;
    
    if (stat == NULL) {
        return 0;
    }
    
    nng_stat* next = nng_stat_next(stat);
    return (jlong)(uintptr_t)next;
}

