#include <napi.h>
#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif

#define NANOSOCKETS_IMPLEMENTATION
#include "nanosockets.h" 

Napi::Value Initialize(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    NanoStatus status = nanosockets_initialize();
    return Napi::Number::New(env, status);
}

Napi::Value Deinitialize(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    nanosockets_deinitialize();
    return env.Undefined();
}

Napi::Value Create(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    int sendBufferSize = info[0].As<Napi::Number>().Int32Value();
    int receiveBufferSize = info[1].As<Napi::Number>().Int32Value();
    NanoSocket socket = nanosockets_create(sendBufferSize, receiveBufferSize);
    return Napi::Number::New(env, socket);
}

Napi::Value Destroy(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    NanoSocket socket = info[0].As<Napi::Number>().Int64Value();
    
    nanosockets_destroy(&socket);
    return env.Undefined();
}

Napi::Value Bind(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    NanoSocket socket = info[0].As<Napi::Number>().Int64Value();
    std::string ip = info[1].As<Napi::String>();
    uint16_t port = info[2].As<Napi::Number>().Uint32Value();

    NanoAddress address;
    nanosockets_address_set_ip(&address, ip.c_str());
    address.port = port;

    NanoStatus status = (NanoStatus)nanosockets_bind(socket, &address);
    return Napi::Number::New(env, status);
}

Napi::Value Connect(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    NanoSocket socket = info[0].As<Napi::Number>().Int64Value();
    std::string ip = info[1].As<Napi::String>();
    uint16_t port = info[2].As<Napi::Number>().Uint32Value();

    NanoAddress address;
    nanosockets_address_set_ip(&address, ip.c_str());
    address.port = port;

    NanoStatus status = (NanoStatus)nanosockets_connect(socket, &address);
    return Napi::Number::New(env, status);
}

Napi::Value SetOption(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    NanoSocket socket = info[0].As<Napi::Number>().Int64Value();
    int level = info[1].As<Napi::Number>().Int32Value();
    int optionName = info[2].As<Napi::Number>().Int32Value();
    int optionValue = info[3].As<Napi::Number>().Int32Value();
    
    NanoStatus status = nanosockets_set_option(socket, level, optionName, &optionValue, sizeof(optionValue));
    return Napi::Number::New(env, status);
}

Napi::Value GetOption(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    NanoSocket socket = info[0].As<Napi::Number>().Int64Value();
    int level = info[1].As<Napi::Number>().Int32Value();
    int optionName = info[2].As<Napi::Number>().Int32Value();
    int optionValue;
    int optionLen = sizeof(optionValue);

    NanoStatus status = nanosockets_get_option(socket, level, optionName, &optionValue, &optionLen);
    
    Napi::Object result = Napi::Object::New(env);
    result.Set("status", Napi::Number::New(env, status));
    result.Set("value", Napi::Number::New(env, optionValue));
    return result;
}

Napi::Value SetNonBlocking(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    NanoSocket socket = info[0].As<Napi::Number>().Int64Value();
    bool nonBlocking = info[1].As<Napi::Boolean>().Value();

    NanoStatus status = nanosockets_set_nonblocking(socket, nonBlocking ? 1 : 0);
    return Napi::Number::New(env, status);
}

Napi::Value SetDontFragment(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    NanoSocket socket = info[0].As<Napi::Number>().Int64Value();
    
    NanoStatus status = nanosockets_set_dontfragment(socket);
    return Napi::Number::New(env, status);
}

Napi::Value Poll(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    NanoSocket socket = info[0].As<Napi::Number>().Int64Value();
    int64_t timeout = info[1].As<Napi::Number>().Int64Value();

    NanoStatus status = (NanoStatus)nanosockets_poll(socket, timeout);
    return Napi::Number::New(env, status);
}

Napi::Value Send(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    NanoSocket socket = info[0].As<Napi::Number>().Int64Value();
    std::string ip = info[1].As<Napi::String>();
    uint16_t port = info[2].As<Napi::Number>().Uint32Value();
    Napi::Buffer<uint8_t> buffer = info[3].As<Napi::Buffer<uint8_t>>();

    NanoAddress address;
    nanosockets_address_set_ip(&address, ip.c_str());
    address.port = port;

    int sendResult = nanosockets_send(socket, &address, buffer.Data(), buffer.Length());
    return Napi::Number::New(env, sendResult);
}

Napi::Value Receive(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    NanoSocket socket = info[0].As<Napi::Number>().Int64Value();
    int bufferSize = info[1].As<Napi::Number>().Int32Value();

    std::vector<uint8_t> buffer(bufferSize);
    NanoAddress address;
    
    int receiveResult = nanosockets_receive(socket, &address, buffer.data(), bufferSize);
    
    Napi::Object result = Napi::Object::New(env);
    result.Set("status", Napi::Number::New(env, receiveResult));
    result.Set("data", Napi::Buffer<uint8_t>::Copy(env, buffer.data(), receiveResult));
    
    char ip[INET6_ADDRSTRLEN];
    nanosockets_address_get_ip(&address, ip, sizeof(ip));
    
    result.Set("address", Napi::String::New(env, ip));
    result.Set("port", Napi::Number::New(env, address.port));
    
    return result;
}

Napi::Value IsEqual(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    NanoAddress* address1 = Napi::ObjectWrap<NanoAddress>::Unwrap(info[0].As<Napi::Object>());
    NanoAddress* address2 = Napi::ObjectWrap<NanoAddress>::Unwrap(info[1].As<Napi::Object>());

    NanoStatus status = nanosockets_address_is_equal(address1, address2);
    return Napi::Number::New(env, status);
}

Napi::Value SetIP(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    NanoAddress* address = Napi::ObjectWrap<NanoAddress>::Unwrap(info[0].As<Napi::Object>());
    std::string ip = info[1].As<Napi::String>();

    NanoStatus status = nanosockets_address_set_ip(address, ip.c_str());
    return Napi::Number::New(env, status);
}

Napi::Value GetIP(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    NanoAddress* address = Napi::ObjectWrap<NanoAddress>::Unwrap(info[0].As<Napi::Object>());
    Napi::Buffer<char> ipBuffer = info[1].As<Napi::Buffer<char>>();
    int ipLength = info[2].As<Napi::Number>().Int32Value();

    NanoStatus status = nanosockets_address_get_ip(address, ipBuffer.Data(), ipLength);
    return Napi::Number::New(env, status);
}


Napi::Value GetAddress(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    NanoSocket socket = info[0].As<Napi::Number>().Int64Value();

    NanoAddress address;
    NanoStatus status = nanosockets_address_get(socket, &address);

    if (status != NANOSOCKETS_STATUS_OK) {
        Napi::TypeError::New(env, "Failed to get address").ThrowAsJavaScriptException();
        return env.Null();
    }

    Napi::Object addressObj = Napi::Object::New(env);

    if (address.ipv4.ffff == 0xFFFF) {
        Napi::Object ipv4Obj = Napi::Object::New(env);
        Napi::Array zerosArray = Napi::Array::New(env, 10);
        for (uint32_t i = 0; i < 10; i++) {
            zerosArray.Set(i, Napi::Number::New(env, address.ipv4.zeros[i]));
        }
        ipv4Obj.Set("zeros", zerosArray);
        ipv4Obj.Set("ffff", Napi::Number::New(env, address.ipv4.ffff));
        ipv4Obj.Set("ip", Napi::Number::New(env, address.ipv4.ip.s_addr));

        addressObj.Set("ipv4", ipv4Obj);
    } else {
        Napi::Array ipv6Array = Napi::Array::New(env, 16);

        for (uint32_t i = 0; i < 16; i++) {
            ipv6Array.Set(i, Napi::Number::New(env, address.ipv6.s6_addr[i]));
        }

        addressObj.Set("ipv6", ipv6Array);
    }

    addressObj.Set("port", Napi::Number::New(env, address.port));

    return addressObj;
}

Napi::Value SetHostName(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    Napi::Object addressObj = info[0].As<Napi::Object>();
    std::string hostname = info[1].As<Napi::String>().Utf8Value();

    NanoAddress address;

    if (addressObj.Has("ipv4")) {
        Napi::Object ipv4Obj = addressObj.Get("ipv4").As<Napi::Object>();
        address.ipv4.ffff = ipv4Obj.Get("ffff").As<Napi::Number>().Uint32Value();
        Napi::Array zerosArray = ipv4Obj.Get("zeros").As<Napi::Array>();
        for (uint32_t i = 0; i < zerosArray.Length(); i++) {
            address.ipv4.zeros[i] = zerosArray.Get(i).As<Napi::Number>().Uint32Value();
        }
        address.ipv4.ip.s_addr = ipv4Obj.Get("ip").As<Napi::Number>().Uint32Value();
    } else if (addressObj.Has("ipv6")) {
        Napi::Array ipv6Array = addressObj.Get("ipv6").As<Napi::Array>();
        for (uint32_t i = 0; i < ipv6Array.Length(); i++) {
            address.ipv6.s6_addr[i] = ipv6Array.Get(i).As<Napi::Number>().Uint32Value();
        }
    }

    address.port = addressObj.Get("port").As<Napi::Number>().Uint32Value();

    NanoStatus status = nanosockets_address_set_hostname(&address, hostname.c_str());

    if (status != NANOSOCKETS_STATUS_OK) {
        Napi::TypeError::New(env, "Failed to set hostname").ThrowAsJavaScriptException();
    }

    return env.Undefined();
}


Napi::Value GetHostName(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::Object addressObj = info[0].As<Napi::Object>();

    NanoAddress address;

    if (addressObj.Has("ipv4")) {
        Napi::Object ipv4Obj = addressObj.Get("ipv4").As<Napi::Object>();
        address.ipv4.ffff = ipv4Obj.Get("ffff").As<Napi::Number>().Uint32Value();
        Napi::Array zerosArray = ipv4Obj.Get("zeros").As<Napi::Array>();
        for (uint32_t i = 0; i < zerosArray.Length(); i++) {
            address.ipv4.zeros[i] = zerosArray.Get(i).As<Napi::Number>().Uint32Value();
        }
        address.ipv4.ip.s_addr = ipv4Obj.Get("ip").As<Napi::Number>().Uint32Value();
    } else if (addressObj.Has("ipv6")) {
        Napi::Array ipv6Array = addressObj.Get("ipv6").As<Napi::Array>();
        for (uint32_t i = 0; i < ipv6Array.Length(); i++) {
            address.ipv6.s6_addr[i] = ipv6Array.Get(i).As<Napi::Number>().Uint32Value();
        }
    }

    address.port = addressObj.Get("port").As<Napi::Number>().Uint32Value();

    char hostname[64];
    
    NanoStatus status = nanosockets_address_get_hostname(&address, hostname, sizeof(hostname));

    if (status != NANOSOCKETS_STATUS_OK) {
        Napi::TypeError::New(env, "Failed to get hostname").ThrowAsJavaScriptException();
        return env.Null();
    }

    return Napi::String::New(env, hostname);
}


Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "initialize"), Napi::Function::New(env, Initialize));
    exports.Set(Napi::String::New(env, "deinitialize"), Napi::Function::New(env, Deinitialize));
    exports.Set(Napi::String::New(env, "create"), Napi::Function::New(env, Create));
    exports.Set(Napi::String::New(env, "destroy"), Napi::Function::New(env, Destroy));
    exports.Set(Napi::String::New(env, "bind"), Napi::Function::New(env, Bind));
    exports.Set(Napi::String::New(env, "connect"), Napi::Function::New(env, Connect));
    exports.Set(Napi::String::New(env, "setOption"), Napi::Function::New(env, SetOption));
    exports.Set(Napi::String::New(env, "getOption"), Napi::Function::New(env, GetOption));
    exports.Set(Napi::String::New(env, "setNonBlocking"), Napi::Function::New(env, SetNonBlocking));
    exports.Set(Napi::String::New(env, "setDontFragment"), Napi::Function::New(env, SetDontFragment));
    exports.Set(Napi::String::New(env, "poll"), Napi::Function::New(env, Poll));
    exports.Set(Napi::String::New(env, "send"), Napi::Function::New(env, Send));
    exports.Set(Napi::String::New(env, "receive"), Napi::Function::New(env, Receive));
    exports.Set(Napi::String::New(env, "isEqual"), Napi::Function::New(env, IsEqual));
    exports.Set(Napi::String::New(env, "setIP"), Napi::Function::New(env, SetIP));
    exports.Set(Napi::String::New(env, "getIP"), Napi::Function::New(env, GetIP));
    exports.Set(Napi::String::New(env, "getAddress"), Napi::Function::New(env, GetAddress));
    exports.Set(Napi::String::New(env, "setHostName"), Napi::Function::New(env, SetHostName));
    exports.Set(Napi::String::New(env, "getHostName"), Napi::Function::New(env, GetHostName));
        
    return exports;
}

NODE_API_MODULE(nanosockets_binding, Init)
