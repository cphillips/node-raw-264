#include "raw_264.h"

using namespace Napi;

Raw264::Raw264(const Napi::CallbackInfo &info) : ObjectWrap(info)
{
  Napi::Env env = info.Env();

  if (info.Length() < 1)
  {
    Napi::TypeError::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return;
  }

  if (!info[0].IsString())
  {
    Napi::TypeError::New(env, "You need to name yourself")
        .ThrowAsJavaScriptException();
    return;
  }

  this->_greeterName = info[0].As<Napi::String>().Utf8Value();
}

Napi::Value Raw264::Greet(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() < 1)
  {
    Napi::TypeError::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsString())
  {
    Napi::TypeError::New(env, "You need to introduce yourself to greet")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  Napi::String name = info[0].As<Napi::String>();

  printf("Hello %s\n", name.Utf8Value().c_str());
  printf("I am %s\n", this->_greeterName.c_str());

  return Napi::String::New(env, this->_greeterName);
}

Napi::Value Raw264::Decode(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  Napi::Array result = Napi::Array::New(env);

  if (info.Length() < 1 || !info[0].IsBuffer())
  {
    Napi::TypeError::New(env, "Argument is not an Buffer.").ThrowAsJavaScriptException();
    return result;
  }

  Napi::Buffer<ubyte> bufferIn = info[0].As<Napi::Buffer<ubyte>>();

  size_t bufferInLen = bufferIn.ByteLength();
  size_t bytesConsumed = 0;
  ubyte *dataIn = bufferIn.Data();
  int frameIndex = 0;

  while (bufferInLen > 0)
  {
    bytesConsumed = _decoder.parse(dataIn, bufferInLen);

    //TODO:throw error if bytesConsumed < 0

    if (_decoder.is_frame_available())
    {
      Napi::Object frameInfo = Napi::Object::New(env);

      try
      {
        const auto &frame = _decoder.decode_frame();

        int w, h;
        std::tie(w, h) = width_height(frame);
        ssize_t outSize = _converter.predict_size(w, h);
        Napi::Buffer<ubyte> bufferOut = Napi::Buffer<ubyte>::New(env, outSize);
        const AVFrame &rgbFrame = _converter.convert(frame, bufferOut.Data());

        frameInfo.Set("buffer", bufferOut);
        frameInfo.Set("width", w);
        frameInfo.Set("height", h);
        frameInfo.Set("depth", row_size(rgbFrame));

        result.Set(frameIndex, frameInfo);

        frameIndex++;
      }
      catch (const H264DecodeFailure &e)
      {
      }
    }

    bufferInLen -= bytesConsumed;
    dataIn += bytesConsumed;
  }
  /*
    try
    {
      auto frame = decode_frame_impl(data_in, len, num_consumed, is_frame_available);
      if (is_frame_available)
      {
        out.append(frame);
      }
    }
    catch (const H264DecodeFailure &e)
    {
      if (num_consumed <= 0)
        // This case is fatal because we cannot continue to move ahead in the stream.
        throw e;
    }

    len -= num_consumed;
    data_in += num_consumed;
  }
  */

  //response.Set("isFrameAvailable", isFrameAvailable);

  return result;
}

Napi::Function Raw264::GetClass(Napi::Env env)
{
  return DefineClass(env, "TelloVideo",
                     {InstanceMethod("greet", &Raw264::Greet),
                      InstanceMethod("decode", &Raw264::Decode)});
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
  Napi::String name = Napi::String::New(env, "TelloVideo");

  exports.Set(name, Raw264::GetClass(env));
  return exports;
}

NODE_API_MODULE(addon, Init)
