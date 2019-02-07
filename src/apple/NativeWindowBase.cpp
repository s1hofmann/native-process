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
                                   : Napi::ObjectWrap<NativeWindowBase>(info),
                                     uiElementRef(*info[0].As<Napi::External<AXUIElementRef>>().Data()) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);
    CFRetain(this->uiElementRef);
}

NativeWindowBase::~NativeWindowBase() {
    CFRelease(this->uiElementRef);
}

Napi::Value NativeWindowBase::getX(const Napi::CallbackInfo &info) {
    AXValueRef positionRef = nullptr;

    if (AXUIElementCopyAttributeValue(this->uiElementRef, kAXPositionAttribute, (CFTypeRef *) &positionRef) != kAXErrorSuccess || positionRef == nullptr) {
        Napi::TypeError::New(info.Env(), "Failed to determine window size.").ThrowAsJavaScriptException();
    }

    CGPoint positionValue;

    if (!AXValueGetValue(positionRef, kAXValueTypeCGPoint, &positionValue)) {
        Napi::TypeError::New(info.Env(), "Failed to unpack window size.").ThrowAsJavaScriptException();
    }

    return Napi::Number::New(info.Env(), positionValue.x);
}

Napi::Value NativeWindowBase::getY(const Napi::CallbackInfo &info) {
    AXValueRef positionRef = nullptr;

    if (AXUIElementCopyAttributeValue(this->uiElementRef, kAXPositionAttribute, (CFTypeRef *) &positionRef) != kAXErrorSuccess || positionRef == nullptr) {
        Napi::TypeError::New(info.Env(), "Failed to determine window size.").ThrowAsJavaScriptException();
    }

    CGPoint positionValue;

    if (!AXValueGetValue(positionRef, kAXValueTypeCGPoint, &positionValue)) {
        Napi::TypeError::New(info.Env(), "Failed to unpack window size.").ThrowAsJavaScriptException();
    }

    return Napi::Number::New(info.Env(), positionValue.y);
}

Napi::Value NativeWindowBase::getWidth(const Napi::CallbackInfo &info) {
    AXValueRef sizeRef = nullptr;

    if (AXUIElementCopyAttributeValue(this->uiElementRef, kAXSizeAttribute, (CFTypeRef *) &sizeRef) != kAXErrorSuccess || sizeRef == nullptr) {
        Napi::TypeError::New(info.Env(), "Failed to determine window size.").ThrowAsJavaScriptException();
    }

    CGSize sizeValue;

    if (!AXValueGetValue(sizeRef, kAXValueTypeCGSize, &sizeValue)) {
        Napi::TypeError::New(info.Env(), "Failed to unpack window size.").ThrowAsJavaScriptException();
    }

    return Napi::Number::New(info.Env(), sizeValue.width);
}

Napi::Value NativeWindowBase::getHeight(const Napi::CallbackInfo &info) {
    AXValueRef sizeRef = nullptr;

    if (AXUIElementCopyAttributeValue(this->uiElementRef, kAXSizeAttribute, (CFTypeRef *) &sizeRef) != kAXErrorSuccess || sizeRef == nullptr) {
        Napi::TypeError::New(info.Env(), "Failed to determine window size.").ThrowAsJavaScriptException();
    }

    CGSize sizeValue;

    if (!AXValueGetValue(sizeRef, kAXValueTypeCGSize, &sizeValue)) {
        Napi::TypeError::New(info.Env(), "Failed to unpack window size.").ThrowAsJavaScriptException();
    }

    return Napi::Number::New(info.Env(), sizeValue.height);
}

Napi::Value NativeWindowBase::getTitle(const Napi::CallbackInfo &info) {
    CFStringRef windowTitleRef = nullptr;

	if (AXUIElementCopyAttributeValue (this->uiElementRef, kAXTitleAttribute, (CFTypeRef*) &windowTitleRef) == kAXErrorSuccess && windowTitleRef != nullptr) {
		char windowTitle[NativeWindowBase::WINDOW_TITLE_MAX];
        if (CFStringGetCString(windowTitleRef, windowTitle, NativeWindowBase::WINDOW_TITLE_MAX, kCFStringEncodingUTF8)) {
            CFRelease (windowTitleRef);
            return Napi::String::New(info.Env(), windowTitle);
        }
        CFRelease (windowTitleRef);
	}

    Napi::Error::New(info.Env(), "Failed to determine window title.");
}
