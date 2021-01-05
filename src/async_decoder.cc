#include <napi.h>
#include "h264_decoder.h"
#include <string>
#include <iostream>
#include <vector>
#include <tuple>
#include <thread>
#include <chrono>

typedef unsigned char ubyte;
typedef std::tuple<int, int, int, ubyte *, size_t> FrameInfo;

    H264Decoder _decoder;
    ConverterRGB24 _converter;
    
class AsyncDecoder : public Napi::AsyncWorker
{
public:
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
    
    // Napi::Function callback = info[1].As<Napi::Function>();
    //AsyncDecoder *asyncWorker = new AsyncDecoder(callback, bufferIn);
    //asyncWorker->Queue();

    std::vector<FrameInfo> frames = DecodeImpl(bufferIn.Data(), bufferIn.ByteLength());

    return ToNodeArray(env, frames);
  }

private:
  //this allocates memory, caller must delete
  static std::vector<FrameInfo> DecodeImpl(ubyte *dataIn, size_t dataInLen)
  {
    std::vector<FrameInfo> frames;
    try
    {
      size_t bytesConsumed = 0;

      while (dataInLen > 0)
      {
   
        bytesConsumed = _decoder.parse(dataIn, dataInLen);
        std::cout << "BUFFER_PARSED!" << std::endl;

        if (_decoder.is_frame_available())
        {
          std::cout << "FRAME_FOUND!" << std::endl;
          try
          {
            const auto &frame = _decoder.decode_frame();
            std::cout << "FRAME_DECODED!" << std::endl;
            int w, h;
            std::tie(w, h) = width_height(frame);

            size_t outSize = _converter.predict_size(w, h);
            ubyte *bufferOut = new ubyte[outSize];
            const AVFrame &rgbFrame = _converter.convert(frame, bufferOut);

            auto frameInfo = std::make_tuple(w, h, row_size(rgbFrame), bufferOut, outSize);
            frames.push_back(frameInfo);
          }
          catch (...)
          {
            std::cout << "FRAME_DECODED_FAILED!" << std::endl;
          }
        }

        dataInLen -= bytesConsumed;
        dataIn += bytesConsumed;
      }
    }
    catch (const std::exception &exc)
    {
      std::cout << exc.what();
    }
    return frames;
  }

  static Napi::Array ToNodeArray(Napi::Env env, std::vector<FrameInfo> frames)
  {
    Napi::Array frameArray = Napi::Array::New(env);

    for (std::vector<int>::size_type i = 0; i != frames.size(); i++)
    {

      Napi::Object frameInfo = Napi::Object::New(env);

      auto width = std::get<0>(frames[i]);
      auto height = std::get<1>(frames[i]);
      auto rowSize = std::get<2>(frames[i]);
      auto buffer = std::get<3>(frames[i]);
      auto bufferLen = std::get<4>(frames[i]);
      Napi::Buffer<ubyte> frameBuffer =
          Napi::Buffer<ubyte>::New(env, buffer, bufferLen);

      frameBuffer.AddFinalizer(
          [](Napi::Env /*env*/, ubyte *ref) {
            delete[] ref;
          },
          buffer);

      frameInfo.Set("width", width);
      frameInfo.Set("height", height);
      frameInfo.Set("rowSize", rowSize);
      frameInfo.Set("frame", frameBuffer);

      frameArray.Set(i, frameInfo);
    }

    return frameArray;
  }

protected:
  AsyncDecoder(Napi::Function callback, Napi::Buffer<ubyte> &buffer)
      : Napi::AsyncWorker(callback),
        _bufferInRef(Napi::Reference<Napi::Buffer<ubyte>>::New(buffer)),
        _dataIn(buffer.Data()),
        _dataInLen(buffer.ByteLength()){}

  void Execute() override
  {
    _framesOut = DecodeImpl(_dataIn, _dataInLen);
  }

  virtual void OnOK() override
  {
    auto framesOut = ToNodeArray(Env(),_framesOut);
    Callback().Call({Env().Null(),framesOut});
  }

  virtual void OnError(const Napi::Error &e) override
  {
  }

private:
  std::vector<FrameInfo> _framesOut;
  Napi::Reference<Napi::Buffer<ubyte>> _bufferInRef;
  ubyte *_dataIn;
  size_t _dataInLen;
};

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
  exports.Set(
      Napi::String::New(env, "decode"), Napi::Function::New(env, AsyncDecoder::Decode));
  return exports;
}

NODE_API_MODULE(addon, Init)