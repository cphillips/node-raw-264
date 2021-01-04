#include <napi.h>
#include "h264_decoder.h"
#include <string>
#include <iostream>
#include <vector>
#include <tuple>

typedef unsigned char ubyte;

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
    Napi::Function callback = info[1].As<Napi::Function>();

    //passing in the data direction worked
    AsyncDecoder *asyncWorker = new AsyncDecoder(callback, bufferIn.Data(), bufferIn.ByteLength());
    asyncWorker->Queue();

    //asyncWorker->Execute();

    return env.Undefined();
  }

protected:
  AsyncDecoder(Napi::Function callback, ubyte *dataIn, size_t dataInLen)
      : Napi::AsyncWorker(callback), _dataIn(dataIn), _dataInLen(dataInLen)
  {
    //_bufferIn = Napi::Reference<Napi::Buffer<ubyte>>::New(bufferIn,1);
    //_dataIn = dataIn;
    //_dataInLen = dataInLen;
  }

  void Execute() override
  {

    try
    {
      size_t bytesConsumed = 0;

      while (_dataInLen > 0)
      {
        bytesConsumed = _decoder.parse(_dataIn, _dataInLen);

        //std::cout << "bytesConsumed:" << bytesConsumed << std::endl;

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
            _frames.push_back(frameInfo);
          }
          catch (...)
          {
          }
          //std::cout << "_dataInLen:" << _dataInLen << std::endl;
        }

        _dataInLen -= bytesConsumed;
        _dataIn += bytesConsumed;

        //std::cout << "EXIT" << std::endl;
      }
    }
    catch (const std::exception &exc)
    {
      std::cout << exc.what();
    }
  }

  virtual void OnOK() override
  {
    Napi::Array frames = Napi::Array::New(Env());

    //std::cout << _frames.size();

    for (std::vector<int>::size_type i = 0; i != _frames.size(); i++)
    {

      Napi::Object frameInfo = Napi::Object::New(Env());

      auto width = std::get<0>(_frames[i]);
      auto height = std::get<1>(_frames[i]);
      auto rowSize = std::get<2>(_frames[i]);
      auto buffer = std::get<3>(_frames[i]);
      auto bufferLen = std::get<4>(_frames[i]);
      Napi::Buffer<ubyte> frameBuffer =
          Napi::Buffer<ubyte>::New(Env(), buffer, bufferLen);

      frameBuffer.AddFinalizer(
          [](Napi::Env /*env*/, ubyte *ref) {
            delete[] ref;
          },
          buffer);

      frameInfo.Set("width", width);
      frameInfo.Set("height", height);
      frameInfo.Set("rowSize", rowSize);
      frameInfo.Set("frame", frameBuffer);

      frames.Set(i, frameInfo);
    }

    Callback().Call({Env().Null(), frames});
  }

  virtual void OnError(const Napi::Error &e) override
  {
  }

private:
  std::vector<std::tuple<size_t, size_t, size_t, ubyte *, size_t>> _frames;
  Napi::Reference<Napi::Buffer<ubyte>> _bufferIn;
  ubyte *_dataIn;
  size_t _dataInLen;
  H264Decoder _decoder;
  ConverterRGB24 _converter;
};

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
  //return Napi::Function::New(env, AsyncDecoder::Decode);

  exports.Set(
      Napi::String::New(env, "decode"), Napi::Function::New(env, AsyncDecoder::Decode));
  return exports;
}

NODE_API_MODULE(addon, Init)