{
  'includes': [
    '../common.gypi',
  ],
  'targets': [{
    'target_name': 'file_lib',
    'type': 'static_library',
    'sources': [
      'scoped_file.cpp',
      'scoped_file.h',
      'util.cpp',
      'util.h',
    ],
  }, {
    'target_name': 'file_test',
    'type': 'executable',
    'dependencies': [
      'file_lib',
      '../testing/testing.gyp:testing_lib',
      '../testing/testing.gyp:testing_main',
    ],
    'sources': [
      'scoped_file_test.cpp',
      'util_test.cpp',
    ],
  }],
}
