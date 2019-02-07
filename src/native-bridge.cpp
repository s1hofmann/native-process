#include <napi.h>

#if defined (__APPLE__)
#include "includes_apple.h"
#elif defined (__linux__)
#include "includes_linux.h"
#elif defined (_WIN32)
#include "includes_win32.h"
#endif

// Initialize native add-on
Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    NativeProcessBase::Init(env, exports);
    NativeWindowBase::Init(env, exports);
    return exports;
}

NODE_API_MODULE(nut_native_bridge, Init)
