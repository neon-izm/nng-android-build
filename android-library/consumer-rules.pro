# Consumer ProGuard rules for NNG Android Library

# Keep all native methods
-keepclasseswithmembernames class * {
    native <methods>;
}

# Keep NNG wrapper classes and methods
-keep class com.nng.android.** { *; }

# Keep JNI related classes
-keepclasseswithmembers class * {
    public static native <methods>;
}
