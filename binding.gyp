{
  'targets': [
    {
      'target_name': 'node-raw-264',
      'sources': [ 
        'src/h264_decoder.cc',
        'src/raw_264.cc'
       ],
      'include_dirs': [
        "<!@(node -p \"require('node-addon-api').include\")",
        "/usr/include/x86_64-linux-gnu"
       ],
      'dependencies': [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ],
      'xcode_settings': {
        'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
        'CLANG_CXX_LIBRARY': 'libc++',
        'MACOSX_DEPLOYMENT_TARGET': '10.15'
      },
      'msvs_settings': {
        'VCCLCompilerTool': { 'ExceptionHandling': 1 },
      },
      'link_settings':{
        'libraries':[
          '/usr/lib/x86_64-linux-gnu/libavcodec.so',
          '/usr/lib/x86_64-linux-gnu/libswscale.so',
          '/usr/lib/x86_64-linux-gnu/libavutil.so'
        ]
      }
    }
  ]
}