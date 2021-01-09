
#include <napi.h>
#include <iostream>
#include "h264_decoder.h"

typedef unsigned char ubyte;

class Raw264 : public Napi::ObjectWrap<Raw264>
{
public:
  Raw264(const Napi::CallbackInfo &info)
      : ObjectWrap(info) {}

  Napi::Value Decode(const Napi::CallbackInfo &info)
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

      if (_decoder.is_frame_available())
      {
        Napi::Object frameInfo = Napi::Object::New(env);

        try
        {
          const auto &frame = _decoder.decode_frame();

          int w, h;
          std::tie(w, h) = width_height(frame);
          ssize_t outSize = _converter.predict_size(w, h);
          Napi::ArrayBuffer bufferOut = Napi::ArrayBuffer::New(env, outSize);
          const AVFrame &rgbFrame = _converter.convert(frame, (ubyte*)bufferOut.Data());

          frameInfo.Set("buffer", bufferOut);
          frameInfo.Set("width", w);
          frameInfo.Set("height", h);
          frameInfo.Set("rowSize", row_size(rgbFrame));

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

    return result;
  }

private:
  H264Decoder _decoder;
  ConverterRGB24 _converter;
};

Napi::Object Init(Napi::Env env, Napi::Object exports)
{

  const char *name = "Raw264";
  exports.Set(name,
              Raw264::DefineClass(env, name, {
                Raw264::InstanceMethod("decode", &Raw264::Decode)
              }));
  return exports;
}

NODE_API_MODULE(addon, Init)
