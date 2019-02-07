#pragma once

#include <napi.h>
#include <ApplicationServices/ApplicationServices.h>

#include "NativeWindowBase.h"

class NativeProcessBase : public Napi::ObjectWrap<NativeProcessBase> {
  public:
    static Napi::FunctionReference constructor;
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    NativeProcessBase(const Napi::CallbackInfo &info);

    static constexpr long MAX_WINDOWS = 1024;

  private:
    Napi::Value getMainWindow(const Napi::CallbackInfo &info);
    Napi::Value getWindows(const Napi::CallbackInfo &info);
};
