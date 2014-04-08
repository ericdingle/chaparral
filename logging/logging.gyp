{
  'includes': [
    '../common.gypi',
  ],
  'targets': [{
    'target_name': 'logging_lib',
    'type': 'static_library',
    'sources': [
      'color_printf.cpp',
      'color_printf.h',
      'string_format.cpp',
      'string_format.h',
    ],
  }],
}
