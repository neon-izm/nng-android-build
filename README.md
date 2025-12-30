# NNG Android

[![Build Status](https://github.com/nanomsg/nng-android-build/workflows/Build%20NNG%20Shared%20Library%20for%20Android/badge.svg)](https://github.com/nanomsg/nng-android-build/actions)

NNG (nanomsg-next-generation) messaging library for Android. Easy-to-use AAR package with native performance.

## 🚀 Quick Start

### 1. Add Dependency

```gradle
dependencies {
    implementation 'com.nng.android:nng-android:1.11.0'
}
```

### 2. Basic Usage

```java
NngWrapper nng = new NngWrapper();

// Simple send/receive
nng.openPair0();
nng.listen("tcp://127.0.0.1:5555");
nng.sendString("Hello World");
String response = nng.receiveString();
nng.close();
```

## 📦 Features

- ✅ **All Android architectures** (arm64-v8a, armeabi-v7a, x86_64, x86)
- ✅ **Multiple protocols** (Pair, Req/Rep, Pub/Sub, Push/Pull, Bus)
- ✅ **High-level Java API** with error handling
- ✅ **Sample app** for testing send/receive
- ✅ **ProGuard ready** with consumer rules

## 🛠️ Development

```bash
# Build AAR
./gradlew android-library:assembleRelease

# Build sample app  
./gradlew sample-app:assembleDebug
```

**Requirements**: Android NDK r28c+, Java 17+, Gradle 8.7+

## 📱 Sample App

Test send/receive on a single device with the included sample app:

```bash
./gradlew sample-app:installDebug
```

Features: Auto-setup, self-testing, real-time message display.

## 🔧 Advanced Usage

### Error Handling
```java
try {
    nng.openPair0();
    nng.listen("tcp://127.0.0.1:5555");
} catch (NngWrapper.NngException e) {
    Log.e("NNG", "Error: " + e.getMessage());
}
```

### Pub/Sub Pattern
```java
// Publisher
long pubSocket = nng.nngPub0Open();
nng.nngListen(pubSocket, "tcp://127.0.0.1:5555");

// Subscriber  
long subSocket = nng.nngSub0Open();
nng.nngSubSubscribe(subSocket, "topic".getBytes());
nng.nngDial(subSocket, "tcp://127.0.0.1:5555");
```

### Debug Logging
```bash
adb logcat -s NngWrapper
```


## 📄 License

Unlicense