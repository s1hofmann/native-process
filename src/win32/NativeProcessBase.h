#pragma once

#include <napi.h>

class NativeProcessBase : public Napi::ObjectWrap<NativeProcessBase> {
  public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    NativeProcessBase(const Napi::CallbackInfo &info);

  private:
    static Napi::FunctionReference constructor;
    Napi::Value getMainWindow(const Napi::CallbackInfo &info);
    Napi::Value getWindows(const Napi::CallbackInfo &info);
};
