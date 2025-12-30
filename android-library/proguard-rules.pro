# Add project specific ProGuard rules here.
# You can control the set of applied configuration files using the
# proguardFiles setting in build.gradle.

# Keep all native methods
-keepclasseswithmembernames class * {
    native <methods>;
}

# Keep NNG wrapper classes
-keep class com.nng.android.** { *; }

# Keep JNI related classes
-keepclasseswithmembers class * {
    public static native <methods>;
}

# Keep exceptions
-keep public class * extends java.lang.Exception
