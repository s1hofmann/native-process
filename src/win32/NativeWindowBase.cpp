#include "NativeWindowBase.h"
#include <iostream>

Napi::FunctionReference NativeWindowBase::constructor;

Napi::Object NativeWindowBase::Init(Napi::Env env, Napi::Object exports) {
    // This method is used to hook the accessor and method callbacks
    Napi::Function func = DefineClass(env, "NativeWindow", {
        InstanceAccessor("x", &NativeWindowBase::getX, nullptr),
        InstanceAccessor("y", &NativeWindowBase::getY, nullptr),
        InstanceAccessor("width", &NativeWindowBase::getWidth, nullptr),
        InstanceAccessor("height", &NativeWindowBase::getHeight, nullptr),
        InstanceAccessor("title", &NativeWindowBase::getTitle, nullptr),
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("NativeWindow", func);
    return exports;
}

NativeWindowBase::NativeWindowBase(const Napi::CallbackInfo &info)
                                   : Napi::ObjectWrap<NativeWindowBase>(info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);
}

NativeWindowBase::~NativeWindowBase() {
}

Napi::Value NativeWindowBase::getX(const Napi::CallbackInfo &info) {
    return Napi::Number::New(info.Env(), 0);
}

Napi::Value NativeWindowBase::getY(const Napi::CallbackInfo &info) {
    return Napi::Number::New(info.Env(), 0);
}

Napi::Value NativeWindowBase::getWidth(const Napi::CallbackInfo &info) {
    return Napi::Number::New(info.Env(), 0);
}

Napi::Value NativeWindowBase::getHeight(const Napi::CallbackInfo &info) {
    return Napi::Number::New(info.Env(), 0);
}

Napi::Value NativeWindowBase::getTitle(const Napi::CallbackInfo &info) {
    return Napi::String::New(info.Env(), "The title.");
}
