#pragma once

#include "h264_decoder.h"
#include "async_decoder.h"
#include <napi.h>

class Raw264 : public Napi::ObjectWrap<Raw264>
{
public:
  Raw264(const Napi::CallbackInfo &);
  Napi::Value Decode(const Napi::CallbackInfo &)

private:
  H264Decoder _decoder;
  ConverterRGB24 _converter;
};


