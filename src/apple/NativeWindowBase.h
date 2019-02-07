
#pragma once

#include <napi.h>
#include <ApplicationServices/ApplicationServices.h>

class NativeWindowBase : public Napi::ObjectWrap<NativeWindowBase> {
  public:
    static constexpr long WINDOW_TITLE_MAX = 512;

    static Napi::FunctionReference constructor;
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    explicit NativeWindowBase(const Napi::CallbackInfo &info);
    ~NativeWindowBase();

  private:
    Napi::Value getX(const Napi::CallbackInfo &info);
    Napi::Value getY(const Napi::CallbackInfo &info);
    Napi::Value getWidth(const Napi::CallbackInfo &info);
    Napi::Value getHeight(const Napi::CallbackInfo &info);

    Napi::Value getTitle(const Napi::CallbackInfo &info);

    AXUIElementRef uiElementRef;
};
