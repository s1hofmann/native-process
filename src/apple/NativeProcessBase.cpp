#include "NativeProcessBase.h"
#include <iostream>
#include <thread>
#include <chrono>

Napi::FunctionReference NativeProcessBase::constructor;

Napi::Object NativeProcessBase::Init(Napi::Env env, Napi::Object exports) {
    // This method is used to hook the accessor and method callbacks
    Napi::Function func = DefineClass(env, "NativeProcessBase", {
        InstanceMethod("getMainWindow", &NativeProcessBase::getMainWindow),
        InstanceMethod("getWindows", &NativeProcessBase::getWindows),
    });

    constructor = Napi::Persistent(func);
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
    Napi::Number nPid = info[0].As<Napi::Number>();
    const int32_t pid = nPid.Int32Value();
    Napi::Object windowBoundary = Napi::Object::New(info.Env());

    // Create accessibility object using the process PID
    auto application = AXUIElementCreateApplication(pid);
    if (application == nullptr) {
        Napi::TypeError::New(info.Env(), "Failed to create top-level accessibility object for process.").ThrowAsJavaScriptException();
    }

    AXUIElementRef mainWindow = nullptr;
    AXError getMainWindowError = AXUIElementCopyAttributeValue(application, kAXMainWindowAttribute, (CFTypeRef *) &mainWindow);
    for (int retry = 0; retry < 5; ++retry) {
        if (getMainWindowError == kAXErrorCannotComplete) {
            std::cout << "Failed to grab main window reference on try: " << retry << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            getMainWindowError = AXUIElementCopyAttributeValue(application, kAXMainWindowAttribute, (CFTypeRef *) &mainWindow);
        } else {
            break;
        }
    }
    if (getMainWindowError != kAXErrorSuccess || mainWindow == nullptr) {
        Napi::TypeError::New(info.Env(), "Failed to determine main window.").ThrowAsJavaScriptException();
    }

    return NativeWindowBase::constructor.New({ Napi::External<AXUIElementRef>::New(info.Env(), &mainWindow) });
}

Napi::Value NativeProcessBase::getWindows(const Napi::CallbackInfo &info)
{
    if (!info[0].IsNumber()) {
        Napi::Error::New(info.Env(), "Expected numeric process id.").ThrowAsJavaScriptException();
    }
    Napi::Number nPid = info[0].As<Napi::Number>();
    const int32_t pid = nPid.Int32Value();
    Napi::Array windowBoundaries;

    // Create accessibility object using the process PID
    auto application = AXUIElementCreateApplication(pid);
    if (application == nullptr) {
        Napi::TypeError::New(info.Env(), "Failed to create top-level accessibility object for process.").ThrowAsJavaScriptException();
    }

    CFArrayRef windows = nullptr;
    AXError getWindowsError = AXUIElementCopyAttributeValues(application,
                                                            kAXWindowsAttribute,
                                                            0,
                                                            NativeProcessBase::MAX_WINDOWS,
                                                            &windows);
    for (int retry = 0; retry < 5; ++retry) {
        if (getWindowsError == kAXErrorCannotComplete) {
            std::cout << "Failed to grab main window reference on try: " << retry << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            getWindowsError = AXUIElementCopyAttributeValues(application,
                                                            kAXWindowsAttribute,
                                                            0,
                                                            NativeProcessBase::MAX_WINDOWS,
                                                            &windows);
            getWindowsError = AXUIElementCopyAttributeValue(application, kAXMainWindowAttribute, (CFTypeRef *) &windows);
        } else {
            break;
        }
    }

    if (windows == nullptr) {
        Napi::TypeError::New(info.Env(), "No windows associated with process.").ThrowAsJavaScriptException();
    } else {
        const CFIndex count = CFArrayGetCount(windows);
        windowBoundaries = Napi::Array::New(info.Env(), count);
        // Loop all windows in the process
        for (CFIndex idx = 0; idx < count; ++idx) {
            // Get the element at the index
            auto element = (AXUIElementRef)
                CFArrayGetValueAtIndex(windows, idx);
            windowBoundaries.Set(idx, NativeWindowBase::constructor.New({ Napi::External<AXUIElementRef>::New(info.Env(), &element) }));
        }
    }

    return windowBoundaries;
}
