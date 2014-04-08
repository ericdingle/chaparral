{
  'includes': [
    '../common.gypi',
  ],
  'targets': [{
    'target_name': 'base_lib',
    'type': 'static_library',
    'sources': [
      'command_line.cpp',
      'command_line.h',
      'macros.h',
      'types.h',
    ],
  }, {
    'target_name': 'base_test',
    'type': 'executable',
    'dependencies': [
      'base_lib',
      '../testing/testing.gyp:testing_lib',
      '../testing/testing.gyp:testing_main',
    ],
    'sources': [
      'command_line_test.cpp',
    ],
  }],
}
