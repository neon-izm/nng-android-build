package com.nng.android;

import android.util.Log;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

/**
 * NNG (nanomsg-next-generation) JNI Wrapper for Android
 *
 * This class provides Java bindings for the NNG messaging library.
 * It supports multiple protocols and various socket options.
 *
 * <p>Usage example:</p>
 * <pre>{@code
 * NngWrapper nng = new NngWrapper();
 * long socket = nng.nngPair0Open();
 * nng.nngListen(socket, "inproc://test");
 * // ... use socket ...
 * nng.nngClose(socket);
 * }</pre>
 *
 * <p>Or use the high-level API:</p>
 * <pre>{@code
 * NngWrapper nng = new NngWrapper();
 * nng.openPair0();
 * nng.listen("tcp://127.0.0.1:5555");
 * nng.sendString("Hello World");
 * String response = nng.receiveString();
 * nng.close();
 * }</pre>
 *
 * @version 1.11.0
 * @since 1.0.0
 */
public class NngWrapper {
    private static final String TAG = "NngWrapper";

    static {
        try {
            System.loadLibrary("nng");
            System.loadLibrary("nng_jni");
            Log.i(TAG, "NNG libraries loaded successfully");
        } catch (UnsatisfiedLinkError e) {
            Log.e(TAG, "Failed to load NNG libraries: " + e.getMessage());
            throw e;
        }
    }

    // ========== Socket Operations ==========

    /**
     * Open a Pair0 socket (1-to-1 bidirectional)
     * @return socket handle (positive value on success, negative error code on failure)
     */
    public native long nngPair0Open();

    /**
     * Open a Pair1 socket (polyamorous mode supported)
     * @return socket handle (positive value on success, negative error code on failure)
     */
    public native long nngPair1Open();

    /**
     * Open a Req0 socket (request side of request/reply)
     * @return socket handle (positive value on success, negative error code on failure)
     */
    public native long nngReq0Open();

    /**
     * Open a Rep0 socket (reply side of request/reply)
     * @return socket handle (positive value on success, negative error code on failure)
     */
    public native long nngRep0Open();

    /**
     * Open a Pub0 socket (publisher side of pub/sub)
     * @return socket handle (positive value on success, negative error code on failure)
     */
    public native long nngPub0Open();

    /**
     * Open a Sub0 socket (subscriber side of pub/sub)
     * @return socket handle (positive value on success, negative error code on failure)
     */
    public native long nngSub0Open();

    /**
     * Open a Push0 socket (push side of pipeline)
     * @return socket handle (positive value on success, negative error code on failure)
     */
    public native long nngPush0Open();

    /**
     * Open a Pull0 socket (pull side of pipeline)
     * @return socket handle (positive value on success, negative error code on failure)
     */
    public native long nngPull0Open();

    /**
     * Open a Bus0 socket (many-to-many communication)
     * @return socket handle (positive value on success, negative error code on failure)
     */
    public native long nngBus0Open();

    /**
     * Open a Surveyor0 socket (survey side of survey/respond)
     * @return socket handle (positive value on success, negative error code on failure)
     */
    public native long nngSurveyor0Open();

    /**
     * Open a Respondent0 socket (respond side of survey/respond)
     * @return socket handle (positive value on success, negative error code on failure)
     */
    public native long nngRespondent0Open();

    /**
     * Close a socket
     * @param socket socket handle
     * @return 0 on success, error code on failure
     */
    public native int nngClose(long socket);

    // ========== Communication ==========

    /**
     * Start listening on the specified URL
     * @param socket socket handle
     * @param url URL to listen on (e.g., "inproc://test", "tcp://127.0.0.1:5555")
     * @return 0 on success, error code on failure
     */
    public native int nngListen(long socket, @NonNull String url);

    /**
     * Connect to the specified URL
     * @param socket socket handle
     * @param url URL to connect to
     * @return 0 on success, error code on failure
     */
    public native int nngDial(long socket, @NonNull String url);

    /**
     * Send data with flags
     * @param socket socket handle
     * @param data data to send
     * @param size size of data
     * @param flags send flags (NNG_FLAG_NONBLOCK, etc.)
     * @return 0 on success, error code on failure
     */
    public native int nngSend(long socket, @NonNull byte[] data, int size, int flags);

    /**
     * Receive data with flags
     * @param socket socket handle
     * @param buffer buffer to receive into
     * @param bufferSize size of buffer
     * @param flags receive flags (NNG_FLAG_NONBLOCK, etc.)
     * @return number of bytes received on success, negative error code on failure
     */
    public native int nngRecv(long socket, @NonNull byte[] buffer, int bufferSize, int flags);

    // ========== Socket Options ==========

    /**
     * Set receive timeout
     * @param socket socket handle
     * @param ms timeout in milliseconds (use NNG_DURATION_* constants)
     * @return 0 on success, error code on failure
     */
    public native int nngSetRecvTimeout(long socket, int ms);

    /**
     * Set send timeout
     * @param socket socket handle
     * @param ms timeout in milliseconds
     * @return 0 on success, error code on failure
     */
    public native int nngSetSendTimeout(long socket, int ms);

    /**
     * Get receive timeout
     * @param socket socket handle
     * @return timeout in milliseconds, or negative error code
     */
    public native int nngGetRecvTimeout(long socket);

    /**
     * Get send timeout
     * @param socket socket handle
     * @return timeout in milliseconds, or negative error code
     */
    public native int nngGetSendTimeout(long socket);

    // ========== Subscriber Specific ==========

    /**
     * Subscribe to a topic (Sub0 socket only)
     * @param socket socket handle
     * @param topic topic to subscribe (empty array for all messages)
     * @return 0 on success, error code on failure
     */
    public native int nngSubSubscribe(long socket, @NonNull byte[] topic);

    /**
     * Unsubscribe from a topic (Sub0 socket only)
     * @param socket socket handle
     * @param topic topic to unsubscribe
     * @return 0 on success, error code on failure
     */
    public native int nngSubUnsubscribe(long socket, @NonNull byte[] topic);

    // ========== Advanced Features ==========

    /**
     * Allocate an AIO object for asynchronous operations
     * @return AIO handle (positive value on success, negative error code on failure)
     */
    public native long nngAioAlloc();

    /**
     * Free an AIO object
     * @param aio AIO handle
     */
    public native void nngAioFree(long aio);

    /**
     * Start asynchronous send operation
     * @param socket socket handle
     * @param aio AIO handle
     */
    public native void nngSocketSendAio(long socket, long aio);

    /**
     * Start asynchronous receive operation
     * @param socket socket handle
     * @param aio AIO handle
     */
    public native void nngSocketRecvAio(long socket, long aio);

    /**
     * Get result of asynchronous operation
     * @param aio AIO handle
     * @return 0 on success, error code on failure
     */
    public native int nngAioResult(long aio);

    /**
     * Wait for asynchronous operation to complete
     * @param aio AIO handle
     */
    public native void nngAioWait(long aio);

    /**
     * Allocate a message object
     * @param size initial size of message
     * @return message handle (positive value on success, negative error code on failure)
     */
    public native long nngMsgAlloc(int size);

    /**
     * Free a message object
     * @param msg message handle
     */
    public native void nngMsgFree(long msg);

    /**
     * Get message body as byte array
     * @param msg message handle
     * @return message body, or null on error
     */
    @Nullable
    public native byte[] nngMsgBody(long msg);

    /**
     * Get message length
     * @param msg message handle
     * @return message length, or 0 on error
     */
    public native int nngMsgLen(long msg);

    /**
     * Send a message object
     * @param socket socket handle
     * @param msg message handle
     * @param flags send flags
     * @return 0 on success, error code on failure
     */
    public native int nngSendMsg(long socket, long msg, int flags);

    /**
     * Receive a message object
     * @param socket socket handle
     * @param flags receive flags
     * @return message handle (positive value on success, negative error code on failure)
     */
    public native long nngRecvMsg(long socket, int flags);

    // ========== URL Operations ==========

    /**
     * Parse a URL string
     * @param url URL string to parse
     * @return URL handle (positive value on success, negative error code on failure)
     */
    public native long nngUrlParse(@NonNull String url);

    /**
     * Free a URL object
     * @param url URL handle
     */
    public native void nngUrlFree(long url);

    /**
     * Get URL scheme
     * @param url URL handle
     * @return scheme string, or null on error
     */
    @Nullable
    public native String nngUrlScheme(long url);

    /**
     * Get URL host
     * @param url URL handle
     * @return host string, or null on error
     */
    @Nullable
    public native String nngUrlHost(long url);

    /**
     * Get URL port
     * @param url URL handle
     * @return port string, or null on error
     */
    @Nullable
    public native String nngUrlPort(long url);

    // ========== Dialer/Listener Operations ==========

    /**
     * Create a dialer
     * @param socket socket handle
     * @param url URL to dial
     * @return dialer handle (positive value on success, negative error code on failure)
     */
    public native long nngDialerCreate(long socket, @NonNull String url);

    /**
     * Create a listener
     * @param socket socket handle
     * @param url URL to listen on
     * @return listener handle (positive value on success, negative error code on failure)
     */
    public native long nngListenerCreate(long socket, @NonNull String url);

    /**
     * Start a dialer
     * @param dialer dialer handle
     * @param flags start flags
     * @return 0 on success, error code on failure
     */
    public native int nngDialerStart(long dialer, int flags);

    /**
     * Start a listener
     * @param listener listener handle
     * @param flags start flags
     * @return 0 on success, error code on failure
     */
    public native int nngListenerStart(long listener, int flags);

    /**
     * Close a dialer
     * @param dialer dialer handle
     * @return 0 on success, error code on failure
     */
    public native int nngDialerClose(long dialer);

    /**
     * Close a listener
     * @param listener listener handle
     * @return 0 on success, error code on failure
     */
    public native int nngListenerClose(long listener);

    // ========== Statistics ==========

    /**
     * Get statistics
     * @return statistics handle (positive value on success, negative error code on failure)
     */
    public native long nngStatsGet();

    /**
     * Free statistics
     * @param stats statistics handle
     */
    public native void nngStatsFree(long stats);

    /**
     * Get statistic name
     * @param stat statistic handle
     * @return statistic name, or null on error
     */
    @Nullable
    public native String nngStatName(long stat);

    /**
     * Get statistic value
     * @param stat statistic handle
     * @return statistic value
     */
    public native long nngStatValue(long stat);

    /**
     * Get next statistic
     * @param stat current statistic handle
     * @return next statistic handle, or 0 if no more
     */
    public native long nngStatNext(long stat);

    // ========== Utilities ==========

    /**
     * Get the socket ID
     * @param socket socket handle
     * @return socket ID, or negative value if invalid
     */
    public native int nngSocketId(long socket);

    /**
     * Convert error code to human-readable string
     * @param error error code
     * @return error message
     */
    @NonNull
    public native String nngStrerror(int error);

    /**
     * Get NNG library version string
     * @return version string (e.g., "1.11.0")
     */
    @NonNull
    public native String nngVersion();

    // ========== Constants: Flags ==========

    /** Allocate memory for received data */
    public static final int NNG_FLAG_ALLOC = 1;

    /** Non-blocking operation */
    public static final int NNG_FLAG_NONBLOCK = 2;

    // ========== Constants: Timeouts ==========

    /** Wait indefinitely */
    public static final int NNG_DURATION_INFINITE = -1;

    /** Use default timeout */
    public static final int NNG_DURATION_DEFAULT = -2;

    /** Do not wait (poll) */
    public static final int NNG_DURATION_ZERO = 0;

    // ========== Constants: Error Codes ==========

    /** Success */
    public static final int NNG_OK = 0;

    /** Interrupted */
    public static final int NNG_EINTR = 1;

    /** Out of memory */
    public static final int NNG_ENOMEM = 2;

    /** Invalid argument */
    public static final int NNG_EINVAL = 3;

    /** Resource busy */
    public static final int NNG_EBUSY = 4;

    /** Timed out */
    public static final int NNG_ETIMEDOUT = 5;

    /** Connection refused */
    public static final int NNG_ECONNREFUSED = 6;

    /** Object closed */
    public static final int NNG_ECLOSED = 7;

    /** Try again (would block) */
    public static final int NNG_EAGAIN = 8;

    /** Not supported */
    public static final int NNG_ENOTSUP = 9;

    /** Address in use */
    public static final int NNG_EADDRINUSE = 10;

    /** Incorrect state */
    public static final int NNG_ESTATE = 11;

    /** Entry not found */
    public static final int NNG_ENOENT = 12;

    /** Protocol error */
    public static final int NNG_EPROTO = 13;

    /** Unreachable */
    public static final int NNG_EUNREACHABLE = 14;

    /** Address invalid */
    public static final int NNG_EADDRINVAL = 15;

    /** Permission denied */
    public static final int NNG_EPERM = 16;

    /** Message too large */
    public static final int NNG_EMSGSIZE = 17;

    /** Connection reset */
    public static final int NNG_ECONNRESET = 18;

    /** Connection aborted */
    public static final int NNG_ECONNABORTED = 19;

    /** Operation canceled */
    public static final int NNG_ECANCELED = 20;

    // ========== High-Level API (Backward Compatibility) ==========

    private long socketHandle = 0;

    /**
     * Open a Pair0 socket (high-level API)
     * @throws NngException if socket creation fails
     */
    public void openPair0() throws NngException {
        socketHandle = nngPair0Open();
        if (socketHandle <= 0) {
            throw new NngException("Failed to open Pair0 socket", (int)(-socketHandle));
        }
        Log.i(TAG, "Pair0 socket opened: " + socketHandle);
    }

    /**
     * Listen on URL (high-level API)
     * @param url URL to listen on
     * @throws NngException if listen fails
     */
    public void listen(@NonNull String url) throws NngException {
        if (socketHandle == 0) throw new IllegalStateException("Socket not open");
        int rv = nngListen(socketHandle, url);
        if (rv != 0) {
            throw new NngException("Failed to listen on " + url, rv);
        }
        Log.i(TAG, "Listening on " + url);
    }

    /**
     * Dial URL (high-level API)
     * @param url URL to connect to
     * @throws NngException if dial fails
     */
    public void dial(@NonNull String url) throws NngException {
        if (socketHandle == 0) throw new IllegalStateException("Socket not open");
        int rv = nngDial(socketHandle, url);
        if (rv != 0) {
            throw new NngException("Failed to dial " + url, rv);
        }
        Log.i(TAG, "Connected to " + url);
    }

    /**
     * Send bytes (high-level API)
     * @param data data to send
     * @throws NngException if send fails
     */
    public void send(@NonNull byte[] data) throws NngException {
        if (socketHandle == 0) throw new IllegalStateException("Socket not open");
        int rv = nngSend(socketHandle, data, data.length, 0);
        if (rv != 0) {
            throw new NngException("Failed to send", rv);
        }
    }

    /**
     * Send string (high-level API)
     * @param message message to send
     * @throws NngException if send fails
     */
    public void sendString(@NonNull String message) throws NngException {
        send(message.getBytes(java.nio.charset.StandardCharsets.UTF_8));
    }

    /**
     * Receive bytes (high-level API)
     * @return received data
     * @throws NngException if receive fails
     */
    @NonNull
    public byte[] receive() throws NngException {
        if (socketHandle == 0) throw new IllegalStateException("Socket not open");
        byte[] buffer = new byte[4096];
        int rv = nngRecv(socketHandle, buffer, buffer.length, 0);
        if (rv < 0) {
            throw new NngException("Failed to receive", -rv);
        }
        byte[] result = new byte[rv];
        System.arraycopy(buffer, 0, result, 0, rv);
        return result;
    }

    /**
     * Receive string (high-level API)
     * @return received message
     * @throws NngException if receive fails
     */
    @NonNull
    public String receiveString() throws NngException {
        byte[] data = receive();
        return new String(data, java.nio.charset.StandardCharsets.UTF_8);
    }

    /**
     * Close socket (high-level API)
     */
    public void close() {
        if (socketHandle != 0) {
            nngClose(socketHandle);
            Log.i(TAG, "Socket closed: " + socketHandle);
            socketHandle = 0;
        }
    }

    /**
     * Custom exception for NNG operations
     */
    public static class NngException extends Exception {
        private final int errorCode;

        public NngException(@NonNull String message, int errorCode) {
            super(message + ": " + getErrorMessage(errorCode));
            this.errorCode = errorCode;
        }

        public int getErrorCode() {
            return errorCode;
        }

        private static String getErrorMessage(int errorCode) {
            // This will be replaced with actual nngStrerror call when available
            switch (errorCode) {
                case NNG_OK: return "Success";
                case NNG_EINTR: return "Interrupted";
                case NNG_ENOMEM: return "Out of memory";
                case NNG_EINVAL: return "Invalid argument";
                case NNG_EBUSY: return "Resource busy";
                case NNG_ETIMEDOUT: return "Timed out";
                case NNG_ECONNREFUSED: return "Connection refused";
                case NNG_ECLOSED: return "Object closed";
                case NNG_EAGAIN: return "Try again";
                case NNG_ENOTSUP: return "Not supported";
                case NNG_EADDRINUSE: return "Address in use";
                case NNG_ESTATE: return "Incorrect state";
                case NNG_ENOENT: return "Entry not found";
                case NNG_EPROTO: return "Protocol error";
                case NNG_EUNREACHABLE: return "Unreachable";
                case NNG_EADDRINVAL: return "Address invalid";
                case NNG_EPERM: return "Permission denied";
                case NNG_EMSGSIZE: return "Message too large";
                case NNG_ECONNRESET: return "Connection reset";
                case NNG_ECONNABORTED: return "Connection aborted";
                case NNG_ECANCELED: return "Operation canceled";
                default: return "Unknown error (" + errorCode + ")";
            }
        }
    }
}
