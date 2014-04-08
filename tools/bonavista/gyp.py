import os

def Run(current_file, relative_gyp_path, root_gyp_file):
  os.chdir(os.path.dirname(os.path.realpath(current_file)))
  os.system('python %s %s' % (os.path.join(relative_gyp_path, 'gyp/gyp'), root_gyp_file))

if __name__ == '__main__':
  Run(__file__, '..', '../../src/bonavista/bonavista.gyp')
