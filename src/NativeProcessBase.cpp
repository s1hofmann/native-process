#include "NativeProcessBase.h"

Napi::FunctionReference NativeProcessBase::constructor;

Napi::Object NativeProcessBase::Init(Napi::Env env, Napi::Object exports)
{
    // This method is used to hook the accessor and method callbacks
    Napi::Function func = DefineClass(env, "NativeProcessBase", {
        InstanceMethod("getMainWindow", &NativeProcessBase::getMainWindow),
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
    Napi::Number nPid = info[0].As<Napi::Number>();
    int32_t pid = nPid.Int32Value();
    Napi::Object windowBoundary = Napi::Object::New(info.Env());

#if defined (__APPLE__)
    // Create accessibility object using the process PID
    auto application = AXUIElementCreateApplication(pid);
    if (application == nullptr) {
        Napi::TypeError::New(info.Env(), "Failed to create top-level accessibility object for process.").ThrowAsJavaScriptException();
    }

    CFArrayRef windows = nullptr;
    // Get all windows associated with the app
    AXUIElementCopyAttributeValues(application,
                                   kAXWindowsAttribute,
                                   0,
                                   1024,
                                   &windows);

    if (windows == nullptr) {
        Napi::TypeError::New(info.Env(), "No windows associated with process.").ThrowAsJavaScriptException();
    } else {
        auto count = CFArrayGetCount(windows);
        // Loop all windows in the process
        for (CFIndex i = 0; i < count; ++i) {
            // Get the element at the index
            auto element = (AXUIElementRef)
                CFArrayGetValueAtIndex(windows, i);

            CFBooleanRef isMain = nullptr;
            if (
                AXUIElementCopyAttributeValue(element, kAXMainAttribute, (CFTypeRef *)&isMain) != kAXErrorSuccess ||
                isMain == nullptr) {
                continue;
            }

            if (!CFBooleanGetValue(isMain)) {
                continue;
            }

            AXValueRef positionRef = nullptr;
            AXValueRef sizeRef = nullptr;

            if (AXUIElementCopyAttributeValue(element, kAXPositionAttribute, (CFTypeRef *)&positionRef) != kAXErrorSuccess || positionRef == nullptr) {
                Napi::TypeError::New(info.Env(), "Failed to determine window position.").ThrowAsJavaScriptException();
            }
            if (AXUIElementCopyAttributeValue(element, kAXSizeAttribute, (CFTypeRef *)&sizeRef) != kAXErrorSuccess || sizeRef == nullptr) {
                Napi::TypeError::New(info.Env(), "Failed to determine window size.").ThrowAsJavaScriptException();
            }

            CGPoint positionValue;
            CGSize sizeValue;

            if (!AXValueGetValue(positionRef, kAXValueTypeCGPoint, &positionValue)) {
                Napi::TypeError::New(info.Env(), "Failed to unpack window position.").ThrowAsJavaScriptException();
            }
            if (!AXValueGetValue(sizeRef, kAXValueTypeCGSize, &sizeValue)) {
                Napi::TypeError::New(info.Env(), "Failed to unpack window size.").ThrowAsJavaScriptException();
            }

            windowBoundary.Set("x", positionValue.x);
            windowBoundary.Set("y", positionValue.y);
            windowBoundary.Set("width", sizeValue.width);
            windowBoundary.Set("height", sizeValue.height);

            CFRetain(element);
            break;
        }
        CFRelease(windows);
    }
    CFRelease(application);
#elif defined (__linux__)
#elif defined (_WIN32)
#endif

    return windowBoundary;
}

// Initialize native add-on
Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    NativeProcessBase::Init(env, exports);
    return exports;
}

NODE_API_MODULE(native_process, Init)
