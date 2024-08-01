project('orb', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++23'])

# Add all source files
sources = files()
subdir('src')

executable('lighter',
           sources,
           install : true)

# Run tests
subdir('tests')
