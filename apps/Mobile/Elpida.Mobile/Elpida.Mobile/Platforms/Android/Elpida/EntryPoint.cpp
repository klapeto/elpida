#include <string>
#include "Elpida/Platform/CpuInfoLoader.hpp"
// #include "../../../../../../../include/Elpida/Platform/CpuInfoLoader.hpp"
// #include "../../../../../../../include/Elpida/Core/CpuInfo.hpp"

#include <jni.h>
#include <string>
extern "C" {

// JNIEXPORT jstring JNICALL
// Java_dev_elpida_myapplication_MainActivity_stringFromJNI(
//         JNIEnv* env,
//         jobject /* this */) {
//     std::string hello = "Hello from C++";
//
//     return env->NewStringUTF(hello.c_str());
// }

void GetCpuName(const char* ptr, size_t size, size_t* actualSize)
{    
    auto info = Elpida::CpuInfoLoader::Load();
    auto name = info.GetModelName();
    *actualSize = std::min(size, name.size());
    std::memcpy((void*)ptr, name.c_str(), size);
}

const char* GetMessage()
{
    return "TEST NATIVE";
}
}
