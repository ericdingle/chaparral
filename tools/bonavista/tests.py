import glob
import os
import platform

if platform.system() == 'Windows':
  import win32com.client

def Run(current_file, relative_src_path, name):
  os.chdir(os.path.join(os.path.dirname(os.path.realpath(current_file)), relative_src_path))

  if platform.system() == 'Linux':
    if os.system('make tests'):
      exit('Build error.')
    tests = glob.glob('out/Debug/*_test')
  elif platform.system() == 'Windows':
    solution = win32com.client.GetObject(os.path.abspath(r'%s\%s.sln' % (name, name)))
    builder = solution.SolutionBuild
    builder.BuildProject('Debug', os.path.abspath(r'%s\tests.vcproj' % name), True)
    if builder.LastBuildInfo:
      exit('Build error.')
    tests = glob.glob(r'%s\Debug\*_test.exe' % name)
  else:
    exit('Platform not supported.')
  
  for test in tests:
    if os.system(test):
      exit()

if __name__ == '__main__':
  Run(__file__, '../../src', 'bonavista')
