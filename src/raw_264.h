#pragma once

#include <napi.h>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <cassert>

#include "h264_decoder.h"

typedef unsigned char ubyte;

class Raw264 : public Napi::ObjectWrap<Raw264>
{
public:
  Raw264(const Napi::CallbackInfo &);
  Napi::Value Greet(const Napi::CallbackInfo &);
  Napi::Value Decode(const Napi::CallbackInfo &);

  static Napi::Function GetClass(Napi::Env);

private:
  std::string _greeterName;
  H264Decoder _decoder;
  ConverterRGB24 _converter;
};
