project('orbc', 'cpp',
  version : '0.0.1',
  default_options : ['warning_level=3', 'cpp_std=c++23'])

if get_option('no-color')
  add_global_arguments('-DORBF_NO_COLOR', language: ['cpp'])
endif

add_global_arguments('-stdlib=libc++', language: ['cpp'])
add_global_link_arguments('-stdlib=libc++', language: ['cpp'])

## Libraries
include = include_directories('include')

# ICU unicode support
add_global_arguments('-DUNISTR_FROM_STRING_EXPLICIT=explicit', language: ['cpp'])
icu_uc = dependency('icu-uc')
icu_io = dependency('icu-io')
icu_i18n = dependency('icu-i18n')

# Add all source files
sources = files()
subdir('src')

executable('orbc',
           'src/main.cpp',
           sources,
           install : true,
           include_directories : [include],
           dependencies: [icu_uc, icu_io, icu_i18n])

# Run tests
subdir('tests')
