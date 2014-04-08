{
  'target_defaults': {
    'conditions': [
      ['OS=="linux"', {
        'cflags': ['-std=c++0x', '-Wall', '-Wextra', '-pedantic'],
        'defines': ['OS_LINUX'],
      }],
      ['OS=="win"', {'defines': ['OS_WIN']}],
    ],
    'configurations': {
      'Debug': {
        'conditions': [
          ['OS=="linux"', {'cflags': ['-ggdb']}],
        ],
        'defines': ['DEBUG'],
        'msvs_settings': {
          'VCCLCompilerTool': {
            'BasicRuntimeChecks': '3',
            'DebugInformationFormat': '3',
            'Optimization': '0',
            'RuntimeLibrary': '3',
            'WarningLevel': '3',
          },
          'VCLinkerTool': {
            'GenerateDebugInformation': 'true',
            'TargetMachine': '1',
          },
        },
      },
      'Release': {
        'conditions': [
          ['OS=="linux"', {'cflags': ['-O2']}],
        ],
        'msvs_settings': {
          'VCCLCompilerTool': {
            'EnableFunctionLevelLinking': 'true',
            'EnableIntrinsicFunctions': 'true',
            'Optimization': '2',
            'RuntimeLibrary': '2',
            'WarningLevel': '3',
            'WholeProgramOptimization': 'true',
          },
          'VCLinkerTool': {
            'EnableCOMDATFolding': '2',
            'LinkTimeCodeGeneration': '1',
            'OptimizeReferences': '2',
            'TargetMachine': '1',
          },
        },
      },
    },
    'include_dirs': [
      '<(DEPTH)',
    ],
  },
}
