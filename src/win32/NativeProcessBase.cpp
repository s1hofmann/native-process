#include "NativeProcessBase.h"
#include <iostream>

Napi::FunctionReference NativeProcessBase::constructor;

Napi::Object NativeProcessBase::Init(Napi::Env env, Napi::Object exports)
{
    // This method is used to hook the accessor and method callbacks
    Napi::Function func = DefineClass(env, "NativeProcessBase", {
        InstanceMethod("getMainWindow", &NativeProcessBase::getMainWindow),
        InstanceMethod("getWindows", &NativeProcessBase::getWindows),
    });

    // Create a peristent reference to the class constructor. This will allow
    // a function called on a class prototype and a function
    // called on instance of a class to be distinguished from each other.
    constructor = Napi::Persistent(func);
    // Call the SuppressDestruct() method on the static data prevent the calling
    // to this destructor to reset the reference when the environment is no longer
    // available.
    constructor.SuppressDestruct();
    exports.Set("NativeProcessBase", func);
    return exports;
}

NativeProcessBase::NativeProcessBase(const Napi::CallbackInfo &info) : Napi::ObjectWrap<NativeProcessBase>(info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);
}

Napi::Value NativeProcessBase::getMainWindow(const Napi::CallbackInfo &info) {
    if (!info[0].IsNumber()) {
        Napi::Error::New(info.Env(), "Expected numeric process id.").ThrowAsJavaScriptException();
    }
    const int32_t pid = info[0].As<Napi::Number>().Int32Value();
    Napi::Object windowBoundary = Napi::Object::New(info.Env());

    return windowBoundary;
}

Napi::Value NativeProcessBase::getWindows(const Napi::CallbackInfo &info) {
    if (!info[0].IsNumber()) {
        Napi::Error::New(info.Env(), "Expected numeric process id.").ThrowAsJavaScriptException();
    }
    const int32_t pid = info[0].As<Napi::Number>().Int32Value();
    Napi::Object windowBoundary = Napi::Object::New(info.Env());

    EnumWindowsParam enumWindowsParam;
    enumWindowsParam.pid = static_cast<DWORD>(pid);

    // EnumWindows(EnumWindowsHandler, (LPARAM)&enumWindowsParam);

    std::cout << enumWindowsParam.windowHandles.size() << std::endl;

    return windowBoundary;
}
