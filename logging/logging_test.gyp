{
  'includes': [
    '../common.gypi',
  ],
  'targets': [{
    'target_name': 'logging_test',
    'type': 'executable',
    'dependencies': [
      'logging.gyp:logging_lib',
      '../testing/testing.gyp:testing_lib',
      '../testing/testing.gyp:testing_main',
    ],
    'sources': [
      'string_format_test.cpp',
    ],
  }],
}
