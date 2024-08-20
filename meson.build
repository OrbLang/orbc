project('orbc', 'cpp',
  version : '0.0.1',
  default_options : ['warning_level=3', 'cpp_std=c++23'])


no_color = get_option('no-color')
if no_color
  add_global_arguments('-DORBF_NO_COLOR', language: ['cpp'])
endif


add_global_arguments('-stdlib=libc++', language: ['cpp'])
add_global_link_arguments('-stdlib=libc++', language: ['cpp'])

## Libraries
boost_preprocessor_inc = include_directories('deps/boost')
src_inc = include_directories('src')

# ICU unicode support
icu_unicode = dependency('icu-uc')

# Add all source files
sources = files()
subdir('src')

executable('orbc',
           'src/main.cpp',
           sources,
           install : true,
           include_directories : [boost_preprocessor_inc, src_inc])

# Run tests
subdir('tests')
