{
  "targets": [
    {
      "target_name": "secure_http_client",
      "sources": [ "secure_http_client_napi.cpp", "xios.cpp" ],
      "include_dirs": [
        "<!(node -p \"require('node-addon-api').include\")",
        "/Users/kylemcgrath/homebrew/Cellar/wolfssl/5.7.6/include",
        "/Users/kylemcgrath/homebrew/Cellar/sqlite/3.49.1/include",
        "/Users/kylemcgrath/Documents/GitHub/wolfXios/node_modules/node-addon-api"
      ],
      "libraries": [
        "-L/Users/kylemcgrath/homebrew/Cellar/wolfssl/5.7.6/lib -lwolfssl",
        "-L/Users/kylemcgrath/homebrew/Cellar/sqlite/3.49.1/lib -lsqlite3"
      ],
       "defines": [ "NODE_ADDON_API_CPP_EXCEPTIONS_ALL", "NODE_ADDON_API_CPP_EXCEPTIONS", "NAPI_CPP_EXCEPTIONS" ],
  'cflags': [ '-fexceptions' ],
  'cflags_cc': [ '-fexceptions' ],
  'conditions': [
    ["OS=='win'", {
      "msvs_settings": {
        "VCCLCompilerTool": {
          'ExceptionHandling': 1,
          'EnablePREfast': 'true',
        },
      },
    }],
    ["OS!='win'", {
      'xcode_settings': {
        'CLANG_CXX_LIBRARY': 'libc++',
        'MACOSX_DEPLOYMENT_TARGET': '15.0',
        'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
      },
      "variables": {
        "cflags_cc!": [ "-fno-exceptions" ],
        "cflags!": [ "-fno-exceptions" ],
        "release_cflags_cc!": [ "-fno-exceptions" ],
        "release_cflags!": [ "-fno-exceptions" ]
      },
    }],
  ]
    }
  ]
}