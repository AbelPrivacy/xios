#include <napi.h>
#include <iostream>
#include "src/xios.hpp"

using namespace Napi;

// Helper: Convert JS array of strings to std::vector<std::string>
std::vector<std::string> JsArrayToStringVector(const Array& jsArray) {
    std::vector<std::string> vec;
    for (uint32_t i = 0; i < jsArray.Length(); ++i) {
        if (jsArray.Get(i).IsString()) {
            vec.push_back(jsArray.Get(i).As<String>().Utf8Value());
        }
    }
    return vec;
}

// JS: SecureHttpClient.initialize(sqlitePath, allowedProtocols, allowedCiphers, blockedCiphers, allowedGroups)
void InitializeWrapped(const CallbackInfo& info) {
    Env env = info.Env();
    try {

        if (info.Length() != 5 || !info[0].IsString() ||
            !info[1].IsArray() || !info[2].IsArray() ||
            !info[3].IsArray() || !info[4].IsArray()) {
            std::cerr << "Expected (string, array, array, array, array)" << std::endl;
        }

        std::string sqlitePath = info[0].As<String>().Utf8Value();
        auto allowedProtocols = JsArrayToStringVector(info[1].As<Array>());
        auto allowedCiphers = JsArrayToStringVector(info[2].As<Array>());
        auto blockedCiphers = JsArrayToStringVector(info[3].As<Array>());
        auto allowedGroups = JsArrayToStringVector(info[4].As<Array>());

        SecureHttpClient::initialize(sqlitePath, allowedProtocols, allowedCiphers, blockedCiphers, allowedGroups);
    } catch (const std::exception& e) {
        throw Napi::Error::New(env, e.what());
    } catch (...) {
        throw Napi::Error::New(env, "Unknown exception occurred");
    }
}

// JS: SecureHttpClient.get(url): string
Value GetWrapped(const CallbackInfo& info) {
    Env env = info.Env();
    try {
    
        if (info.Length() != 1 || !info[0].IsString()) {
            std::cerr << "Expected (string)" << std::endl;
        }
    
        std::string url = info[0].As<String>().Utf8Value();
        std::string result;
    
        result = SecureHttpClient::get(url);
    
        return String::New(env, result);
    } catch (const std::exception& e) {
        throw Napi::Error::New(env, e.what());
    } catch (...) {
        throw Napi::Error::New(env, "Unknown exception occurred");
    }
}

// N-API module initialization
Object Init(Env env, Object exports) {
    try {
        exports.Set("initialize", Function::New(env, InitializeWrapped));
        exports.Set("get", Function::New(env, GetWrapped));
        return exports;
    } catch (const std::exception& e) {
        throw Napi::Error::New(env, e.what());
    } catch (...) {
        throw Napi::Error::New(env, "Unknown exception occurred");
    }
}

NODE_API_MODULE(secure_http_client, Init)