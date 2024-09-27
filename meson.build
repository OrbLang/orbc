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
compile_options = []
link_options = []

cpp = meson.get_compiler('cpp')

if get_option('no-color').enabled()
  compile_options += '-DORBF_NO_COLOR'
endif

# Change stdlib
if get_option('stdlib') != ''
  stdlib = get_option('stdlib')
  message('Setting standard library to: "' + stdlib + '"')
  stdlib_option = cpp.get_supported_arguments(
    '-stdlib=' + stdlib,
    checked: 'require'
  )

  compile_options += stdlib_option
endif

# Warnings and suggestions
add_project_arguments([
  '-pedantic',
  '-fsafe-buffer-usage-suggestions',
  ],
  language: 'cpp'
)

include = include_directories('include')

# ICU unicode support
add_global_arguments('-DUNISTR_FROM_STRING_EXPLICIT=explicit', language: 'cpp')
icu_uc   = dependency('icu-uc', fallback: ['icu-uc', 'icu-uc_dep'])
icu_io   = dependency('icu-io', fallback: ['icu-io', 'icu-io_dep'])
icu_i18n = dependency('icu-i18n', fallback: ['icu-i18n', 'icu-i18n_dep'])

if get_option('build-tests').enabled()
  subdir('tests')
endif
