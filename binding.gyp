{
  "targets": [
    {
      "target_name": "nanosockets_binding",
      "sources": ["src/nanosockets.cpp"],
      'include_dirs': [
          "<!@(node -p \"require('node-addon-api').include\")",
          "<(module_root_dir)/src"
      ],
      'dependencies': ["<!(node -p \"require('node-addon-api').gyp\")"],
      "cflags!": ["-fno-exceptions"],
      "cflags_cc!": ["-fno-exceptions"],
      "xcode_settings": {
        "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
        "CLANG_CXX_LIBRARY": "libc++",
        "MACOSX_DEPLOYMENT_TARGET": "10.7"
      },
      "conditions": [
        ['OS=="win"', {
          "msvs_settings": {
            "VCCLCompilerTool": { "ExceptionHandling": 1 }
          }
        }]
      ]
    }
  ]
}
