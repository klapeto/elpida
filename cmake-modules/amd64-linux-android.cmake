set(CMAKE_SYSTEM_NAME Android)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_SYSTEM_VERSION 23)
set(ANDROID_PLATFORM android-23)
set(ANDROID_ABI x86_64)
set(CMAKE_ANDROID_ARCH_ABI x86_64)
set(ANDROID_NDK /home/klapeto/Android/Sdk/ndk/27.0.12077973)
set(CMAKE_ANDROID_NDK /home/klapeto/Android/Sdk/ndk/27.0.12077973)
include(/home/klapeto/Android/Sdk/ndk/27.0.12077973/build/cmake/android.toolchain.cmake)

set(CROSS_TRIPLE ${CMAKE_LIBRARY_ARCHITECTURE}${ANDROID_NATIVE_API_LEVEL})

