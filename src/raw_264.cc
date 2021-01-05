#include "async_decoder.h"
#include "raw_264.h"

#include <napi.h>

Raw264::Raw264(const Napi::CallbackInfo &);

Napi::Value Raw264::Decode(const Napi::CallbackInfo &)
{
  static Napi::Value Decode(const Napi::CallbackInfo &info)
  {
    Napi::Env env = info.Env();

    if (info.Length() < 1)
    {
      Napi::TypeError::New(env, "Missing Arguments")
          .ThrowAsJavaScriptException();
      return env.Null();
    }

    if (!info[0].IsBuffer())
    {
      Napi::TypeError::New(env, "Buffer is required")
          .ThrowAsJavaScriptException();
      return env.Null();
    }

    if (!info[1].IsFunction())
    {
      Napi::TypeError::New(env, "Callback is required")
          .ThrowAsJavaScriptException();
      return env.Null();
    }

    Napi::Buffer<ubyte> bufferIn = info[0].As<Napi::Buffer<ubyte>>();

    Napi::Function callback = info[1].As<Napi::Function>();
    
    AsyncDecoder *asyncWorker = new AsyncDecoder(callback, bufferIn);
    asyncWorker->Queue();

    //std::vector<FrameInfo> frames = DecodeImpl(bufferIn.Data(), bufferIn.ByteLength());

    //return ToNodeArray(env, frames);
    return env.Undefined();
  }
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
  Napi::String name = Napi::String::New(env, "Raw264");
  exports.Set(name, DefineClass(env,
                                name,
                                {InstanceMethod("decode", &Raw264::Decode)}));

  return exports;
}

NODE_API_MODULE(addon, Init)