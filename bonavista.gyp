{
  'includes': [
    'common.gypi',
  ],
  'targets': [{
    'target_name': 'libs',
    'type': 'none',
    'dependencies': [
      'base/base.gyp:base_lib',
      'file/file.gyp:file_lib',
      'logging/logging.gyp:logging_lib',
      'testing/testing.gyp:testing_lib',
    ],
  }, {
    'target_name': 'tests',
    'type': 'none',
    'dependencies': [
      'base/base.gyp:base_test',
      'file/file.gyp:file_test',
      'logging/logging_test.gyp:logging_test',
      'testing/testing.gyp:testing_test',
    ],
  }],
}
