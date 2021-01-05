{
  'targets': [
    {
      'target_name': 'node-raw-264',
      'sources': [ 
        'src/h264_decoder.cc',
        'src/raw_264.cc'
       ],
      'include_dirs': ["<!@(node -p \"require('node-addon-api').include\")"],
      'dependencies': ["<!(node -p \"require('node-addon-api').gyp\")"],
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
          '/usr/local/lib/libavcodec.dylib',
          '/usr/local/lib/libswscale.dylib',
          '/usr/local/lib/libavutil.dylib'
        ]
      }
    }
  ]
}