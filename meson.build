project('orbc', 'cpp',
  version : '0.0.1',
  default_options : [
    'warning_level=3',
    'cpp_std=c++23', # potentially bump to 26?
    'optimization=3',
    'buildtype=release',
    'debug=false',
  ],
)

if get_option('no-color').enabled()
  add_project_arguments('-DORBF_NO_COLOR', language: 'cpp')
endif

# Stdlib
add_project_arguments('-stdlib=libc++', language: 'cpp')
add_project_link_arguments('-stdlib=libc++', language: 'cpp')

# Warnings and suggestions
add_project_arguments([
  '-pedantic',
  '-fsafe-buffer-usage-suggestions',
  ], language: 'cpp')

## Libraries
include = include_directories('include')

# ICU unicode support
add_global_arguments('-DUNISTR_FROM_STRING_EXPLICIT=explicit', language: 'cpp')

icu_uc = dependency('icu-uc', fallback: ['icu-uc', 'icu-uc_dep'])
icu_io = dependency('icu-io', fallback: ['icu-io', 'icu-io_dep'])
icu_i18n = dependency('icu-i18n', fallback: ['icu-i18n', 'icu-i18n_dep'])

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
