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
cpp = meson.get_compiler('cpp')
pch = meson.source_root() / 'pch/pch.hpp'

compile_options = []
link_options = []
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
  link_options += stdlib_option
endif

foreach option : get_option('compile-opts').strip().split(',')
  foreach sub_option : option.strip().split(' ')
    compile_options += sub_option.strip()
  endforeach
endforeach

foreach option : get_option('link-opts').strip().split(',')
  foreach sub_option : option.strip().split(' ')
    link_options += sub_option.strip()
  endforeach
endforeach


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

subdir('src')

if get_option('build-tests').enabled()
  subdir('tests')
endif
