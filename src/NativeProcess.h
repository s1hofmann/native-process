#pragma once

#include <napi.h>

#if defined (__APPLE__)
#include <ApplicationServices/ApplicationServices.h>
#else
#endif

class NativeProcess : public Napi::ObjectWrap<NativeProcess> {
  public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    NativeProcess(const Napi::CallbackInfo &info);

  private:
    static Napi::FunctionReference constructor;
    Napi::Value getMainWindow(const Napi::CallbackInfo &info);
};
